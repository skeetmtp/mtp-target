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
	lua_getglobal(_luaSession, "entityWaterCollideEvent");
	Lunar<CEntityProxy>::push(_luaSession, entity->luaProxy);
	res = lua_pcall (_luaSession,1,0,0);
	/*
	if(res<0)
		nlwarning("error calling lua function");
	*/

}


	/*
	int res ;
	
	//CEntityProxy *entityProxy = new CEntityProxy(_luaSession,entity);
	int mp = Lunar<CEntityProxy>::push(_luaSession, entity->luaProxy);
	res = Lunar<CEntityProxy>::call(_luaSession,"init",0);
	
	if(res<0)
		nlwarning("error calling lua function");
		*/

		/*	
	lua_getglobal(_luaSession, "entityTouch");
	Lunar<CModuleProxy>::push(_luaSession, module->luaProxy);
	res = lua_pcall (_luaSession,1,0,0);
	*/
  
	//module->luaProxy->call("show");
	/*
	int mp = Lunar<CModuleProxy>::push(_luaSession, module->luaProxy);
	res = Lunar<CModuleProxy>::call(_luaSession,"show",0);
	if(res<0)
		nlwarning("error calling lua function");
	*/

	
/*
	int sum = (int)lua_tonumber(_luaSession, -1);
	lua_pop(_luaSession, 1);
	nlinfo("sum = %d",sum);
*/
	

#if 0

//////////////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////////////
class CEntityProxy
{
public:
	CEntityProxy(lua_State *luaSession, CEntity *entity)
	{
		Entity = entity;
	}
	
	~CEntityProxy()
	{
	}
	
	const int getName(lua_State *luaSession)
	{
		lua_pushstring(luaSession, Entity->name().c_str());
		return 1;
	}
	
	const int getNbOpenClose(lua_State *luaSession)
	{
		lua_pushnumber(luaSession, Entity->NbOpenClose);
		return 1;
	}
	
	const int getUserData(lua_State *luaSession)
	{
		if(Entity->LuaUserDataRef)
			lua_getref(luaSession, Entity->LuaUserDataRef); //push obj which have this ref id
		return 1; // one return value
	}
	
	const int setUserData(lua_State *luaSession)
	{
		Entity->LuaUserData = lua_touserdata(luaSession, 1); // get arg
		Entity->LuaUserDataRef = lua_ref(luaSession,1); //get ref id and lock it
		return 0; // no return value
	}
	
	
private:
	CEntity *Entity;
};


const char *CLuaProxy<CEntityProxy>::ClassName = "Entity";
const CLuaProxy<CEntityProxy>::RegType CLuaProxy<CEntityProxy>::FunctionList[] =
{
	{ "getUserData"       , &CEntityProxy::getUserData },
	{ "setUserData"       , &CEntityProxy::setUserData },
	{ "getName"           , &CEntityProxy::getName },
	{ "getNbOpenClose"    , &CEntityProxy::getNbOpenClose },
	{ 0 },
};



//////////////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////////////
class CModuleProxy
{
public:
	CModuleProxy(lua_State *luaSession,CModule *module)
	{
		_module = module;
	}
	
	~CModuleProxy()
	{
	}
	
	const int getUserData(lua_State *luaSession)
	{
		lua_getref(luaSession, _module->LuaUserDataRef); //push obj which have this ref id
		return 1; // one return value
	}
	
	const int setUserData(lua_State *luaSession)
	{
		_module->LuaUserData = lua_touserdata(luaSession, 1); // get arg
		_module->LuaUserDataRef = lua_ref(luaSession,1); //get ref id and lock it
		return 0; // no return value
	}
	
	
private:
	CModule *_module;
};


const char *CLuaProxy<CModuleProxy>::ClassName = "Module";
const CLuaProxy<CModuleProxy>::RegType CLuaProxy<CModuleProxy>::FunctionList[] =
{
	{ "getUserData", &CModuleProxy::getUserData },
	{ "setUserData", &CModuleProxy::setUserData },
	{ 0 },
};



//////////////////////////////////////////////////////////////////////////////////////
//
// Misc function , some one override default lua
//
//////////////////////////////////////////////////////////////////////////////////////

static int lua_nlinfo(lua_State *luaSession)
{
	const char *msg = lua_tostring(luaSession,1);
	nlinfo("LUA : %s",msg);
	lua_pop(luaSession,1);
	return 0;
}

static int lua_nlwarning(lua_State *luaSession)
{
	const char *msg = lua_tostring(luaSession,1);
	nlwarning("LUA : %s",msg);
	lua_pop(luaSession,1);
	return 0;
}

static int lua_ALERT(lua_State *luaSession)
{
	const char *msg = lua_tostring(luaSession,1);
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
		nlwarning("LUA ALERT : %s",ch);
	}
	lua_pop(luaSession,1);
	return 0;
}

