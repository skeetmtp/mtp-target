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
#include "level_manager.h"
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
	_editMode = 0;
}

void CSessionManager::update()
{
	nlassert(CurrentState);
	CurrentState->update();
}

void CSessionManager::release()
{
	CSessionManager::uninstance();
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
	CEntityManager::instance().saveAllValidReplay();
	changeState(CWaitingClientsSessionState::instance());
	StartTime = 0;
	EndTime = 0;
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

NLMISC_COMMAND(reparsePath, "reparse path for new file", "")
{
	reparsePath();
	log.displayNL("reparsePath");
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
			*pointer = (sint32)((CSessionManager::instance().startTime()+(TTime)CLevelManager::instance().timeTimeout() - currentTime)/1000);
		}
		else
		{
			*pointer = 0;
		}
	}
}
