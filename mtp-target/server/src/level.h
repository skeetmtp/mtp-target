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

#ifndef MTPT_LEVEL
#define MTPT_LEVEL


//
// Includes
//

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

#include "module.h"
#include "../../common/lua_nel.h"


//
// Classes
//
class CLevelProxy;

class CLevel
{
public:

	CLevel(const std::string &filename);
	~CLevel();

	void display(NLMISC::CLog *log = NLMISC::InfoLog) const;
	void update();
	
	void nextStartingPoint(NLMISC::CVector &pos, uint8 &id);

	// return true of this level instance is valid (well initialized and ready to work)
	bool valid() const { return Valid; }

	std::string name() const { return Name; }
	std::string fileName() const { return FileName; }

	CModule *getModule(uint32 id);
	uint8 getModuleCount();
	void updateModule(uint32 id,NLMISC::CVector pos,NLMISC::CVector rot);

	void save();
	bool changed();
	
	CLevelProxy		*luaProxy;
private:

	void _luaInit();
		

	std::vector<CModule *>		Modules;

	static uint8				NextStartingPoint;

	std::vector<CLuaVector>		StartPoints;
	std::vector<CLuaVector>		Cameras;

	// Level name
	std::string					Name;

	// Path + File name of this level
	std::string					FileName;

	bool						Valid;
};

#endif
