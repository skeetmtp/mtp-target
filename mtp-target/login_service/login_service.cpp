/** \file login_service.cpp
 * Login Service (LS)
 *
 * $Id$
 *
 */

/* Copyright, 2000 Nevrax Ltd.
 *
 * This file is part of NEVRAX NeL Network Services.
 * NEVRAX NeL Network Services is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * NEVRAX NeL Network Services is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with NEVRAX NeL Network Services; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

// 44449 is the server connection for welcome services
// 44448 is the server connection for web
// 44447 is the server connection for clients

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifndef NELNS_CONFIG
#define NELNS_CONFIG ""
#endif // NELNS_CONFIG

#ifndef NELNS_LOGS
#define NELNS_LOGS ""
#endif // NELNS_LOGS

#ifndef NELNS_STATE
#define NELNS_STATE ""
#endif // NELNS_STATE

#include "nel/misc/types_nl.h"

#include <stdio.h>
#include <ctype.h>
#include <math.h>

#include <vector>
#include <map>

#include "nel/misc/debug.h"
#include "nel/misc/config_file.h"
#include "nel/misc/displayer.h"
#include "nel/misc/command.h"
#include "nel/misc/log.h"
#include "nel/misc/window_displayer.h"
#include "nel/misc/path.h"

#include "nel/net/service.h"
#include "nel/net/login_cookie.h"

#include "login_service.h"
//#include "connection_client.h"
#include "connection_ws.h"
#include "connection_web.h"
#include "connection_client.h"


using namespace std;
using namespace NLMISC;
using namespace NLNET;


//
// Variables
//


// store specific user information
NLMISC::CFileDisplayer Fd (NELNS_LOGS "login_service.stat");
NLMISC::CStdDisplayer Sd;
NLMISC::CLog *Output = NULL;

//uint32 CUser::NextUserId = 1;	// 0 is reserved

// Variables

//vector<CUser>	Users;
//vector<CShard>	Shards;

IService *ServiceInstance = NULL;

string DatabaseName, DatabaseHost, DatabaseLogin, DatabasePassword;

MYSQL *DatabaseConnection = NULL;

vector<CShard> Shards;


//
// Functions
//


// transform "192.168.1.1:80" into "192.168.1.1"
string removePort (const string &addr)
{
	return addr.substr (0, addr.find (":"));
}

void checkClients ()
{
	nldebug ("checkClients ()");
	/*for (uint i = 0; i < Users.size (); i++)
	{
		switch (Users[i].State)
		{
		case CUser::Offline:
			nlassert (Users[i].SockId == NULL);
			nlassert (!Users[i].Cookie.isValid());
			nlassert (Users[i].ShardId == NULL);
			break;
		case CUser::Authorized:
			nlassert (Users[i].SockId != NULL);
			nlassert (Users[i].Cookie.isValid());
			nlassert (Users[i].ShardId == NULL);
			break;
		case CUser::Awaiting:
			nlassert (Users[i].SockId == NULL);
			nlassert (!Users[i].Cookie.isValid());
			nlassert (Users[i].ShardId == NULL);
			break;
		case CUser::Online:
			nlassert (Users[i].SockId == NULL);
			nlassert (!Users[i].Cookie.isValid());
			nlassert (Users[i].ShardId != NULL);
			break;
		default:
			nlstop;
			break;
		}
	}*/
}

