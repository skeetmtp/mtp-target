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

#include "stdpch.h"

#include "nel/misc/types_nl.h"

#include <stdio.h>
#include <ctype.h>
#include <math.h>

#include <list>

#include "nel/misc/debug.h"
#include "nel/misc/config_file.h"
#include "nel/misc/displayer.h"
#include "nel/misc/command.h"
#include "nel/misc/log.h"

#include "nel/net/service.h"
#include "nel/net/unified_network.h"
#include "nel/net/login_cookie.h"

#include "welcome.h"

using namespace std;
using namespace NLMISC;
using namespace NLNET;

/// association between cookie in string and username
map<string, string> UserIdNameAssociations;

string ListenAddr;

string getUserFromCookie(const string &cookie)
{
	map<string, string>::iterator it = UserIdNameAssociations.find(cookie);
	if(it == UserIdNameAssociations.end())
		return "dummy";
	else
		return(*it).second;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// CONNECTION TO THE FRONT END SERVICE ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void cbFESShardChooseShard (CMessage &msgin, const std::string &serviceName, uint16 sid)
{
	// the WS answer a user authorize
	string reason;
	CLoginCookie cookie;
	string addr;

	//
	// S09: receive "SCS" message from FES and send the "SCS" message to the LS
	//
	
	CMessage msgout ("SCS");

	msgin.serial (reason);
	msgout.serial (reason);

	msgin.serial (cookie);
	msgout.serial (cookie);
	
	if (reason.empty())
	{
		msgin.serial (addr);

		// if we set the FontEndAddress in the welcome_service.cfg we use this address
		if (FrontEndAddress.empty())
		{
			msgout.serial (addr);
		}
		else
		{
			msgout.serial (FrontEndAddress);
		}
	}
	
	CUnifiedNetwork::getInstance()->send ("LS", msgout);
}

// This function is call when a FES accepted a new client or lost a connection to a client
void cbFESClientConnected (CMessage &msgin, const std::string &serviceName, uint16 sid)
{
	//
	// S15: receive "CC" message from FES and send "CC" message to the "LS"
	//

	CMessage msgout ("CC");
	
	uint32 userid;
	msgin.serial (userid);
	msgout.serial (userid);

	uint8 con;
	msgin.serial (con);
	msgout.serial (con);

	CUnifiedNetwork::getInstance()->send ("LS", msgout);

	// add or remove the user number really connected on this shard
	for (list<CFES>::iterator it = FESList.begin(); it != FESList.end(); it++)
	{
		if ((*it).SId == sid)
		{
			if (con) (*it).NbUser++;
			else (*it).NbUser--;
			break;
		}
	}

	if (con)
	{
		// we know that this user is on this FES
		UserIdSockAssociations.insert (make_pair (userid, sid));

	}
	else
	{
		// remove the user
		UserIdSockAssociations.erase (userid);
	}
}

// a new front end connecting to me, add it
void cbFESConnection (const std::string &serviceName, uint16 sid, void *arg)
{
	FESList.push_back (CFES ((TServiceId)sid));
	nldebug("new FES connection: sid %u", sid);
	displayFES ();
}

// a front end closes the connection, deconnect him
void cbFESDisconnection (const std::string &serviceName, uint16 sid, void *arg)
{
	nldebug("new FES disconnection: sid %u", sid);

	for (list<CFES>::iterator it = FESList.begin(); it != FESList.end(); it++)
	{
		if ((*it).SId == sid)
		{
			// send a message to the LS to say that all players from this FES are offline
			map<uint32, TServiceId>::iterator itc = UserIdSockAssociations.begin();
			map<uint32, TServiceId>::iterator nitc = itc;
			while (itc != UserIdSockAssociations.end())
			{
				nitc++;
				if ((*itc).second == sid)
				{
					// bye bye little player
					uint32 userid = (*itc).first;
					nldebug ("due to a frontend crash, removed the player %d", userid);
					CMessage msgout ("CC");
					msgout.serial (userid);
					uint8 con = 0;
					msgout.serial (con);
					CUnifiedNetwork::getInstance()->send ("LS", msgout);
					UserIdSockAssociations.erase (itc);
				}
				itc = nitc;
			}

			// remove the FES
			FESList.erase (it);

			break;
		}
	}

	displayFES ();
}

// Callback Array for message from FES
TUnifiedCallbackItem FESCallbackArray[] =
{
	{ "SCS", cbFESShardChooseShard },
	{ "CC", cbFESClientConnected },
};
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// CONNECTION TO THE LOGIN SERVICE ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void cbLSChooseShard (CMessage &msgin, const std::string &serviceName, uint16 sid)
{
	// the LS warns me that a new client want to come in my shard

	//
	// S07: receive the "CS" message from LS and send the "CS" message to the selected FES
	//

	CLoginCookie cookie;
	string userName;

	msgin.serial(cookie);
	msgin.serial(userName);

	// always accept clients
	string reason = "";

	//
	// S09: receive "SCS" message from FES and send the "SCS" message to the LS
	//

	CMessage msgout("SCS");

	msgout.serial(reason);
	msgout.serial(cookie);

	if(reason.empty())
		msgout.serial(ListenAddr);

	CUnifiedNetwork::getInstance()->send("LS", msgout);

	UserIdNameAssociations.insert(make_pair(cookie.setToString(), userName));

	nlinfo("Client %s will come with cookie %s to ip '%s'", userName.c_str(), cookie.toString().c_str(), ListenAddr.c_str());
}

void cbFailed(CMessage &msgin, const std::string &serviceName, uint16 sid)
{
	// I can't connect to the Login Service, just nlerror();
	string reason;
	msgin.serial(reason);
	nlerror(reason.c_str());
}

void cbLSDisconnectClient(CMessage &msgin, const std::string &serviceName, uint16 sid)
{
	// the LS tells me that i have to disconnect a client
/*
	uint32 userid;
	msgin.serial(userid);

	map<uint32, TServiceId>::iterator it = UserIdSockAssociations.find(userid);
	if(it == UserIdSockAssociations.end())
	{
		nlwarning("Can't disconnect the user %d, he is not found", userid);
	}
	else
	{
		CMessage msgout("DC");
		msgout.serial(userid);
		CUnifiedNetwork::getInstance()->send((*it).second, msgout);
	}*/
}

// connection to the LS, send the identification message
void cbLSConnection(const std::string &serviceName, uint16 sid, void *arg)
{
	CMessage msgout("WS_IDENT");
	sint32 shardId;
	
	try
	{
		shardId = IService::getInstance()->ConfigFile.getVar("ShardId").asInt();
	}
	catch(Exception &)
	{
		shardId = -1;
	}

	if(shardId == -1)
	{
		nlerror("ShardId variable in the config file must be valid(>0)");
	}

	msgout.serial(shardId);
	CUnifiedNetwork::getInstance()->send(sid, msgout);

	nlinfo("Connected to %s-%hu and sent identification with shardId '%d'", serviceName.c_str(), sid, shardId);
}





// Callback Array for message from LS
TUnifiedCallbackItem LSCallbackArray[] =
{
	{ "CS", cbLSChooseShard },
	{ "DC", cbLSDisconnectClient },
	{ "FAILED", cbFailed },
};

/// Init the service, load the universal time.
void initWelcome()
{
	if(IService::getInstance()->ConfigFile.getVar("DontUseLS").asInt() == 1)
		return;

	for(sint i = 0; i < IService::getInstance()->ConfigFile.getVar("LSHost").size(); i++)
	{
		// add a connection to the LS
		string LSAddr = IService::getInstance()->ConfigFile.getVar("LSHost").asString(i);

		// add default port if not set by the config file
		if(LSAddr.find(":") == string::npos)
			LSAddr += ":49999";

		CUnifiedNetwork::getInstance()->addCallbackArray(LSCallbackArray, sizeof(LSCallbackArray)/sizeof(LSCallbackArray[0]));
		CUnifiedNetwork::getInstance()->setServiceUpCallback("LS", cbLSConnection, 0);
		CUnifiedNetwork::getInstance()->addService("LS", LSAddr);
	}

	if(IService::getInstance()->ConfigFile.exists("ListenAddr"))
		ListenAddr = IService::getInstance()->ConfigFile.getVar("ListenAddr").asString();
	else
		ListenAddr = CInetAddress::localHost().ipAddress();

	if(ListenAddr.find(":") == string::npos)
	{
		ListenAddr += ":" + IService::getInstance()->ConfigFile.getVar("TcpPort").asString();
	}

	nlinfo("Listen address send to client will be '%s'", ListenAddr.c_str());
}

void clientConnected(const string &cookie, bool connected)
{
	CLoginCookie c;
	c.setFromString(cookie);
	
	if(!c.isValid())
	{
		nlwarning("Invalid cookie '%s'", cookie.c_str());
		return;
	}

	CMessage msgout("CC");
	
	uint32 userid = c.getUserId();
	msgout.serial(userid);
	
	uint8 con = connected?1:0;
	msgout.serial(con);
	
	CUnifiedNetwork::getInstance()->send("LS", msgout);
}
