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
#include "entity_lua_proxy.h"
#include "lua_engine.h"
#include "network.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;


//
//
//

const char CEntityProxy::className[] = "Entity";

Lunar<CEntityProxy>::RegType CEntityProxy::methods[] = 
{
	bind_method(CEntityProxy, setMetatable),	
		bind_method(CEntityProxy, getUserData),	
		bind_method(CEntityProxy, setUserData),	
		bind_method(CEntityProxy, getName),	
		bind_method(CEntityProxy, getIsOpen),	
		bind_method(CEntityProxy, setIsOpen),	
		bind_method(CEntityProxy, setCurrentScore),	
		bind_method(CEntityProxy, displayText),	
	{0,0}
};


bool CEntityProxy::call(string funcName)
{ 
	int res;
	lua_State *L = CLuaEngine::instance().session();
	if(L==NULL)
		return false;
	int mp = Lunar<CEntityProxy>::push(L, this);
	res = Lunar<CEntityProxy>::call(L,funcName.c_str(),0);
	/*
	if(res<0)
		nlwarning("error calling lua function : %s",funcName.c_str());
	*/
	return res>=0;
}

int CEntityProxy::setMetatable(lua_State *L)
{ 
    int metatable = lua_gettop(L);
	int res = lua_setmetatable(L, -2);
	if(res==0)
		nlwarning("cannot set metatable");
	return 0; 
}

int CEntityProxy::getUserData(lua_State *luaSession)
{
	lua_getref(luaSession, LuaUserDataRef); //push obj which have this ref id
	/*
	if(!LuaUserDataRef)
		nlwarning("clien lua call getuserdata but userdata is NULL");
	*/
	return 1; // one return value
}

int CEntityProxy::setUserData(lua_State *luaSession)
{
	LuaUserData = lua_touserdata(luaSession, 1); // get arg
	LuaUserDataRef = lua_ref(luaSession,1); //get ref id and lock it
	return 0; // no return value
}

int CEntityProxy::getIsOpen(lua_State *luaSession)
{
	lua_Number isOpen = _entity->isOpen() ? 1 : 0;
	lua_pushnumber(luaSession, isOpen); 
	return 1;
}

int CEntityProxy::setIsOpen(lua_State *luaSession)
{
	lua_Number isOpen = luaL_checknumber(luaSession,1);
	_entity->isOpen(isOpen==1);
	return 0; // no return value
}

int CEntityProxy::getName(lua_State *luaSession)
{
	lua_pushstring(luaSession, _entity->name().c_str());
	return 1;
}

int CEntityProxy::setCurrentScore(lua_State *luaSession)
{
	lua_Number score = luaL_checknumber(luaSession,1);
	_entity->CurrentScore = ((uint32)score); 
	//nlinfo(">> %s set score = %d",_entity->name().c_str(),_entity->CurrentScore);
	return 0;
}

int CEntityProxy::displayText(lua_State *luaSession)
{
	float x = (float)luaL_checknumber(luaSession,1);
	float y = (float)luaL_checknumber(luaSession,2);
	float s = (float)luaL_checknumber(luaSession,3);
	

	uint8 r = (uint8 )luaL_checknumber(luaSession,4);
	uint8 g = (uint8 )luaL_checknumber(luaSession,5);
	uint8 b = (uint8 )luaL_checknumber(luaSession,6);
	unsigned int len;
	const char *text = luaL_checklstring(luaSession, 7, &len);

	CRGBA col(r,g,b,255);
	double duration = luaL_checknumber(luaSession,8);

	if(_entity->type() == CEntity::Client)
	{
		string message = text;
		
		CNetMessage msgout(CNetMessage::DisplayText);
		msgout.serial(x);
		msgout.serial(y);
		msgout.serial(s);
		msgout.serial(message);
		msgout.serial(col);
		msgout.serial(duration);
		CNetwork::instance().send(_entity->id(),msgout);
		nlinfo("display message : %s to %s",message.c_str(),_entity->name().c_str());
	}

	return 0;
}

