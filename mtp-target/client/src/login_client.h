/** \file login_client.h
 * CLoginClient is the interface used by the client to identifies itself to the login_sytem and
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

#ifndef LOGIN_CLIENT_H
#define LOGIN_CLIENT_H


//
// Includes
//

#include <nel/misc/types_nl.h>

#include <string>
#include <vector>

#include <nel/net/udp_sock.h>
#include <nel/net/login_cookie.h>
#include <nel/net/callback_client.h>


//
// Classes
//

class CLoginClientMtp
{
public:

	struct CShardEntry
	{
		CShardEntry() { }
		CShardEntry(const std::string &name, uint32 sid) : ShardName(name), ShardId(sid)
		{
		}
		std::string ShardName;
		uint32		ShardId;
	};

	typedef std::vector<CShardEntry> TShardList;


	/** Set the graphics informations (card name, etc...) that will be send to the Login Service
	 *	ex: CLoginClient::setInformations (CNELU::Driver->getVideocardInformation ());
	 */
	//static void setInformations (std::string gfxInfos) { _GfxInfos = gfxInfos; }


	/** Tries to connect to the authentication server.
	 * Generates a ESocketConnectionFailed if it can't establish the connection.
	 */
	/** Tries to login with login and password.
	 * If the authentication is ok, the function return an empty string else it returns the reason of the failure.
	 */
	static std::string authenticate (const std::string &loginServiceAddr, const std::string &login, const std::string &password, uint32 clientVersion);


	/** Try to connect to the shard and return a TCP connection to the shard.
	 */
	static std::string connectToShard (uint32 shardListIndex, NLNET::CInetAddress &ip, std::string &cookie);

	static TShardList ShardList;

private:

	static std::string confirmConnection (uint32 shardListIndex);

	static std::string _GfxInfos;

	static NLNET::CCallbackClient *_CallbackClient;

};

#endif // LOGIN_CLIENT_H

/* End of login_client.h */
