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
#include "module_lua_proxy.h"
#include "lua_engine.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;


//
//
//

const char CModuleProxy::className[] = "Module";

Lunar<CModuleProxy>::RegType CModuleProxy::methods[] = 
{
	bind_method(CModuleProxy, setMetatable),	
		bind_method(CModuleProxy, getUserData),	
		bind_method(CModuleProxy, setUserData),	
		bind_method(CModuleProxy, setBounce),	
		bind_method(CModuleProxy, setScore),	
		bind_method(CModuleProxy, getScore),	
		bind_method(CModuleProxy, setAccel),	
		bind_method(CModuleProxy, setFriction),	
		bind_method(CModuleProxy, getName),	
		bind_method(CModuleProxy, setShapeName),	
	{0,0}
};


bool CModuleProxy::call(string funcName)
{ 
//	nlinfo("CModuleProxy::call(%s)",funcName.c_str());
	int res;
	lua_State *L = CLuaEngine::instance().session();
	if(L==NULL)
		return false;
	int mp = Lunar<CModuleProxy>::push(L, this);
	res = Lunar<CModuleProxy>::call(L,funcName.c_str(),0);
	/*
	if(res<0)
		nlwarning("error calling lua function : %s",funcName.c_str());
	*/

	lua_pop(L,-1);
	return res>=0;
}


int CModuleProxy::setMetatable(lua_State *L)
{ 
	int res = lua_setmetatable(L, -2);
	if(res==0)
		nlwarning("cannot set metatable");
	return 0; 
}

int CModuleProxy::setBounce(lua_State *L)
{ 
	lua_Number b = luaL_checknumber(L,1);
	_module->bounce(b==1); 
	return 0; 
}

int CModuleProxy::setScore(lua_State *L)
{ 
	lua_Number score = luaL_checknumber(L,1);
	_module->score((uint32)score); 
	return 0; 
}

int CModuleProxy::getScore(lua_State *L)
{ 
	lua_Number score = _module->score();
	lua_pushnumber(L,score); 
	return 1; 
}

int CModuleProxy::setAccel(lua_State *L)
{ 
	lua_Number accel = luaL_checknumber(L,1);
	_module->accel((float)accel); 
	return 0; 
}

int CModuleProxy::setFriction(lua_State *L)
{ 
	lua_Number friction = luaL_checknumber(L,1);
	_module->friction((float)friction); 
	return 0; 
}

int CModuleProxy::getName(lua_State *luaSession)
{
	lua_pushstring(luaSession, _module->name().c_str());
	return 1;
}

int CModuleProxy::setShapeName(lua_State *luaSession)
{
	unsigned int name_len;
	const char *name = luaL_checklstring(luaSession, 1, &name_len);
	_module->shapeName(name);
	return 0;
}

int CModuleProxy::getUserData(lua_State *luaSession)
{
	lua_getref(luaSession, LuaUserDataRef); //push obj which have this ref id
	/*
	if(!LuaUserDataRef)
		nlwarning("clien lua call getuserdata but userdata is NULL");
	*/
	return 1; // one return value
}

int CModuleProxy::setUserData(lua_State *luaSession)
{
	LuaUserData = lua_touserdata(luaSession, 1); // get arg
	LuaUserDataRef = lua_ref(luaSession,1); //get ref id and lock it
	return 0; // no return value
}
