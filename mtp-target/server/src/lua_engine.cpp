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
#include "session_manager.h"
#include "entity_manager.h"


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
	int status = 0; lua_pcall(LuaSession, argCount, retCount); \
	if (status) {\
		const char *msg = lua_tostring(session(), -1);\
		if (msg == 0) msg = "(error with no message)";\
		nlwarning("pcall error(MT_LUA_CALL) : (status = %d)%s", status, msg);\
		luaCodeIsValid = false; \
		return;\
	}\
}while(false)


//////////////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////////////

uint32 CLuaEngine::_sessionId = 0;

CLuaEngine::CLuaEngine()
{
	_luaSession = 0;
	_error = true;
}

lua_State *CLuaEngine::session()
{
	if(_error)
		return 0;
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
		lua_register(session(), "getEntityCount", getEntityCount);
		lua_register(session(), "getEntity", getEntity);
		lua_register(session(), "getEntityById", getEntityById);
		lua_register(session(), "getModuleCount", getModuleCount);
		lua_register(session(), "getModule", getModule);
		lua_register(session(), "sendChat", sendChat);
		lua_register(session(), "displayTextToAll", displayTextToAll);
		lua_register(session(), "setLevelHasBonusTime", setLevelHasBonusTime);
		lua_register(session(), "setLevelRecordBest", setLevelRecordBest);
		lua_register(session(), "setLevelTimeout", setLevelTimeout);
		lua_register(session(), "getLevelTimeout", getLevelTimeout);
		lua_register(session(), "getTimeRemaining", getTimeRemaining);
		lua_register(session(), "execLuaOnAllClient", execLuaOnAllClient);
		lua_register(session(), "execLuaOnOneClient", execLuaOnOneClient);
		lua_register(session(), "execLuaOnAllButOneClient", execLuaOnAllButOneClient);
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
	_luaSession = 0;
}

void CLuaEngine::entityEntityCollideEvent(CEntity *entity1, CEntity *entity2)
{
	if(!session())
		return;
//	nlinfo("CLuaEngine::entityEntityCollideEvent(0x%p[0x%p],0x%p[0x%p])",entity1,entity1->luaProxy,entity2,entity2->luaProxy);
	lua_getglobal(session(), "entityEntityCollideEvent");
	Lunar<CEntityProxy>::push(session(), entity1->luaProxy);
	Lunar<CEntityProxy>::push(session(), entity2->luaProxy);
	int status = lua_pcall (session(),2,0,0);
    if (status) {
		const char *msg = lua_tostring(session(), -1);
		if (msg == 0) msg = "(error with no message)";
		nlwarning("pcall error(entityEntityCollideEvent) : (status = %d)%s", status, msg);
		return;
    }
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
    if (res) {
		const char *msg = lua_tostring(session(), -1);
		if (msg == 0) msg = "(error with no message)";
		nlwarning("pcall error(entitySceneCollideEvent) : (status = %d)%s", res, msg);
		return;
    }
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
    if (res) {
		const char *msg = lua_tostring(session(), -1);
		if (msg == 0) msg = "(error with no message)";
		nlwarning("pcall error(entityWaterCollideEvent) : (status = %d)%s", res, msg);
		return;
    }
	/*
	if(res<0)
		nlwarning("error calling lua function");
	*/

}

void CLuaEngine::entityLeaveEvent(CEntity *entity)
{
	if(!session())
		return;
	int res ;
	//	nlinfo("CLuaEngine::entityWaterCollideEvent(0x%p)",entity);
	lua_getglobal(session(), "entityLeaveEvent");
	Lunar<CEntityProxy>::push(session(), entity->luaProxy);
	res = lua_pcall (session(),1,0,0);	
    if (res) {
		const char *msg = lua_tostring(session(), -1);
		if (msg == 0) msg = "(error with no message)";
		nlwarning("pcall error(entityLeaveEvent) : (status = %d)%s", res, msg);
		return;
    }
}


