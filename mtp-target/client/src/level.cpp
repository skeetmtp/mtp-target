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

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

#include <nel/misc/file.h>
#include <nel/misc/path.h>

#include <nel/3d/u_instance.h>

#include "level.h"
#include "3d_task.h"
#include "hud_task.h"
#include "mtp_target.h"
#include "entity_manager.h"
#include "config_file_task.h"
#include "editor_task.h"
#include "resource_manager.h"
#include "../../common/lua_nel.h"
#include "../../common/lua_utility.h"


//
// Namespaces
//

using namespace std;
using namespace NL3D;
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

CLevel::CLevel(const string &filename)
{
	nlinfo("Trying to load level '%s'", filename.c_str());

	Valid = false;
	_changed = false;

	DisplayStartPositions = CConfigFileTask::instance().configFile().getVar("DisplayStartPositions").asInt() == 1;
	DisplayLevel  = CConfigFileTask::instance().configFile().getVar("DisplayLevel").asInt() == 1;

	LuaState = luaOpenAndLoad(filename);

	luaGetGlobalVariable(LuaState, Name);
	nlinfo("level name '%s'", Name.c_str());
	
	luaGetGlobalVariable(LuaState, Author);
	nlinfo("author name '%s'", Author.c_str());
	
	vector<CLuaVector> Cameras;
	luaGetGlobalVector(LuaState, Cameras);
	
	for(uint i = 0; i < Cameras.size(); i++)
	{
		nlinfo("camera %g %g %g", Cameras[i].x, Cameras[i].y, Cameras[i].z);

		uint8 eid = CMtpTarget::instance().controler().getControledEntity();
		if (i == 0 || eid != 255 && CEntityManager::instance()[eid].rank() == (uint8)i)
		{
			CMtpTarget::instance().controler().Camera.setInitialPosition(Cameras[i]);
		}
	}

	vector<CLuaVector> StartPoints;
	luaGetGlobalVector(LuaState, StartPoints);
	uint8 startPositionId = 0;
	for(uint i = 0; i < StartPoints.size(); i++)
	{
		nlinfo("%g %g %g", StartPoints[i].x, StartPoints[i].y, StartPoints[i].z);
		CAngleAxis Rotation(CVector(1,0,0),0);
		CStartPosition *startPosition = new CStartPosition("start pos",startPositionId,StartPoints[i],Rotation);
		if (!DisplayStartPositions)
			startPosition->hide();
		StartPositions.push_back(startPosition);
		
			/*
		string res = CResourceManager::instance().get("col_box.shape");
		UInstance *inst = C3DTask::instance().scene().createInstance (res);
		if (!inst)
		{
			nlwarning ("Can't load 'col_box.shape'");
			return;
		}

		inst->setTransformMode(UTransformable::RotQuat);
		inst->setPos(StartPoints[i]);

		if (!DisplayStartPositions)
			inst->hide();

		StartPositions.push_back(inst);
		*/
		startPositionId++;
	}


	// Load modules
	lua_getglobal(LuaState, "Modules");
	lua_pushnil(LuaState);
	uint8 moduleId = 0;
	while(lua_next(LuaState, -2) != 0)
	{
		// `key' is at index -2 and `value' at index -1

		// Get module position
		CLuaVector Position;
		lua_pushstring(LuaState,"Position");
		lua_gettable(LuaState, -2);
		luaGetVariable(LuaState, Position);		
		nlinfo("pos %g %g %g", Position.x, Position.y, Position.z);
		lua_pop(LuaState, 1);  // removes `value'; keeps `key' for next iteration
		
		// Get module rotation
		CLuaAngleAxis Rotation;
		lua_pushstring(LuaState,"Rotation");
		lua_gettable(LuaState, -2);
		luaGetVariable(LuaState, Rotation);		
		nlinfo("rot %g %g %g %g", Rotation.Axis.x , Rotation.Axis.y, Rotation.Axis.z, Rotation.Angle);
		lua_pop(LuaState, 1);  // removes `value'; keeps `key' for next iteration
		
		// Get module name
		string Name;
		lua_pushstring(LuaState,"Name");
		lua_gettable(LuaState, -2);
		luaGetVariable(LuaState, Name);		
		nlinfo("name %s", Name.c_str());
		lua_pop(LuaState, 1);  // removes `value'; keeps `key' for next iteration

		CModule *module = new CModule(Name,moduleId,Position,Rotation);
		moduleId++;
		if(!DisplayLevel)
			module->hide();
	
		Modules.push_back(module);

	}
	lua_pop(LuaState, 1);  // removes `key'

	FileName = filename;

	DisplayStartPositions = false;
	DisplayLevel = true;

	Valid = true;
	if(C3DTask::instance().levelParticle()!=0)
		C3DTask::instance().levelParticle()->show();
}

CLevel::~CLevel()
{
	if(C3DTask::instance().levelParticle()!=0)
		C3DTask::instance().levelParticle()->hide();

	if(changed())
		CResourceManager::instance().refresh(FileName);

	CEditorTask::instance().reset();

	if(LuaState)
		luaClose(LuaState);

	// removing all stuffs


	for(uint i = 0; i < Modules.size(); i++)
	{
		delete Modules[i];
	}
	Modules.clear();

	for(uint j = 0; j < StartPositions.size(); j++)
	{
		delete StartPositions[j];
		//C3DTask::instance().scene().deleteInstance(StartPositions[j]);
	}
	StartPositions.clear();
}

