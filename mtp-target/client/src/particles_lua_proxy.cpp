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
#include "level_manager.h"
#include "particles_lua_proxy.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;


//
//
//

const char CParticlesProxy::className[] = "Particles";

Lunar<CParticlesProxy>::RegType CParticlesProxy::methods[] = 
{
	bind_method(CParticlesProxy, setMetatable),	
		bind_method(CParticlesProxy, getUserData),	
		bind_method(CParticlesProxy, setUserData),	
		bind_method(CParticlesProxy, getName),	
	{0,0}
};


bool CParticlesProxy::call(string funcName)
{ 
//	nlinfo("CParticlesProxy::call(%s)",funcName.c_str());
	int res;
	if(!CLevelManager::instance().levelPresent())
		return false;
	lua_State *L = CLevelManager::instance().currentLevel().luaState();
	if(L==0)
		return false;
	int mp = Lunar<CParticlesProxy>::push(L, this);
	res = Lunar<CParticlesProxy>::call(L,funcName.c_str(),0);
	/*
	if(res<0)
		nlwarning("error calling lua function : %s",funcName.c_str());
	*/

	lua_pop(L,-1);
	return res>=0;
}


int CParticlesProxy::setMetatable(lua_State *L)
{ 
	int res = lua_setmetatable(L, -2);
	if(res==0)
		nlwarning("cannot set metatable");
	return 0; 
}


int CParticlesProxy::getUserData(lua_State *luaSession)
{
	lua_getref(luaSession, LuaUserDataRef); //push obj which have this ref id
	/*
	if(!LuaUserDataRef)
		nlwarning("clien lua call getuserdata but userdata is 0");
	*/
	return 1; // one return value
}

int CParticlesProxy::setUserData(lua_State *luaSession)
{
	LuaUserData = lua_touserdata(luaSession, 1); // get arg
	LuaUserDataRef = lua_ref(luaSession,1); //get ref id and lock it
	return 0; // no return value
}

int CParticlesProxy::getName(lua_State *luaSession)
{
	lua_pushstring(luaSession,_particles->name().c_str());
	return 1;
}

