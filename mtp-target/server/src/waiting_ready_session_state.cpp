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

#include "bot.h"
#include "main.h"
#include "network.h"
#include "variables.h"
#include "lua_engine.h"
#include "level_manager.h"
#include "entity_manager.h"
#include "session_manager.h"
#include "waiting_ready_session_state.h"
#include "waiting_start_session_state.h"

//
// Namespaces
//

using namespace std;
using namespace NLMISC;


//
// Functions
//

void CWaitingReadySessionState::update()
{
	TTime currentTime = CTime::getLocalTime();
	CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
	CEntityManager::EntityConstIt it;
	
	bool allReady = true;
	// send the message to all entities
	for(it = acces.value().begin(); it != acces.value().end(); it++)
	{
		if(!(*it)->Ready)
		{
			allReady = false;
			break;
		}
	}
	
	if(CSessionManager::instance().forceEnding() || allReady)
	{
		nlinfo("Everybody ready, start session in %g seconds", TimeBeforeStart/1000.0f);
		changeState(CWaitingStartSessionState::instance());
		CSessionManager::instance().startTime(currentTime+(TTime)TimeBeforeStart);
		
		CNetMessage msgout(CNetMessage::Ready);
		CNetwork::instance().send(msgout);
		for(it = acces.value().begin(); it != acces.value().end(); it++)
		{
			(*it)->initBeforeStartLevel();
		}
	}
}
