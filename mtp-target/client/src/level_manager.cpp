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

#include "level.h"
#include "level_manager.h"
#include "resource_manager.h"
#include "entity_manager.h"
#include "config_file_task.h"


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

CLevelManager::CLevelManager() : CSingleton<CLevelManager>()
{
	CurrentLevel = 0;
}

void CLevelManager::init()
{
	CurrentLevel = 0;
}

void CLevelManager::release()
{
	if(CurrentLevel)
	{
		delete CurrentLevel;
		CurrentLevel = 0;
	}
}

void CLevelManager::loadLevel(const std::string &fileName)
{
	nlinfo("Loading level '%s'", fileName.c_str());


	if(CurrentLevel!=0)// || CurrentLevel->filename()!=res)
	{
		delete CurrentLevel;
		CurrentLevel = 0;
	}

	string res = CResourceManager::instance().get(fileName);
	nlassert(!res.empty());
	
	CurrentLevel = new CLevel(res);
	nlassert(CurrentLevel->valid());
	
	if(CConfigFileTask::instance().configFile().getVar("ForceCRCCheck").asInt())
		CResourceManager::instance().CRCCheckTimes.clear();
}

void CLevelManager::display(CLog *log) const
{
	if(CurrentLevel)
		CurrentLevel->display(log);
	else
		log->displayNL("There's not current level");
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