CVector CLevel::startPosition(uint32 id)
{
	return StartPositions[id]->position();
}

void CLevel::reset()
{
	nlinfo("--level reset--");
	CHudTask::instance().messages.clear();
	luaClose(LuaState);

	LuaState = luaOpenAndLoad(FileName);
	
	luaGetGlobalVariable(LuaState, Name);
	nlinfo("level name '%s'", Name.c_str());
	
	luaGetGlobalVariable(LuaState, Author);
	nlinfo("author name '%s'", Author.c_str());
	
	vector<CLuaVector> Cameras;
	luaGetGlobalVector(LuaState, Cameras);
	
	for(uint i = 0; i < Cameras.size(); i++)
	{
		nlinfo("camera %g %g %g", Cameras[i].x, Cameras[i].y, Cameras[i].z);
		uint8 eid = CMtpTarget::instance().controler().getControledEntity();
		if (i == 0 || eid != 255 && CEntityManager::instance()[eid].rank() == (uint8)i)
		{
			CMtpTarget::instance().controler().Camera.setInitialPosition(Cameras[i]);
		}
	}
	
/*


	vector<CLuaVector> StartPoints;
	luaGetGlobalVector(LuaState, StartPoints);
	for(uint i = 0; i < StartPoints.size(); i++)
	{
		nlinfo("%g %g %g", StartPoints[i].x, StartPoints[i].y, StartPoints[i].z);
		string res = CResourceManager::instance().get("col_box.shape");
		UInstance *inst = StartPositions[i];
		
		inst->setTransformMode(UTransformable::RotQuat);
		inst->setPos(StartPoints[i]);
		
		if (!DisplayStartPositions)
			inst->hide();
		
		StartPositions.push_back(inst);
	}
	
	
	// Load modules
	lua_getglobal(LuaState, "Modules");
	lua_pushnil(LuaState);
	int j = 0;
	while(lua_next(LuaState, -2) != 0)
	{
		// `key' is at index -2 and `value' at index -1
		
		// Get module position
		CLuaVector Position;
		lua_pushstring(LuaState,"Position");
		lua_gettable(LuaState, -2);
		luaGetVariable(LuaState, Position);		
		nlinfo("pos %g %g %g", Position.x, Position.y, Position.z);
		lua_pop(LuaState, 1);  // removes `value'; keeps `key' for next iteration
		
		// Get module name
		string Name;
		lua_pushstring(LuaState,"Name");
		lua_gettable(LuaState, -2);
		luaGetVariable(LuaState, Name);		
		nlinfo("name %s", Name.c_str());
		lua_pop(LuaState, 1);  // removes `value'; keeps `key' for next iteration
		
		string res = CResourceManager::instance().get(Name+".shape");
		UInstance *inst = Meshes[j++];
		if (inst == 0)
		{
			nlwarning ("Can't load '%s.shape'", Name.c_str());
		}
		else
		{
			inst->setTransformMode(UTransformable::RotQuat);
			inst->setPos(Position);
			
			if (!DisplayLevel)
				inst->hide();
		}
	}
	lua_pop(LuaState, 1);  // removes `key'
	*/
}


void CLevel::display(CLog *log) const
{
	log->displayNL("LevelName = %s", name().c_str());
	log->displayNL("There's %d starting points", StartPositions.size());
	log->displayNL("There's %d Modules", Modules.size());
}

void CLevel::displayStartPositions(bool b)
{
	DisplayStartPositions = b;
	
	if(DisplayStartPositions)
	{
		for(uint j = 0; j < StartPositions.size(); j++)
		{
			StartPositions[j]->show();
		}
	}
	else
	{
		for(uint j = 0; j < StartPositions.size(); j++)
		{
			StartPositions[j]->hide();
		}
	}
}

void CLevel::displayLevel(bool b)
{
	DisplayLevel = b;
	
	if(DisplayLevel)
	{
		for(uint i = 0; i < Modules.size(); i++)
		{
			Modules[i]->mesh()->show ();
		}
	}
	else
	{
		for(uint i = 0; i < Modules.size(); i++)
		{
			Modules[i]->mesh()->hide ();
		}
	}
}


CModule *CLevel::getModule(uint32 index)
{
	nlassert(index<getModuleCount());
	return Modules[index];
}

uint32 CLevel::getModuleCount()
{
	nlassert(Modules.size()<255);
	return Modules.size();
}

void CLevel::updateModule(uint32 id,CVector pos,CVector rot,uint32 selectedBy)
{
	getModule(id)->update(pos,rot,selectedBy);
}

CStartPosition *CLevel::getStartPosition(uint32 index)
{
	nlassert(index<getStartPositionCount());
	return StartPositions[index];
}

uint32 CLevel::getStartPositionCount()
{
	nlassert(StartPositions.size()<255);
	return StartPositions.size();
}

void CLevel::updateStartPosition(uint32 id,CVector pos,CVector rot,uint32 selectedBy)
{
	getStartPosition(id)->update(pos,rot,selectedBy);
}

