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

#include "main.h"
#include "client.h"
#include "network.h"
#include "variables.h"
#include "session_manager.h"
#include "waiting_clients_session_state.h"


//
// Namespaces
//

using namespace std;
using namespace NLNET;
using namespace NLMISC;


//
// Functions
//

void CSessionManager::init()
{
	// set the first state
	CurrentState = 0;
	changeState(CWaitingClientsSessionState::instance());
	
	ForceEnding = false;

	StartTime = 0;
	EndTime = 0;
}

void CSessionManager::update()
{
	nlassert(CurrentState);
	CurrentState->update();
}

void CSessionManager::release()
{
}

void CSessionManager::changeState(CSessionState &ns)
{
	if(CurrentState)
	{
		nldebug("Calling leave() for state %s", CurrentState->name().c_str());
		CurrentState->leave();
	}

	CurrentState = &ns;

	nldebug("Calling enter() for state %s", CurrentState->name().c_str());
	CurrentState->enter();

	// send this new state to all clients
	CNetMessage msgout(CNetMessage::SessionState);
	string sn = ns.shortName();
	msgout.serial(sn);
	CNetwork::instance().send(msgout);
}	

void CSessionManager::reset()
{
	CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
	saveAllValidReplay(acces);
	changeState(CWaitingClientsSessionState::instance());
	StartTime = 0;
	EndTime = 0;
}

void CSessionManager::saveAllValidReplay(CEntityManager::CEntities::CReadAccessor &acces)
{
	CEntityManager::EntityConstIt it;
	// close all replay file
	for(it = acces.value().begin(); it != acces.value().end(); it++)
	{
		if((*it)->type() == CEntity::Client)
		{
			CClient *c = (CClient *)(*it);
			if(c->ReplayFile)
			{
				fprintf(c->ReplayFile, "%d AU %.1f %s %d\n", c->id(), 0.0f, c->name().c_str(), c->CurrentScore);
				fclose(c->ReplayFile);
				c->ReplayFile = 0;
				if(c->CurrentScore <(uint32)IService::getInstance()->ConfigFile.getVar("SavedReplayMinimumScore").asInt())
					CFile::deleteFile(c->ReplayFilename.c_str());
			}
		}
		//		else
		//			nlinfo("bot do score = %d ",c->CurrentScore);
	}
}


//
// Commands
//

NLMISC_COMMAND(reset, "reset the session brutally", "")
{
	CSessionManager::instance().reset();
	log.displayNL("reset");
	return true;
}

NLMISC_COMMAND(forceEnd, "end the session en display the score", "")
{
	CSessionManager::instance().forceEnding(true);
	log.displayNL("forceEnd");
	return true;
}

NLMISC_DYNVARIABLE(string, CurrentState, "")
{
	if(get)
	{
		*pointer = CSessionManager::instance().currentStateName();
	}
}

NLMISC_DYNVARIABLE(sint32, Timeout, "")
{
	if(get)
	{
		if(CSessionManager::instance().startTime())
		{
			TTime currentTime = CTime::getLocalTime();
			*pointer = (sint32)((CSessionManager::instance().startTime()+(TTime)TimeTimeout - currentTime)/1000);
		}
		else
		{
			*pointer = 0;
		}
	}
}
