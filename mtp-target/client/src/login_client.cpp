/** \file login_client.cpp
 * CLoginClientMtp is the interface used by the client to identifies itself to the login_sytem and
 * connects to the shard.
 *
 * $Id$
 *
 */

/* Copyright, 2000 Nevrax Ltd.
 *
 * This file is part of NEVRAX NEL.
 * NEVRAX NEL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * NEVRAX NEL is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with NEVRAX NEL; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#include "stdpch.h"

#include "nel/misc/system_info.h"

#include "nel/net/callback_client.h"

#include "nel/net/login_cookie.h"
#include "login_client.h"

#include "nel/net/udp_sock.h"

#include "3d_task.h"

using namespace std;
using namespace NLMISC;
using namespace NLNET;
using namespace NL3D;

//namespace NLNET {

CLoginClientMtp::TShardList CLoginClientMtp::ShardList;

string CLoginClientMtp::_GfxInfos;

CCallbackClient *CLoginClientMtp::_CallbackClient = NULL;


// Callback for answer of the login password.
bool VerifyLoginPassword;
string VerifyLoginPasswordReason;
void cbVerifyLoginPassword (CMessage &msgin, TSockId from, CCallbackNetBase &netbase)
{
	//
	// S04: receive the "VLP" message from LS
	//

	uint8 ok;
	msgin.serial (ok);
	if (ok)
	{
		uint32 nbshard = 0;
		msgin.serial (nbshard);

		CLoginClientMtp::ShardList.clear ();
		VerifyLoginPasswordReason = "";

		// get the shard list
		for (uint i = 0; i < nbshard; i++)
		{
			CLoginClientMtp::CShardEntry se;
			msgin.serial (se.ShardName);
			msgin.serial (se.WSAddr);
			CLoginClientMtp::ShardList.push_back (se);
		}		
	}
	else
	{
		msgin.serial (VerifyLoginPasswordReason);
	}
	VerifyLoginPassword = true;
}

// Callback for answer of the request shard
bool ShardChooseShard;
string ShardChooseShardReason;
string ShardChooseShardAddr;
CLoginCookie ShardChooseShardCookie;
void cbShardChooseShard (CMessage &msgin, TSockId from, CCallbackNetBase &netbase)
{
	//
	// S11: receive "SCS" message from LS
	//

	msgin.serial (ShardChooseShardReason);
	
	if (ShardChooseShardReason.empty())
	{
		msgin.serial (ShardChooseShardCookie);
		msgin.serial (ShardChooseShardAddr);
	}
	ShardChooseShard = true;
}

static TCallbackItem CallbackArray[] =
{
	{ "VLP", cbVerifyLoginPassword },
	{ "SCS", cbShardChooseShard },
};

//

string CLoginClientMtp::authenticate (const string &loginServiceAddr, const string &login, const string &password, uint32 clientVersion)
{
	//
	// S01: connect to the LS
	//
	try
	{
		nlassert (_CallbackClient == NULL);
		_CallbackClient = new CCallbackClient();
		_CallbackClient->addCallbackArray (CallbackArray, sizeof(CallbackArray)/sizeof(CallbackArray[0]));
		
		string addr = loginServiceAddr;
		if(addr.find(":") == string::npos)
			addr += ":49997";
		_CallbackClient->connect (CInetAddress(addr));
	}
	catch (ESocket &e)
	{
		delete _CallbackClient;
		_CallbackClient = NULL;
		nlwarning ("Connection refused to LS (addr:%s): %s", loginServiceAddr.c_str(), e.what ());
		return "Connection refused to LS";
	}

	//
	// S02: create and send the "VLP" message
	//
	CMessage msgout ("VLP");
	msgout.serial (const_cast<string&>(login));
	msgout.serial (const_cast<string&>(password));
	msgout.serial (clientVersion);

	string OS, Proc, Mem, Gfx;
	OS = CSystemInfo::getOS().c_str();
	Proc = CSystemInfo::getProc().c_str();
	Mem = toString(CSystemInfo::availablePhysicalMemory()) + " " + toString(CSystemInfo::totalPhysicalMemory());
	msgout.serial (OS);
	msgout.serial (Proc);
	msgout.serial (Mem);
	_GfxInfos = C3DTask::instance().driver().getDriverInformation();
	msgout.serial (_GfxInfos);

	_CallbackClient->send (msgout);

	// wait the answer from the LS
	VerifyLoginPassword = false;
	while (_CallbackClient->connected() && !VerifyLoginPassword)
	{
		_CallbackClient->update ();
		nlSleep(10);
	}

	// have we received the answer?
	if (!VerifyLoginPassword)
	{
		delete _CallbackClient;
		_CallbackClient = NULL;
		return "CLoginClientMtp::authenticate(): LS disconnects me";
	}

	if (!VerifyLoginPasswordReason.empty())
	{
		_CallbackClient->disconnect ();
		delete _CallbackClient;
		_CallbackClient = NULL;
	}

	return VerifyLoginPasswordReason;
}

string CLoginClientMtp::confirmConnection (uint32 shardListIndex)
{
	nlassert (_CallbackClient != NULL && _CallbackClient->connected());
	nlassert (shardListIndex < ShardList.size());

	//
	// S05: send the client shard choice
	//

	// send CS
	CMessage msgout ("CS");
	msgout.serial (ShardList[shardListIndex].WSAddr);
	_CallbackClient->send (msgout);

	// wait the answer
	ShardChooseShard = false;
	while (_CallbackClient->connected() && !ShardChooseShard)
	{
		_CallbackClient->update ();
		nlSleep(10);
	}

	// have we received the answer?
	if (!ShardChooseShard)
	{
		delete _CallbackClient;
		_CallbackClient = NULL;
		return "CLoginClientMtp::connectToShard(): LS disconnects me";
	}
	else
	{
		_CallbackClient->disconnect ();
		delete _CallbackClient;
		_CallbackClient = NULL;
	}

	if (!ShardChooseShardReason.empty())
	{
		return ShardChooseShardReason;
	}

	// ok, we can try to connect to the good front end

	nlinfo("addr:%s cookie:%s", ShardChooseShardAddr.c_str(), ShardChooseShardCookie.toString().c_str());

	return "";
}

string CLoginClientMtp::connectToShard (uint32 shardListIndex, CInetAddress &ip, CLoginCookie &cookie)
{
	string res = confirmConnection (shardListIndex);
	if (!res.empty()) return res;

	ip = ShardChooseShardAddr;
	cookie = ShardChooseShardCookie;
	
	return "";
}


//} // NLNET

/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
/*
using namespace NLMISC;
using namespace NLNET;
using namespace std;

static uint32 ClientVersion = 1;

#include <time.h>

void main ()
{
	string res = CLoginClientMtp::authenticate ("localhost:49999", "login", "password", ClientVersion);

	if (res.empty())
	{
		// ok
		nlinfo("auth OK");

		nlinfo("Shard list:");
		for (uint i = 0; i < CLoginClientMtp::ShardList.size(); i++)
		{
			nlinfo(" > %s %s", CLoginClientMtp::ShardList[i].ShardName.c_str(), CLoginClientMtp::ShardList[i].WSAddr.c_str());
		}
		nlinfo("End shard list");

		CCallbackClient cnx;
		res = CLoginClientMtp::connectToShard (0, cnx);

		if (res.empty())
		{
			// ok
			nlinfo("cnx OK");

			while (cnx.connected ())
			{
				cnx.update ();
			}
		}
		else
		{
			// not ok, and reason
			nlinfo("cnx FAILED: %s", res.c_str());
		}
	}
	else
	{
		// not ok, and reason
		nlinfo("auth FAILED: %s", res.c_str());
	}

	getchar ();
}
*/