static int lua_exit(lua_State *luaSession)
{
	const int exitCode = (int) lua_tonumber(luaSession,-1);
	lua_pop(luaSession,1);
	nlinfo("LUA : exit(%d)",exitCode);
	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////
//
// General function to access global variable
//
//////////////////////////////////////////////////////////////////////////////////////

static int lua_mtChat(lua_State *luaSession)
{
	const char *msg = lua_tostring(luaSession,-1);
	CNetwork::instance().sendChat(msg);
	lua_pop(luaSession,1);
	return 0;
}




//////////////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////////////

lua_State* CLuaEngine::LuaSession = 0;

void CLuaEngine::start()
{
	luaCodeIsValid = false;
	if(NLNET::IService::getInstance()->ConfigFile.getVar("UseLua").asInt()==0)
		return;
	LuaSession = lua_open();
	nlassert(LuaSession != 0);
	
	lua_baselibopen(LuaSession);
	lua_iolibopen(LuaSession);
	lua_strlibopen(LuaSession);
	lua_mathlibopen(LuaSession);
	lua_dblibopen(LuaSession);

	lua_register(LuaSession, "nlinfo", lua_nlinfo);
	lua_register(LuaSession, "print", lua_nlinfo);
	lua_register(LuaSession, "nlwarning", lua_nlwarning);
	lua_register(LuaSession, "warn", lua_nlwarning);
	lua_register(LuaSession, "error", lua_nlwarning);
	lua_register(LuaSession, "_ALERT", lua_ALERT);
	lua_register(LuaSession, "exit", lua_exit);

	lua_register(LuaSession, "mtChat", lua_mtChat);

	CLuaProxy<CEntityProxy>::reg(LuaSession);
	CLuaProxy<CModuleProxy>::reg(LuaSession);
}


void CLuaEngine::stop()
{
	if(LuaSession)
	{
		CLuaProxy<CEntityProxy>::unreg(LuaSession);
		CLuaProxy<CModuleProxy>::unreg(LuaSession);
		lua_close(LuaSession);	
	}
	luaCodeIsValid = false;
	LuaSession = 0;
}

void CLuaEngine::loadLevelCode(string filename)
{
	if(NLNET::IService::getInstance()->ConfigFile.getVar("UseLua").asInt()==0)
		return;
	int res = lua_dofile(LuaSession,filename.c_str());
	luaCodeIsValid = res<=0;
	if(!luaCodeIsValid)
		nlwarning("error(%d) loading lua script , skip lua stuf",res);
	
	LUA_CODE_CHECK;

	MT_LUA_GET_GLOBAL("mtLevelInit");
	MT_LUA_CALL(0,0);
}

void CLuaEngine::entityEntityCollideEvent(CEntity *entity1, CEntity *entity2)
{
	if(!entity1->LuaInit) return;
	if(!entity2->LuaInit) return;
	LUA_CODE_CHECK;
	
	MT_LUA_GET_GLOBAL("mtEntityCB");
	CEntityProxy lua_entity1(LuaSession,entity1);
	CLuaProxy<CEntityProxy>::initAndPush(LuaSession,&lua_entity1);
	CEntityProxy lua_entity2(LuaSession,entity2);
	CLuaProxy<CEntityProxy>::initAndPush(LuaSession,&lua_entity2);
	MT_LUA_CALL(2,0);
}

void CLuaEngine::entityWaterCollideEvent(CEntity *entity)
{
	if(!entity->LuaInit) return;
	LUA_CODE_CHECK;

	MT_LUA_GET_GLOBAL("mtWaterCB");
	CEntityProxy lua_entity(LuaSession,entity);
	CLuaProxy<CEntityProxy>::initAndPush(LuaSession,&lua_entity);
	MT_LUA_CALL(1,0);
}

void CLuaEngine::entitySceneCollideEvent(CEntity *entity, CModule *module, string luaFunctionName)
{
	if(!entity->LuaInit) return;
	LUA_CODE_CHECK;
	
	if(luaFunctionName.size()==0)
		return;

	MT_LUA_GET_GLOBAL(luaFunctionName.c_str());
	CEntityProxy lua_entity(LuaSession,entity);
	CLuaProxy<CEntityProxy>::initAndPush(LuaSession,&lua_entity);
	CModuleProxy lua_module(LuaSession,module);
	CLuaProxy<CModuleProxy>::initAndPush(LuaSession,&lua_module);
	MT_LUA_CALL(2,0);
}

void CLuaEngine::entityInit(CEntity *entity)
{
	LUA_CODE_CHECK;

	MT_LUA_GET_GLOBAL("mtEntityInit");
	CEntityProxy lua_entity(LuaSession,entity);
	CLuaProxy<CEntityProxy>::initAndPush(LuaSession,&lua_entity);
	MT_LUA_CALL(1,0);
	entity->LuaInit = true;
}

void CLuaEngine::collisionEntityInit(CModule *module)
{
	LUA_CODE_CHECK;
	
	MT_LUA_GET_GLOBAL("mtModuleInit");
	CModuleProxy lua_module(LuaSession,module);
	CLuaProxy<CModuleProxy>::initAndPush(LuaSession,&lua_module);
	MT_LUA_CALL(1,0);
}

void CLuaEngine::levelUpdate()
{
	LUA_CODE_CHECK;
	
	MT_LUA_GET_GLOBAL("mtLevelUpdate");
	MT_LUA_CALL(0,0);
}

void CLuaEngine::entityUpdate(CEntity *entity)
{
	if(!entity->LuaInit) return;
	LUA_CODE_CHECK;
	
	MT_LUA_GET_GLOBAL("mtEntityUpdate");
	CEntityProxy lua_entity(LuaSession,entity);
	CLuaProxy<CEntityProxy>::initAndPush(LuaSession,&lua_entity);
	MT_LUA_CALL(1,0);
}

void CLuaEngine::collisionEntityUpdate(CModule *module)
{
	LUA_CODE_CHECK;
	
	MT_LUA_GET_GLOBAL("mtModuleUpdate");
	CModuleProxy lua_module(LuaSession,module);
	CLuaProxy<CModuleProxy>::initAndPush(LuaSession,&lua_module);
	MT_LUA_CALL(1,0);
}

#endif