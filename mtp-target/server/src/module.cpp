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
#include "../../common/load_mesh.h"
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

CModule::CModule() : CModuleCommon()
{
	luaProxy = NULL;
}

void CModule::init(const std::string &name,uint8 id, NLMISC::CVector position, NLMISC::CAngleAxis rotation)
{
	CModuleCommon::init(name,id,position,rotation);
	
	Geom = 0;
	luaProxy = NULL;
	_luaInit();
	
	// vertices must be multiple of 3
	OdeVertices.resize(Vertices.size()*3);
	for(uint i = 0; i < Vertices.size(); i++)
	{
		OdeVertices[i*3+0] = Vertices[i].x /** triColl.scaleX*/;
		OdeVertices[i*3+1] = Vertices[i].y /** triColl.scaleY*/;
		OdeVertices[i*3+2] = Vertices[i].z /** triColl.scaleZ*/;
//		nlinfo("%d %f %f %f", i, Vertices[i*3+0], Vertices[i*3+1], Vertices[i*3+2]);
	}
	dTriMeshDataID data = dGeomTriMeshDataCreate();

//	dGeomTriMeshDataBuildSingle(data, &OdeVertices[0], 3*sizeof(dReal), OdeVertices.size()/3, &Indices[0], Indices.size(), 3*sizeof(int));
	dGeomTriMeshDataBuild(data, &OdeVertices[0], 3*sizeof(dReal), OdeVertices.size()/3, &Indices[0], Indices.size(), 3*sizeof(int));

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
	if(luaProxy)
		delete luaProxy;
	luaProxy = NULL;
	if(Geom)
	{
		dGeomSetData(Geom, (void *)0xDEADBEEF);
		dGeomDestroy(Geom);
		Geom = 0;
	}
}

/*
NLMISC::CVector CModule::position() const
{
	return Position;

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

*/


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



//
//
//
