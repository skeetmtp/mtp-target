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

#include "bot.h"
#include "main.h"
#include "level.h"
#include "client.h"
#include "network.h"
#include "physics.h"
#include "variables.h"
#include "lua_engine.h"
#include "level_manager.h"
#include "entity_manager.h"
#include "../../common/net_message.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;
using namespace NLNET;


//
// Types
//


//
// Declarations
//


//
// Variables
//

//
// Functions
//

CLevelManager::CLevelManager()
{
	NextLevelId = 0;
	CurrentLevel = 0;
	LevelSessionCount = 0;
	MaxLevelSessionCount = 0;
}

void CLevelManager::init()
{
}

void CLevelManager::update()
{
	if(CurrentLevel)
		CurrentLevel->update();		
}

void CLevelManager::release()
{
	if(CurrentLevel)
	{
		delete CurrentLevel;
		CurrentLevel = 0;
	}
	CLuaEngine::instance().release();
	CLevelManager::uninstance();
}

bool CLevelManager::newLevel()
{
	LevelSessionCount++;
	pausePhysics();
	CEntityManager::instance().reset();


	if(LevelSessionCount>MaxLevelSessionCount && CurrentLevel)
	{
		nlinfo("newLevel() : keeping level : '%s'",CurrentLevel->name().c_str());
		resumePhysics();	
		return true;	
	}

	uint i;
	nlinfo("Find a new level");

	vector<string> files;
	vector<string> levels;
	CPath::getPathContent("data/level", true, false, true, files);
	for(i=0; i<files.size(); i++)
	{
		string fn = files[i];
		uint32 p = files[i].find(".lua");
		uint32 pe = files[i].size()-4;
		if(p==pe)
			levels.push_back(files[i]);
	}

	nlassert(levels.size() > 0);


	if(CurrentLevel)
	{
		delete CurrentLevel;
		CurrentLevel = NULL;
	}
	for(i = 0; i < levels.size(); i++)
	{
		CLevel *newLevel = new CLevel(levels[(NextLevelId+i)%levels.size()]);
		if(newLevel->valid())
		{
			CurrentLevel = newLevel;
			nlinfo("'%s' level loaded",newLevel->name().c_str());
			resumePhysics();	
			return true;
		}
	}

	nlwarning("newLevel() : no valid level found");
	resumePhysics();	
	return false;
}

void CLevelManager::display(CLog *log) const
{
	if(CurrentLevel)
		CurrentLevel->display(log);
	else
		log->displayNL("There's not current level");
}

string CLevelManager::levelName()
{
	if(CurrentLevel)
		return CurrentLevel->name();
	else
		return "NoLevel";
}


uint32  CLevelManager::levelSessionCount()
{
	return LevelSessionCount;
}

void  CLevelManager::maxLevelSessionCount(uint32 levelCount)
{
	MaxLevelSessionCount = levelCount;	
	LevelSessionCount = 1;
}

//
// Commands
//

NLMISC_COMMAND(displayLevel, "display the current level", "")
{
	if(args.size() != 0) return false;
	CLevelManager::instance().display(&log);
	return true;
}

NLMISC_DYNVARIABLE(string, CurrentLevel, "")
{
	if(get)
	{
		*pointer = CLevelManager::instance().levelName();
	}
}

