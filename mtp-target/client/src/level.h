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

#include "../../common/lua_nel.h"

#include "module.h"
#include "start_position.h"

//
// Classes
//

class CLevel
{
public:

	CLevel(const std::string &filename);
	~CLevel();

	void display(NLMISC::CLog *log = NLMISC::InfoLog) const;

	// return true of this level instance is valid (well initialized and ready to work)
	bool			valid() const { return Valid; }

	std::string		filename() const { return FileName; }
	std::string		name() const { return Name; }
	std::string		author() const { return Author; }
	
	bool			displayStartPositions() const { return DisplayStartPositions; }
	void			displayStartPositions(bool sp);
	bool			displayLevel() const { return DisplayLevel; }
	void			displayLevel(bool l);

	void			switchStartPositions() { displayStartPositions(!DisplayStartPositions); }
	void			switchLevel() { displayLevel(!DisplayLevel); }

	void			reset();
	bool			changed() {return _changed;}
	void			changed(bool c) {_changed = c;}
	
	NLMISC::CVector startPosition(uint32 id);

	CModule *getModule(uint32 id);
	uint32 getModuleCount();
	void updateModule(uint32 id,NLMISC::CVector pos,NLMISC::CVector rot,uint32 selectedBy);
	CStartPosition *getStartPosition(uint32 id);
	uint32 getStartPositionCount();
	void updateStartPosition(uint32 id,NLMISC::CVector pos,NLMISC::CVector rot,uint32 selectedBy);
	
private:
	
	lua_State						*LuaState;

	// Level name
	std::string						Name;
	std::string						Author;
	
	// Path + File name of this level
	std::string						FileName;

	bool							Valid;

//	std::vector<NL3D::UInstance *>	Meshes;
	std::vector<CModule *>			Modules;
//	std::vector<NL3D::UInstance *>	StartPositions;
	std::vector<CStartPosition *>	StartPositions;
	
	bool							DisplayStartPositions;
	bool							DisplayLevel;
	bool							_changed;

};

#endif
