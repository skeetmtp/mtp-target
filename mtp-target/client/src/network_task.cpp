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

#ifdef NL_OS_WINDOWS
#	include <winsock2.h>
#	undef min
#	undef max
#elif defined NL_OS_UNIX
#	include <unistd.h>
#	include <sys/time.h>
#	include <sys/types.h>
#endif

#include <nel/misc/path.h>
#include <nel/misc/thread.h>
#include <nel/misc/reader_writer.h>
#include <nel/misc/bit_mem_stream.h>

#include <nel/net/sock.h>
#include <nel/net/tcp_sock.h>
#include <nel/net/callback_client.h>

#include "mtp_target.h"
#include "network_task.h"
#include "net_callbacks.h"
#include "config_file_task.h"
#include "../../common/net_message.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;
using namespace NLNET;


//
// Variables
//

FILE *SessionFile = 0;


//
// Thread
//

class CNetworkRunnable : public NLMISC::IRunnable
{
public:
	
	CNetworkRunnable()
	{
	}

	virtual void run()
	{
		while(true)
		{
			if (CNetworkTask::instance().sock().connected())
			{
				CNetMessage msg(CNetMessage::Unknown, true);
				
				NLNET::CSock::TSockResult res = msg.receive(&CNetworkTask::instance().sock());
				
				switch(res)
				{
				case NLNET::CSock::Ok:
					netCallbacksHandler(msg);
					break;
				case NLNET::CSock::ConnectionClosed:
					nlinfo("Lost the server");
					break;
				default:
					nlerror("Received failed: %s (code %u)", NLNET::CSock::errorString(NLNET::CSock::getLastError()).c_str(), NLNET::CSock::getLastError());
					break;
				}
			}
			else
			{
				nlSleep(100);
			}
		}
	}

private:

};


//
// Variables
//


//
// Functions
//

void CNetworkTask::init()
{
	NetworkRunnable = new CNetworkRunnable();
	nlassert(NetworkRunnable);
	
	NetworkThread = NLMISC::IThread::create(NetworkRunnable);
	nlassert(NetworkThread);
	
	NetworkThread->start();
}

void CNetworkTask::release()
{
	if(!NetworkThread || !NetworkRunnable)
		return;
	
	delete NetworkThread;
	NetworkThread = 0;
	delete NetworkRunnable;
	NetworkRunnable = 0;
}

bool CNetworkTask::connected()
{
	return Sock.connected();
}

string CNetworkTask::connect(CInetAddress *ip)
{
	CInetAddress addr;

	if (Sock.connected())
		return "";

	try
	{
		if (ip==NULL)
		{
			// use cfg
			addr.setNameAndPort(CConfigFileTask::instance().configFile().getVar("ServerHost").asString()+":"+toString(CConfigFileTask::instance().configFile().getVar("TcpPort").asInt()));
		}
		else
		{
			// use cookie
			addr = *ip;
			//.setNameAndPort(FSAddr+":"+toString(CConfigFileTask::instance().configFile().getVar("TcpPort").asInt()));
		}
		nlinfo ("Connection to the TCP address: %s", addr.asString().c_str());

		Sock.connect(addr);
		Sock.setNoDelay(true);
		Sock.setNonBlockingMode(false);

		CNetMessage msgout(CNetMessage::Login);
		Login = CConfigFileTask::instance().configFile().getVar("Login").asString();
		string password = CConfigFileTask::instance().configFile().getVar("Password").asString();
		CRGBA color(CConfigFileTask::instance().configFile().getVar("Color").asInt(0), CConfigFileTask::instance().configFile().getVar("Color").asInt(1), CConfigFileTask::instance().configFile().getVar("Color").asInt(2));
		msgout.serial(Cookie, Login, password, color);
		CNetworkTask::instance().send(msgout);
	}
	catch (Exception &e)
	{
//		return std::string("Can't connect to TCP server: ") + e.what();
		return std::string("error : ") + e.what();
	}
	
	return "";
}

void CNetworkTask::send(CNetMessage &msg)
{
	msg.send(&Sock);
}

void CNetworkTask::chat(const string &msg)
{
	// chat
	CNetMessage msgout(CNetMessage::Chat);
	string m = msg;
	msgout.serial(m);
	send(msgout);
}

void CNetworkTask::command(const string &cmd)
{
	CNetMessage msgout(CNetMessage::Command);
	string c = cmd;
	msgout.serial(c);
	send(msgout);
}

void CNetworkTask::openClose()
{
	CNetMessage msgout(CNetMessage::OpenClose);
	send(msgout);
}

void CNetworkTask::ready()
{
	nlinfo ("send ready");

	CNetMessage msgout(CNetMessage::Ready);
	send(msgout);
}

void CNetworkTask::force(const NLMISC::CVector &force)
{
	CNetMessage msgout(CNetMessage::Force);
	CVector f(force);
	msgout.serial(f);
	send(msgout);
}

void CNetworkTask::updateEditableElement(CEditableElementCommon *element)
{
	CNetMessage msgout(CNetMessage::UpdateElement);
	uint8 elementId = element->id();
	uint8 clientId = 0;
	uint8 elementType = element->type();
	msgout.serial(elementType);
	msgout.serial(elementId);
	msgout.serial(clientId);
	CVector pos = element->position();
	msgout.serial(pos);
	CVector rot = CVector(0,0,0);
	msgout.serial(rot);
	send(msgout);
}

void CNetworkTask::setEditMode(uint8 editMode)
{
	CNetMessage msgout(CNetMessage::EditMode);
	uint8 em = editMode;
	msgout.serial(em);
	send(msgout);
	
}
