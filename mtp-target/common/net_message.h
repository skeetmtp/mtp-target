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
// This class manages the loading of a level
//

#ifndef MT_NET_MESSAGE
#define MT_NET_MESSAGE


//
// Includes
//

#include <nel/misc/types_nl.h>
#include <nel/misc/mem_stream.h>

#include <nel/net/tcp_sock.h>

#define MTPT_NETWORK_VERSION 1

//
// Classes
//

//
// Main class that manage buffer messages, receive and send
//

class CNetMessage : public NLMISC::CMemStream
{
public:

	enum TType
	{
		Unknown = 0,
		// alphabetic order
		Chat,				// CS SC
		Command,			// CS
		DisplayText,		//    SC
		EndSession,			//    SC
		EditMode,			// CS SC
		Error,				//    SC
		Force,				// CS
		Login,				// CS SC
		Logout,				//    SC
		OpenClose,			// CS SC
		Ready,				// CS SC
		RequestCRCKey,		// CS SC
		RequestDownload,	// CS SC
		SessionState,		//    SC
		StartSession,		//    SC
		Update,				// CS SC
		UpdateOne,			//    SC
		UpdateList,			//    SC
		FullUpdate,			//    SC
		UpdateElement,		// CS SC
		ShowElement,		//    SC
	};

	CNetMessage(TType type = Unknown, bool inputStream = false);

	TType type() const { return Type; }

	void setHeader (TType type);

private:

	TType Type;

	void send (NLNET::CTcpSock *ts);
	NLNET::CSock::TSockResult receive (NLNET::CTcpSock *ts);

	friend class CNetwork;
	friend class CNetworkTask;
	friend class CNetworkRunnable;
};

#endif
