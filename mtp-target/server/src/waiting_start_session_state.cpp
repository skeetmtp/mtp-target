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

#include "physics.h"
#include "bot.h"
#include "main.h"
#include "client.h"
#include "network.h"
#include "variables.h"
#include "lua_engine.h"
#include "level_manager.h"
#include "entity_manager.h"
#include "session_manager.h"
#include "running_session_state.h"
#include "waiting_start_session_state.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;


//
// Functions
//

void CWaitingStartSessionState::update()
{
	TTime currentTime = CTime::getLocalTime();
	
	if(CSessionManager::instance().forceEnding() || currentTime > CSessionManager::instance().startTime())
	{
		changeState(CRunningSessionState::instance());

		// set gravity if the game started
		dWorldSetGravity(World, 0.0f, 0.0f, Gravity);

		CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
		CEntityManager::EntityConstIt it;
		for(it = acces.value().begin(); it != acces.value().end(); it++)
		{
			if((*it)->type() == CEntity::Client)
			{
				CClient *c = (CClient *)(*it);
				if(c->ReplayFile)
					fclose(c->ReplayFile);
				
				if(!NLMISC::CFile::isDirectory("./replay"))
					NLMISC::CFile::createDirectory("./replay");
				string CurrentLevel = CLevelManager::instance().currentLevel().name();
				c->ReplayFilename = NLMISC::CFile::findNewFile("replay/"+CurrentLevel+"."+toString(c->StartingPointId)+"..mtr");
				c->ReplayFile = fopen(c->ReplayFilename.c_str(), "wt");
			}
			else if((*it)->type() == CEntity::Bot)
			{
				CBot *b = (CBot *)(*it);
				b->loadBotReplay();
			}
		}
	}
}
