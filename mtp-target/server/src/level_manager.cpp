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
	BonusTime = true;
	RecordBest = true;
	TimeTimeout = 60 * 1000;
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

bool CLevelManager::newLevel(string &str1, string &str2)
{
	LevelSessionCount++;
	pausePhysics();
	CNetwork::instance().reset();
	CEntityManager::instance().reset();

	nlinfo("newLevel (%d/%d)",LevelSessionCount,MaxLevelSessionCount);
	if(CurrentLevel)
		CurrentLevel->save();
	
	if(LevelSessionCount<MaxLevelSessionCount && CurrentLevel)
	{
		nlinfo("newLevel() : keeping level : '%s'",CurrentLevel->name().c_str());
		resumePhysics();	
		return true;	
	}

	nextLevel();
	BonusTime = true;
	RecordBest = true;

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
		CurrentLevel = 0;
	}
	for(i = 0; i < levels.size(); i++)
	{
		CLevel *newLevel = new CLevel(levels[(NextLevelId)%levels.size()]);
		if(newLevel->valid())
		{
			CurrentLevel = newLevel;

			// load scores

			CConfigFile::CVar &stats = IService::getInstance()->ConfigFile.getVar("LevelStats");
			nlassert(stats.size() % 5 == 0);

			for(uint i = 0; i < (uint)stats.size(); i += 5)
			{
				if(stats.asString(i) == CFile::getFilename(CurrentLevel->fileName()))
				{
					str1 = toString("Best time: %s with %s seconds", stats.asString(i+1).c_str(), stats.asString(i+2).c_str());
					str2 = toString("Best score: %s with %s points", stats.asString(i+3).c_str(), stats.asString(i+4).c_str());
					break;
				}
			}

			nlinfo(str1.c_str());
			nlinfo(str2.c_str());

			nlinfo("'%s' level loaded",newLevel->name().c_str());
			resumePhysics();
			return true;
		}
		else
		{
			nlinfo("level not valid : don't use it");
			NextLevelId++;
		}
	}

	nlwarning("newLevel() : no valid level found");
	resumePhysics();
	return false;
}

string CLevelManager::updateStats(const std::string &name, sint32 score, float time, bool &breakTime)
{
	string res;
	CConfigFile::CVar &stats = IService::getInstance()->ConfigFile.getVar("LevelStats");
	nlassert(stats.size() % 5 == 0);
	
	if(CurrentLevel == NULL) return "";

	string levelname = CFile::getFilename(CurrentLevel->fileName());

	if(levelname.empty()) return "";
	
	for(uint i = 0; i < (uint)stats.size(); i += 5)
	{
		if(stats.asString(i) == levelname)
		{
			if(score > 0 && int(time*100.0) < int(stats.asFloat(i+2)*100.0f))
			{
				stats.setAsString(name, i+1);
				stats.setAsString(toString("%.2f",time), i+2);
				breakTime = true;
				res = toString("%s has broken the time record with %.2f seconds.", name.c_str(), time);
				nlinfo("best time");
			}
			if(score > 0 && score > stats.asInt(i+4))
			{
				stats.setAsString(name, i+3);
				stats.setAsString(toString("%d",score), i+4);
				nlinfo("best score");
				if(res.empty())
					res = toString("%s has broken the score record with %d points.", name.c_str(), score);
				else
					res = toString("%s has broken the score and time record with %d points and %.2f seconds.", name.c_str(), score, time);
			}
			return res;
		}
	}

	// stat not found for this level, add them
	nlinfo("size %d", stats.size());
	if(stats.size() == 0)
	{
		stats.forceAsString(levelname);
	}
	else
	{
		stats.setAsString(levelname, stats.size());
	}
	stats.SaveWrap = 5;
	stats.setAsString(name, stats.size());
	stats.setAsString(toString("%.2f",time), stats.size());
	stats.setAsString(name, stats.size());
	stats.setAsString(toString("%d",score), stats.size());
	nlinfo("new stat");
	return res;
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
	nlinfo("maxLevelSessionCount(%d)",levelCount);
	MaxLevelSessionCount = levelCount;	
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

void CLevelManager::nextLevel()
{
	NextLevelId++;
	LevelSessionCount = 0;
	MaxLevelSessionCount = 0;
}

bool CLevelManager::bonusTime()
{
	return BonusTime;
}

void CLevelManager::bonusTime(bool b)
{
	BonusTime = b;
}


bool CLevelManager::recordBest()
{
	return RecordBest;
}

void CLevelManager::recordBest(bool b)
{
	RecordBest = b;
}

double CLevelManager::timeTimeout()
{
	return TimeTimeout;
}

void CLevelManager::timeTimeout(double timeout)
{
	TimeTimeout = timeout;
}

