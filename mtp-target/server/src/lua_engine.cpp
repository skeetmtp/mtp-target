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
#include "module.h"
#include "entity.h"
#include "lua_engine.h"
#include "level_manager.h"


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

uint32 CLuaEngine::_sessionId = 0;

CLuaEngine::CLuaEngine()
{
	_luaSession = NULL;
	_error = true;
}

lua_State *CLuaEngine::session()
{
	if(_error)
		return NULL;
	return _luaSession;
}


void CLuaEngine::init(const std::string &filename)
{
	if(_luaSession && !_error)
		luaClose(_luaSession);

	_error = false;
	
	_luaSession = luaOpen();
	if(!_luaSession)
	{
		nlwarning("LEVEL: luaOpenAndLoad() failed while trying to load level '%s'", filename.c_str());
		return;
	}

	lua_register(session(), "_ALERT", lua_ALERT);
	
	_sessionId++;
	_sessionId = _sessionId % 1024;
	
	
	if(session())
	{
		lua_register(session(), "getSessionId", getSessionId);
		lua_register(session(), "getLevelSessionCount", getLevelSessionCount);
		lua_register(session(), "setMaxLevelSessionCount", setMaxLevelSessionCount);
	}
	
	Lunar<CModuleProxy>::Register(session());
	Lunar<CEntityProxy>::Register(session());

	string path = CPath::lookup("helpers.lua", false, false);
	luaLoad(session(),path);
	luaLoad(session(),filename);
	
}

void CLuaEngine::update()
{
	
}

void CLuaEngine::error()
{
	_error = true;
}

void CLuaEngine::release()
{
	if(_luaSession && !_error)
		luaClose(_luaSession);
	_luaSession = NULL;
}

void CLuaEngine::entityEntityCollideEvent(CEntity *entity1, CEntity *entity2)
{
	if(!session())
		return;
	int res ;
//	nlinfo("CLuaEngine::entityEntityCollideEvent(0x%p[0x%p],0x%p[0x%p])",entity1,entity1->luaProxy,entity2,entity2->luaProxy);
	lua_getglobal(session(), "entityEntityCollideEvent");
	Lunar<CEntityProxy>::push(session(), entity1->luaProxy);
	Lunar<CEntityProxy>::push(session(), entity2->luaProxy);
	res = lua_pcall (session(),2,0,0);
	/*
	if(res<0)
		nlwarning("error calling lua function");
	*/

	CLuaEngine::instance();
}

void CLuaEngine::entitySceneCollideEvent(CEntity *entity, CModule *module)
{
	if(!session())
		return;
	int res ;
//	nlinfo("CLuaEngine::entitySceneCollideEvent(0x%p[0x%p],0x%p[0x%p](%s)",entity,entity->luaProxy,module,module->luaProxy,module->name().c_str());
	lua_getglobal(session(), "entitySceneCollideEvent");
	Lunar<CEntityProxy>::push(session(), entity->luaProxy);
	Lunar<CModuleProxy>::push(session(), module->luaProxy);
	res = lua_pcall (session(),2,0,0);
	/*
	if(res<0)
		nlwarning("error calling lua function");
	*/
}

void CLuaEngine::entityWaterCollideEvent(CEntity *entity)
{
	if(!session())
		return;
	int res ;
//	nlinfo("CLuaEngine::entityWaterCollideEvent(0x%p)",entity);
	lua_getglobal(session(), "entityWaterCollideEvent");
	Lunar<CEntityProxy>::push(session(), entity->luaProxy);
	res = lua_pcall (session(),1,0,0);
	/*
	if(res<0)
		nlwarning("error calling lua function");
	*/

}

int CLuaEngine::getSessionId(lua_State *L)
{
	lua_pushnumber(L,_sessionId); 
	return 1;
}

int CLuaEngine::getLevelSessionCount(lua_State *L)
{
	lua_Number levelSessionCount = CLevelManager::instance().levelSessionCount();
	lua_pushnumber(L,levelSessionCount); 
	return 1;
}

int CLuaEngine::setMaxLevelSessionCount(lua_State *L)
{
	lua_Number maxLevelSessionCount = luaL_checknumber(L,1);
	CLevelManager::instance().maxLevelSessionCount((uint32)maxLevelSessionCount);
	return 1;
}

int CLuaEngine::lua_ALERT(lua_State *L)
{
	const char *msg = lua_tostring(L,1);
	char ch[1024];
	size_t i=0;
	while(msg[i]!='\0' && i<strlen(msg))
	{
		size_t j=0;
		while(msg[i]!='\0' && msg[i]!='\n' && i<strlen(msg))
			ch[j++]=msg[i++];
		if(msg[i]=='\n')
			i++;
		ch[j++]='\0';
		nlwarning("LUA_ALERT: %s",ch);
	}
	lua_pop(L,1);
	CLuaEngine::instance().error();
	return 0;
}


