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

#ifndef _LEVEL_LUA_PROXY_H_
#define _LEVEL_LUA_PROXY_H_

#include "level.h"

#include "../../common/lunar.h"

class CLevelProxy
{
	
public:
	CLevelProxy(lua_State *luaSession, CLevel *module)
	{
		_level = module;
		LuaUserData = 0;
		LuaUserDataRef = 0;
	}
	
	CLevelProxy(lua_State *luaSession)
	{
		_level = NULL;
		LuaUserData = 0;
		LuaUserDataRef = 0;
	}
	
	~CLevelProxy()
	{
	}
	
	bool call(std::string funcName);
	
	int setMetatable(lua_State *L);
	int getUserData(lua_State *luaSession);
	int setUserData(lua_State *luaSession);
	
	int getName(lua_State *luaSession);
	
	static const char className[];	
	static Lunar<CLevelProxy>::RegType methods[];	
private:
	void				*LuaUserData;
	uint32				 LuaUserDataRef;
	CLevel *_level;
};



#endif
