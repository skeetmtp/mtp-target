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

#ifndef MTPT_MODULE
#define MTPT_MODULE


//
// Includes
//

#include <vector>
#include <ode/ode.h>

#include "../../common/lua_utility.h"
#include "../../common/lua_nel.h"
#include "../../common/lunar.h"

#include "editable_element.h"

class CModuleProxy;

//
// Classes
//

class CModule : public CEditableElement
{
public:

	CModule();
	CModule(const std::string &name, const NLMISC::CVector &position, const NLMISC::CAngleAxis &rotation, uint8 id);
	
	virtual ~CModule();

	void display(NLMISC::CLog *log = NLMISC::InfoLog) const;
	void update();
	void initBeforeStartLevel();
		
	// accessors
	
	bool			bounce() const { return Bounce; }
	void			bounce(bool b);
	
	sint32			score() const { return Score; }
	void			score(sint32 s) { Score = s; }

	float			accel() const { return Accel; }
	void			accel(float a) { Accel = a; }

	float			friction() const { return Friction; }
	void			friction(float f) { Friction = f; }

	std::string		name() const { return Name; }

	virtual void update(NLMISC::CVector pos,NLMISC::CVector rot);
	
	void shapeName(const std::string n) { ShapeName = n; }
	
	virtual std::string &toLuaString();


	uint8 id() {return _id;}

	std::string	 LuaFunctionName;

	CModuleProxy		*luaProxy;
private:

	void init();
	void _luaInit();
	
	dGeomID				Geom;
	std::vector<dReal>	Vertices;	// 3 entries for one vertex (x,y,z)
	std::vector<int>	Indices;	

	std::string			Name;
	std::string			ShapeName;
	bool				Bounce;
	sint32				Score;
	float				Accel;
	float				Friction;

	lua_State			*LuaState;
	uint8 _id;
	
};






#endif