void CLuaEngine::levelInit()
{
	if(!session())
		return;
	int res ;
//	nlinfo("CLuaEngine::entityWaterCollideEvent(0x%p)",entity);
	lua_getglobal(session(), "levelInit");
	res = lua_pcall (session(),0,0,0);
    if (res) {
		const char *msg = lua_tostring(session(), -1);
		if (msg == 0) msg = "(error with no message)";
		nlwarning("pcall error(levelInit) : (status = %d)%s", res, msg);
		return;
    }
	/*
	if(res<0)
		nlwarning("error calling lua function");
	*/

}


void CLuaEngine::levelPreUpdate()
{
	if(!session())
		return;
	int res ;
//	nlinfo("CLuaEngine::entityWaterCollideEvent(0x%p)",entity);
	lua_getglobal(session(), "levelPreUpdate");
	res = lua_pcall (session(),0,0,0);
    if (res) {
		const char *msg = lua_tostring(session(), -1);
		if (msg == 0) msg = "(error with no message)";
		nlwarning("pcall error(levelPreUpdate) : (status = %d)%s", res, msg);
		return;
    }
	/*
	if(res<0)
		nlwarning("error calling lua function");
	*/

}

void CLuaEngine::levelPostUpdate()
{
	if(!session())
		return;
	int res ;
//	nlinfo("CLuaEngine::entityWaterCollideEvent(0x%p)",entity);
	lua_getglobal(session(), "levelPostUpdate");
	res = lua_pcall (session(),0,0,0);
    if (res) {
		const char *msg = lua_tostring(session(), -1);
		if (msg == 0) msg = "(error with no message)";
		nlwarning("pcall error(levelPostUpdate) : (status = %d)%s", res, msg);
		return;
    }
	/*
	if(res<0)
		nlwarning("error calling lua function");
	*/

}

void CLuaEngine::levelEndSession()
{
	if(!session())
		return;
	int res ;
//	nlinfo("CLuaEngine::entityWaterCollideEvent(0x%p)",entity);
	lua_getglobal(session(), "levelEndSession");
	res = lua_pcall (session(),0,0,0);
    if (res) {
		const char *msg = lua_tostring(session(), -1);
		if (msg == 0) msg = "(error with no message)";
		nlwarning("pcall error(levelEndSession) : (status = %d)%s", res, msg);
		return;
    }
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
	return 0;
}


int CLuaEngine::getEntityCount(lua_State *L)
{
	lua_pushnumber(L,CEntityManager::instance().nbEntities()); 
	return 1;
}

int CLuaEngine::getEntity(lua_State *L)
{
	lua_Number entityNumber = luaL_checknumber(L,1);
	CEntity *e = CEntityManager::instance().getNthEntity((uint8)entityNumber);
	if(e==0)
		nlwarning("getNthEntity(%d)==0",entityNumber);
	else
		Lunar<CEntityProxy>::push(L, e->luaProxy);
	return 1;
}

int CLuaEngine::getEntityById(lua_State *L)
{
	uint eid = (uint )luaL_checknumber(L,1);
	CEntity *e = CEntityManager::instance().getById(eid);
	if(e==0)
		nlwarning("getEntityById(%d)==0",eid);
	else
		Lunar<CEntityProxy>::push(L, e->luaProxy);
	return 1;
}

int CLuaEngine::getModuleCount(lua_State *L)
{
	lua_Number res = 0;
	if(CLevelManager::instance().haveCurrentLevel())
		res = CLevelManager::instance().currentLevel().getModuleCount();
	lua_pushnumber(L,res); 
	return 1;
}

int CLuaEngine::getModule(lua_State *L)
{
	uint moduleNumber = (uint )luaL_checknumber(L,1);
	CModule *m = 0;
	if(CLevelManager::instance().haveCurrentLevel())
		m = CLevelManager::instance().currentLevel().getModule(moduleNumber);
	
	if(m==0)
	{
		nlwarning("getModule(%d)==0",moduleNumber);
		return 0;
	}
	else
		Lunar<CModuleProxy>::push(L, m->luaProxy);
	return 1;
}

int CLuaEngine::sendChat(lua_State *L)
{
	unsigned int len;
	const char *text = luaL_checklstring(L, 1, &len);
	string chatMsg(text);
	CNetwork::instance().sendChat(chatMsg);
	return 0;
}	

