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
#include <nel/net/service.h>

//#include "main.h"
#include "network.h"
#include "lua_engine.h"
#include "module.h"
#include "entity.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;


//
//
//

static bool luaCodeIsValid = false;
#define LUA_CODE_CHECK do{if(!luaCodeIsValid || LuaSession==0) {luaCodeIsValid=false; return;}} while(false)

#define MT_LUA_GET_GLOBAL(funcName) \
do{ \
	lua_getglobal(LuaSession, funcName); \
	if(! lua_isfunction(LuaSession, -1)) \
	{	\
		nlwarning("can't get lua function : %s",funcName); \
		luaCodeIsValid = false; \
		return; \
	}	\
}while(false)

#define MT_LUA_CALL(argCount,retCount) \
do \
{ \
	int status = 0; lua_call(LuaSession, argCount, retCount); \
	if(status != 0) \
	{ \
		nlwarning("lua_call() failed !"); \
		luaCodeIsValid = false; \
		return; \
	} \
}while(false)



//////////////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////////////

CLuaEngine::CLuaEngine()
{
	_luaSession = NULL;
}


void CLuaEngine::init(const std::string &filename)
{
	if(_luaSession)
		luaClose(_luaSession);

	_luaSession = luaOpen();
	if(!_luaSession)
	{
		nlwarning("LEVEL: luaOpenAndLoad() failed while trying to load level '%s'", filename.c_str());
		return;
	}

	Lunar<CModuleProxy>::Register(_luaSession);
	Lunar<CEntityProxy>::Register(_luaSession);
	string path = CPath::lookup("helpers.lua", false, false);
	luaLoad(_luaSession,path);
	luaLoad(_luaSession,filename);
	
}

void CLuaEngine::update()
{

}

void CLuaEngine::release()
{
	if(_luaSession)
		luaClose(_luaSession);
	_luaSession = NULL;
}

void CLuaEngine::entityEntityCollideEvent(CEntity *entity1, CEntity *entity2)
{
	if(!_luaSession)
		return;
	int res ;
//	nlinfo("CLuaEngine::entityEntityCollideEvent(0x%p[0x%p],0x%p[0x%p])",entity1,entity1->luaProxy,entity2,entity2->luaProxy);
	lua_getglobal(_luaSession, "entityEntityCollideEvent");
	Lunar<CEntityProxy>::push(_luaSession, entity1->luaProxy);
	Lunar<CEntityProxy>::push(_luaSession, entity2->luaProxy);
	res = lua_pcall (_luaSession,2,0,0);
	/*
	if(res<0)
		nlwarning("error calling lua function");
	*/

}

void CLuaEngine::entitySceneCollideEvent(CEntity *entity, CModule *module)
{
	if(!_luaSession)
		return;
	int res ;
//	nlinfo("CLuaEngine::entitySceneCollideEvent(0x%p[0x%p],0x%p[0x%p](%s)",entity,entity->luaProxy,module,module->luaProxy,module->name().c_str());
	lua_getglobal(_luaSession, "entitySceneCollideEvent");
	Lunar<CEntityProxy>::push(_luaSession, entity->luaProxy);
	Lunar<CModuleProxy>::push(_luaSession, module->luaProxy);
	res = lua_pcall (_luaSession,2,0,0);
	/*
	if(res<0)
		nlwarning("error calling lua function");
	*/
}

void CLuaEngine::entityWaterCollideEvent(CEntity *entity)
{
	if(!_luaSession)
		return;
	int res ;
//	nlinfo("CLuaEngine::entityWaterCollideEvent(0x%p)",entity);
	lua_getglobal(_luaSession, "entityWaterCollideEvent");
	Lunar<CEntityProxy>::push(_luaSession, entity->luaProxy);
	res = lua_pcall (_luaSession,1,0,0);
	/*
	if(res<0)
		nlwarning("error calling lua function");
	*/

}

