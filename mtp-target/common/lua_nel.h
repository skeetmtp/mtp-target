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

#ifndef LUA_NEL
#define LUA_NEL


//
// Includes
//

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

#include <nel/misc/vector.h>

#include "luna.h"

//
// Classes
//

class CLuaVector : public NLMISC::CVector
{
public:
	CLuaVector() { }
	
	CLuaVector(lua_State *L)
	{
		x = (float)luaL_checknumber(L, 1);
		y = (float)luaL_checknumber(L, 2);
		z = (float)luaL_checknumber(L, 3);
	}
	
	static const char className[];
	static Luna<CLuaVector>::RegType methods[];
};


class CLuaAngleAxis : public NLMISC::CAngleAxis
{
public:
	CLuaAngleAxis() { }
	
	CLuaAngleAxis(lua_State *L)
	{
		Axis.x = (float)luaL_checknumber(L, 1);
		Axis.y = (float)luaL_checknumber(L, 2);
		Axis.z = (float)luaL_checknumber(L, 3);
		Angle = (float)luaL_checknumber(L, 4);
	}
	
	static const char className[];
	static Luna<CLuaAngleAxis>::RegType methods[];
};



#endif