int CLuaEngine::displayTextToAll(lua_State *L)
{
	float x = (float)luaL_checknumber(L,1);
	float y = (float)luaL_checknumber(L,2);
	float scale = (float)luaL_checknumber(L,3);
	
	uint8 r = (uint8 )luaL_checknumber(L,4);
	uint8 g = (uint8 )luaL_checknumber(L,5);
	uint8 b = (uint8 )luaL_checknumber(L,6);
	unsigned int len;
	const char *text = luaL_checklstring(L, 7, &len);
	
	CRGBA col(r,g,b,255);
	double duration = luaL_checknumber(L,8);
	
	string message(text);
	CEntityManager::instance().displayText(x,y,scale,col,duration,message);
	
	return 0;
}	

int CLuaEngine::setLevelHasBonusTime(lua_State *L)
{
	bool hasBonuseTime = luaL_checknumber(L,1) == 1;
	CLevelManager::instance().bonusTime(hasBonuseTime);
	return 0;
}

int CLuaEngine::setLevelRecordBest(lua_State *L)
{
	bool hasRecord = luaL_checknumber(L,1) == 1;
	CLevelManager::instance().recordBest(hasRecord);
	return 0;
}

int CLuaEngine::setLevelTimeout(lua_State *L)
{
	double timeout = luaL_checknumber(L,1);
	CLevelManager::instance().timeTimeout(timeout*1000);
	return 0;	
}

int CLuaEngine::getLevelTimeout(lua_State *L)
{
	lua_Number res = CLevelManager::instance().timeTimeout() / 1000;
	lua_pushnumber(L,res); 
	return 1;
}

int CLuaEngine::getTimeRemaining(lua_State *L)
{
	TTime startTime = CSessionManager::instance().startTime();
	TTime localtime = CTime::getLocalTime();
	TTime timeRemaining = startTime+(TTime)CLevelManager::instance().timeTimeout() - localtime;
	if(CSessionManager::instance().currentStateName()!="Running")
		timeRemaining = (TTime)CLevelManager::instance().timeTimeout();
	lua_Number res = ((lua_Number)(timeRemaining)) / 1000;
	if(res<0)
		res = 0;
	lua_pushnumber(L,res); 
	return 1;
}

int CLuaEngine::execLuaOnAllClient(lua_State *L)
{
	unsigned int len;
	const char *text = luaL_checklstring(L, 1, &len);
	string code(text);
	if(CNetwork::instance().version()>=2)
	{
		//nlwarning("lua : execLuaOnAllClient : %s",code.c_str());
		CNetMessage msgout(CNetMessage::ExecLua);
		msgout.serial(code);
		CNetwork::instance().send(msgout);
	}
	
	return 0;		
}

int CLuaEngine::execLuaOnOneClient(lua_State *L)
{
	uint8 eid = (uint8 )luaL_checknumber(L,1);
	unsigned int len;
	const char *text = luaL_checklstring(L, 2, &len);
	string code(text);
	CEntity *e = CEntityManager::instance().getById(eid);
	if(!e || e->type() != CEntity::Client)
		return 0;

	if(CNetwork::instance().version()>=2)
	{
		//nlwarning("lua : execLuaOnAllClient : %s",code.c_str());
		CNetMessage msgout(CNetMessage::ExecLua);
		msgout.serial(code);
		CNetwork::instance().send(eid,msgout);
	}
	
	return 0;		
}

int CLuaEngine::execLuaOnAllButOneClient(lua_State *L)
{
	uint8 eid = (uint8 )luaL_checknumber(L,1);
	unsigned int len;
	const char *text = luaL_checklstring(L, 2, &len);
	string code(text);
	if(CNetwork::instance().version()>=2)
	{
		//nlwarning("lua : execLuaOnAllClient : %s",code.c_str());
		CNetMessage msgout(CNetMessage::ExecLua);
		msgout.serial(code);
		CNetwork::instance().sendAllExcept(eid,msgout);
	}
	
	return 0;		
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


