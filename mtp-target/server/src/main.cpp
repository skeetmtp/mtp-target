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

#include <nel/misc/vector.h>
#include <nel/misc/time_nl.h>
#include <nel/misc/command.h>
#include <nel/misc/displayer.h>
#include <nel/misc/bit_mem_stream.h>
#include <nel/misc/hierarchical_timer.h>

#include <nel/net/service.h>
#include <nel/net/udp_sock.h>

#include "main.h"
#include "level.h"
#include "physics.h"
#include "welcome.h"
#include "network.h"
#include "variables.h"
#include "level_manager.h"
#include "entity_manager.h"
#include "session_manager.h"
#include "../../common/constant.h"


//
// Namespaces
//

using namespace std;
using namespace NLNET;
using namespace NLMISC;


//
// Variables
//

uint32 UpdatePacketSize = 0;

bool ForceFinish = false;

TTime LastPingSent = 0;	// When we send all information of client (ping)

TTime LastSentUpdate = 0;
TTime NextSendUpdate = 0;

const float GSCALE = 0.01f;


//
// Classes
//

class CMtpTargetService : public NLNET::IService
{
public:

	void init()
	{
#ifdef NL_OS_WINDOWS
		BOOL res;
		if(ConfigFile.getVar("HighPriority").asInt()==1)
		{
			res = SetPriorityClass(GetCurrentProcess(),HIGH_PRIORITY_CLASS);
			//res = SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_HIGHEST);
			nlinfo("High priority %s",res==TRUE?"set":"not set");
		}
#endif

		CPath::remapExtension("dds", "tga", true);

		initVariables();
		
		srand(CTime::getSecondsSince1970());

		initPhysics();

		CEntityManager::instance().init();

		CNetwork::instance().init();

		CSessionManager::instance().init();

		setUpdateTimeout(MT_NETWORK_MY_UPDATE_PERIODE_MS);

		initWelcome();

		nlinfo("Mtp Target User Init");
	}
	
	bool	update()
	{
		CNetwork::instance().update();
		CEntityManager::instance().update();
		
		// Update the current session
		CSessionManager::instance().update();

		CLevelManager::instance().update();

		return true;
	}

	void release()
	{
		CLevelManager::instance().release();
		CEntityManager::instance().release();
		CSessionManager::instance().release();

		releasePhysics();
	}

};
 
NLNET_SERVICE_MAIN(CMtpTargetService, "MTS", "mtp_target_service", 0, EmptyCallbackArray, "", "");


//
// Commands
//

NLMISC_COMMAND(broadcast, "send a message to everybody", "<string>")
{
	if(args.size() == 0)
		return false;

	string msg;
	
	for(uint i = 0; i < args.size(); i++)
		msg += args[i]+" ";

	CNetwork::instance().sendChat(msg);
	
	return true;
}

NLMISC_VARIABLE(uint32, UpdatePacketSize, "update packet size");