/*void disconnectClient (CUser &user, bool disconnectClient, bool disconnectShard)
{
	nlinfo ("User %d '%s' need to be disconnect (%d %d %d)", user.Id, user.Login.c_str(), user.State, disconnectClient, disconnectShard);

	switch (user.State)
	{
	case CUser::Offline:
		break;

	case CUser::Authorized:
		if (disconnectClient)
		{
			CNetManager::getNetBase("LS")->disconnect(user.SockId);
		}
		user.Cookie.clear ();
		break;

	case CUser::Awaiting:
		break;

	case CUser::Online:
		if (disconnectShard)
		{
			// ask the WS to disconnect the player from the shard
			CMessage msgout (CNetManager::getNetBase("WSLS")->getSIDA (), "DC");
			msgout.serial (user.Id);
			CNetManager::send ("WSLS", msgout, user.ShardId);
		}
		user.ShardId = NULL;
		break;

	default:
		nlstop;
		break;
	}

	user.SockId = NULL;
	user.State = CUser::Offline;
}
*/
sint findUser (uint32 Id)
{
/*	for (sint i = 0; i < (sint) Users.size (); i++)
	{
		if (Users[i].Id == Id)
		{
			return i;
		}
	}
	// user not found
*/	return -1;
}
/*
string CUser::Authorize (TSockId sender, CCallbackNetBase &netbase)
{
	string reason;

	switch (State)
	{
	case Offline:
		State = Authorized;
		SockId = sender;
		Cookie = CLoginCookie(netbase.hostAddress(sender).internalIPAddress(), Id);
		break;

	case Authorized:
		nlwarning ("user %d already authorized! disconnect him and the other one", Id);
		reason = "You are already authorized (another user uses your account?)";
		disconnectClient (*this, true, true);
		disconnectClient (Users[findUser(Id)], true, true);
		break;

	case Awaiting:
		nlwarning ("user %d already awaiting! disconnect the new user and the other one", Id);
		reason = "You are already awaiting (another user uses your account?)";
		disconnectClient (*this, true, true);
		disconnectClient (Users[findUser(Id)], true, true);
		break;

	case Online:
		nlwarning ("user %d already online! disconnect the new user and the other one", Id);
		reason = "You are already online (another user uses your account?)";
		disconnectClient (*this, true, true);
		disconnectClient (Users[findUser(Id)], true, true);
		break;

	default:
		reason = "default case should never occurs, there's a bug in the login_service.cpp";
		nlstop;
		break;
	}
	return reason;
}
*/
void displayShards ()
{
	ICommand::execute ("shards", *InfoLog);
}

void displayUsers ()
{
	ICommand::execute ("users", *InfoLog);
}



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// SERVICE IMPLEMENTATION /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void beep (uint freq, uint nb, uint beepDuration, uint pauseDuration)
{
#ifdef NL_OS_WINDOWS
	if (ServiceInstance == NULL)
		return;

	try
	{
		if (ServiceInstance->ConfigFile.getVar ("Beep").asInt() == 1)
		{
			for (uint i = 0; i < nb; i++)
			{
				Beep (freq, beepDuration);
				nlSleep (pauseDuration);
			}
		}
	}
	catch (Exception &)
	{
	}
#endif // NL_OS_WINDOWS
}

void cbDatabaseVar (CConfigFile::CVar &var)
{
	DatabaseName = IService::getInstance ()->ConfigFile.getVar("DatabaseName").asString ();
	DatabaseHost = IService::getInstance ()->ConfigFile.getVar("DatabaseHost").asString ();
	DatabaseLogin = IService::getInstance ()->ConfigFile.getVar("DatabaseLogin").asString ();
	DatabasePassword = IService::getInstance ()->ConfigFile.getVar("DatabasePassword").asString ();

	MYSQL *db = mysql_init(NULL);
	if(db == NULL)
	{
		nlwarning ("mysql_init() failed");
		return;
	}

	DatabaseConnection = mysql_real_connect(db, DatabaseHost.c_str(), DatabaseLogin.c_str(), DatabasePassword.c_str(), DatabaseName.c_str(),0,NULL,0);
	if (DatabaseConnection == NULL || DatabaseConnection != db)
	{
		nlerror ("mysql_real_connect() failed to '%s' with login '%s' and database name '%s'", DatabaseHost.c_str(), DatabaseLogin.c_str(), DatabaseName.c_str());
		return;
	}
}


class CLoginService : public IService
{
	bool Init;

public:

	CLoginService () : Init(false) {};

