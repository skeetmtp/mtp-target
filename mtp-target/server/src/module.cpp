/* Copyright, 2003 Melting Pot
 *
 * This file is part of MTP Target.
 * MTP Target is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * MTP Target is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with MTP Target; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */


//
// Includes
//

#include "stdpch.h"

#include <nel/misc/path.h>

#include "module.h"
#include "physics.h"
#include "load_mesh.h"
#include "../../common/lua_utility.h"
#include "lua_engine.h"

//
// Namespaces
//

using namespace std;
using namespace NLMISC;


//
// Types
//


//
// Declarations
//


//
// Variables
//


//
// Functions
//

static int _dTriCallback(dGeomID TriMesh, dGeomID RefObject, int TriangleIndex)
{
	return 0;
}

static void _dTriArrayCallback(dGeomID TriMesh, dGeomID RefObject, const int* TriIndices, int TriCount)
{
}

static int _dTriRayCallback(dGeomID TriMesh, dGeomID Ray, int TriangleIndex, dReal u, dReal v)
{
	return 0;
}

void CModule::init()
{
	Geom = 0;
	Bounce = true;
	Score = 0;
	Accel = 0.0001f;
	Friction = 5.0f;
	LuaFunctionName = "";
	luaProxy = NULL;
	_changed = false;

	_type = Module;
}

CModule::CModule() : CEditableElement()
{ 
	init(); 
}

CModule::CModule(const std::string &name, const CLuaVector &position, const CLuaAngleAxis &rotation, uint8 id) : CEditableElement()
{
	nlinfo("Adding module '%s' at position %f %f %f", name.c_str(), position.x, position.y, position.z);
	pausePhysics();
	_id = id;
	init();
	ShapeName = name+".shape";
	Name = name;
	Position = position;
	Rotation = rotation;
	_luaInit();
	
	// Get collision faces
	vector<CVector> vertices;
	vector<int> indices;
//	loadMesh(name+".shape", vertices, indices);
	loadMesh(ShapeName, vertices, indices);
	
	// vertices must be multiple of 3
	Vertices.resize(vertices.size()*3);
	for(uint i = 0; i < vertices.size(); i++)
	{
		Vertices[i*3+0] = vertices[i].x /** triColl.scaleX*/;
		Vertices[i*3+1] = vertices[i].y /** triColl.scaleY*/;
		Vertices[i*3+2] = vertices[i].z /** triColl.scaleZ*/;
//		nlinfo("%d %f %f %f", i, Vertices[i*3+0], Vertices[i*3+1], Vertices[i*3+2]);
	}
	Indices.resize(indices.size());
	for(uint i = 0; i < indices.size(); i++)
	{
		Indices[i] = indices[i];
//		nlinfo("%d %d", i, Indices[i]);
	}
	dTriMeshDataID data = dGeomTriMeshDataCreate();
//	dGeomTriMeshDataBuildSingle(data, &Vertices[0], 3*sizeof(dReal), Vertices.size()/3, &Indices[0], Indices.size(), 3*sizeof(int));
	dGeomTriMeshDataBuild(data, &Vertices[0], 3*sizeof(dReal), Vertices.size()/3, &Indices[0], Indices.size(), 3*sizeof(int));

	{
		CSynchronized<dSpaceID>::CAccessor acces(&Space);
		Geom = dCreateTriMesh(acces.value(), data, _dTriCallback, _dTriArrayCallback, _dTriRayCallback);
	}
	nlassert(Geom);
	dGeomSetData(Geom, this);

	//  OLD ODE CODE FOR TRI COL
	//	dGeomID userGeomId;
	//	entity->geom = userGeomId = dCreateTriList(space, _dTriCallback, _dTriArrayCallback, _dTriRayCallback);
	//	dGeomSetData(userGeomId, entity);
	//
	//	dVector3 *f = &entity->vertices[0];
	//	dGeomTriListBuild(userGeomId, &entity->vertices[0], sizeof(dcVector3), entity->vertices.size(), &entity->indices[0], sizeof(int), entity->indices.size(), 3 * sizeof(int));

	dGeomSetPosition(Geom, position.x, position.y, position.z);
	dMatrix3 R;
	dRFromAxisAndAngle(R, rotation.Axis.x, rotation.Axis.y, rotation.Axis.z, rotation.Angle);
	dGeomSetRotation(Geom, R);
	/*	entity->setRotation(CAngleAxis(CVector(triColl.axisX, triColl.axisY, triColl.axisZ), triColl.angle));
	
	entity->Score = triColl.score;
	entity->Accel = triColl.accel;
	entity->Friction = triColl.friction;
	entity->Bounce = triColl.bounce;
	entity->LuaFunctionName = triColl.luaFunctionName;
	entity->Name = triColl.name;
*/	
	dGeomSetCategoryBits(Geom, MT_SCENE_SPECTRUM);
	dGeomSetCollideBits(Geom, MT_CLIENT_SPECTRUM);

	resumePhysics();

}

void CModule::_luaInit()
{
	luaProxy = new CModuleProxy(CLuaEngine::instance().session(),this);

	// load the lua code for this module
	string filename = CPath::lookup("module_" + Name + ".lua", false);
	if(filename.empty())
	{
		nlinfo("MODULE: There's no lua code for module '%s', using default value", filename.c_str());
		return;
	}
	

	lua_pushliteral(CLuaEngine::instance().session(), "module");
	Lunar<CModuleProxy>::push(CLuaEngine::instance().session(), luaProxy);
	lua_settable(CLuaEngine::instance().session(), LUA_GLOBALSINDEX);
	
	luaLoad(CLuaEngine::instance().session(),filename);

}

CModule::~CModule()
{
	pausePhysics();
	if(Geom)
	{
		dGeomSetData(Geom, (void *)0xDEADBEEF);
		dGeomDestroy(Geom);
		Geom = 0;
	}
	resumePhysics();
}

NLMISC::CVector CModule::position() const
{
	CVector pos;
	nlassert(Geom);
	const dReal *p = dGeomGetPosition(Geom);
	if(p)
	{
		pos.x = p[0];
		pos.y = p[1];
		pos.z = p[2];
	}
	else
		pos = NLMISC::CVector::Null;

	return pos;
}

void CModule::display(CLog *log) const
{
	CVector pos = position();
	log->displayNL("  name '%s' pos (%.2g, %.2g, %.2g) col: %d vertices %d faces", name().c_str(), pos.x, pos.y, pos.z, Vertices.size()/3, Indices.size()/3);
	log->displayNL("  score %d accel %g friction %g %sbounce", score(), accel(), friction(), (bounce()?"":"no "));
}


void CModule::bounce(bool b) 
{
	Bounce = b; 
}

void CModule::update() 
{
	if(luaProxy)
		luaProxy->call("update");
}

void CModule::initBeforeStartLevel()
{
	if(luaProxy)
		luaProxy->call("init");
}


void CModule::update(CVector pos,CVector rot)
{	
	pausePhysics();
	dGeomSetPosition(Geom, pos.x, pos.y, pos.z);
	resumePhysics();
	Position = pos;
	_changed = true;
}


void CModule::save(FILE *fp)
{
	fprintf(fp,"{ Position = CVector(%f,%f,%f),Rotation = CAngleAxis(%f,%f,%f,%f), Name=\"%s\" }",Position.x,Position.y,Position.z,Rotation.Axis.x,Rotation.Axis.y,Rotation.Axis.z,Rotation.Angle,Name);
}

bool CModule::changed()
{
	return _changed;
}

//
//
//
