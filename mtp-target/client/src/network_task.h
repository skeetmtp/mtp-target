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
// This class manages the network connection with the server
//

#ifndef MT_NETWORK_TASK_H
#define MT_NETWORK_TASK_H

//
// Includes
//

#include <nel/misc/thread.h>
#include <nel/misc/types_nl.h>

#include <nel/net/tcp_sock.h>
#include <nel/net/inet_address.h>

#include "entity.h"
#include "editable_element.h"
#include "../../common/net_message.h"
#include "../../common/constant.h"


//
// Constants
//

static const float NetVirtualFrameDuration = MT_NETWORK_MY_UPDATE_PERIODE;


//
// Classes
//

class CNetworkRunnable;

class CNetworkTask : public CSingleton<CNetworkTask>, public ITask
{
public:
	
	virtual void init();
	virtual void update() { }
	virtual void render() { }
	virtual void release();

	virtual std::string name() { return "CNetworkTask"; }
	
	// return an empty string if connected, the reason if failed
	std::string connect(NLNET::CInetAddress *ip=0);
	
	void send(CNetMessage &msg);

	void chat(const std::string &msg);
	void command(const std::string &cmd);
	void openClose();
	void ready();
	void stop();
	void force(const NLMISC::CVector &force);
	void updateEditableElement(CEditableElementCommon *element);
	void setEditMode(uint8 editMode);
		
	bool connected();

	NLNET::CTcpSock &sock() { return Sock; }

private:
	
	CNetworkRunnable		*NetworkRunnable;
	NLMISC::IThread			*NetworkThread;
	
	NLNET::CTcpSock		 Sock;

	std::string			 Login;

	friend class CNetworkRunnable;
};


bool pauseNetwork(bool waitAck=true);
bool isNetworkPaused();
void resumeNetwork();

#endif