	/// Init the service, load the universal time.
	void init ()
	{
		ServiceInstance = this;

		beep ();

		Output = new CLog;

		Output->addDisplayer (&Fd);
		if (WindowDisplayer != NULL)
			Output->addDisplayer (WindowDisplayer);

		connectionWSInit ();

		connectionWebInit ();

		// Initialize the database access
		
		ConfigFile.setCallback ("ForceDatabaseReconnection", cbDatabaseVar);
		cbDatabaseVar (ConfigFile.getVar ("ForceDatabaseReconnection"));

		// Init the client server connections

		connectionClientInit ();

		Init = true;

		Output->displayNL ("Login Service initialised");
	}

	bool update ()
	{
		connectionClientUpdate ();
		connectionWSUpdate ();
		connectionWebUpdate ();

		return true;
	}

	/// release the service, save the universal time
	void release ()
	{
		if (Init)
		{
			//writePlayerDatabase ();
		}

		connectionWSRelease ();
		connectionWebRelease ();
		
		Output->displayNL ("Login Service released");
	}
};

// Service instanciation
NLNET_SERVICE_MAIN (CLoginService, "LS", "login_service", 49999, EmptyCallbackArray, NELNS_CONFIG, NELNS_LOGS);


//
// Variables
//

/*NLMISC_DYNVARIABLE(uint32, OnlineUsersNumber, "number of actually connected users")
{
	// we can only read the value
	if (get)
	{
		uint32 nbusers = 0;
		for (uint i = 0; i < Users.size(); i++)
		{
			if (Users[i].State == CUser::Online)
				nbusers++;
		}
		*pointer = nbusers;
	}
}
*/

//
// Commands
//

NLMISC_COMMAND (shards, "displays the list of all registered shards", "")
{
	if(args.size() != 0) return false;

	log.displayNL ("Display the %d registered shards :", Shards.size());
	for (uint i = 0; i < Shards.size(); i++)
	{
		log.displayNL ("* ShardId: %d SId: %d NbPlayers: %d", Shards[i].ShardId, Shards[i].SId, Shards[i].NbPlayers);
		CUnifiedNetwork::getInstance()->displayUnifiedConnection (Shards[i].SId, &log);
	}
	log.displayNL ("End of the list");

	return true;
}

/*
NLMISC_COMMAND (registeredUsers, "displays the list of all registered users", "")
{
	if(args.size() != 0) return false;
	log.displayNL ("Display the %d registered users :", Users.size());
	for (uint i = 0; i < Users.size(); i++)
	{
		log.displayNL ("> %d %d '%s' '%s' '%s' '%s' '%s'", Users[i].Id, Users[i].State, Users[i].Login.c_str(), Users[i].Cookie.toString().c_str(), Users[i].SockId->asString().c_str(), Users[i].ShardId->asString().c_str(), Users[i].ShardPrivilege.c_str());
	}
	log.displayNL ("End ot the list");

	checkClients ();
	return true;
}
*/
/*NLMISC_COMMAND (onlineUsers, "displays the list of online users", "")
{
	if(args.size() != 0) return false;

	uint32 nbusers = 0, nbauth = 0, nbwait = 0;
	log.displayNL ("Display the online users :", Users.size());
	for (uint i = 0; i < Users.size(); i++)
	{
		if (Users[i].State == CUser::Online)
		{
			log.displayNL ("> %d '%s' '%s' '%s' '%s' '%s'", Users[i].Id, Users[i].Login.c_str(), Users[i].Cookie.toString().c_str(), Users[i].SockId->asString().c_str(), Users[i].ShardId->asString().c_str(), Users[i].ShardPrivilege.c_str());
			nbusers++;
		}
		else if (Users[i].State == CUser::Awaiting) nbwait++;
		else if (Users[i].State == CUser::Authorized) nbauth++;
	}
	log.displayNL ("End ot the list (%d online users, %d authorized, %d awaiting)", nbusers, nbauth, nbwait);

	checkClients ();
	return true;
}
*/
