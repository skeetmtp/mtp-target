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

#include <nel/misc/thread.h>
#include <nel/misc/reader_writer.h>

#include <nel/net/sock.h>
#include <nel/net/service.h>
#include <nel/net/listen_sock.h>

#include "main.h"
#include "physics.h"
#include "network.h"
#include "net_callbacks.h"
#include "entity_manager.h"
#include "../../common/constant.h"
#include "../../common/net_message.h"
#include "../../common/custom_floating_point.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;



//
// Thread
//

class CNetworkTask : public NLMISC::IRunnable
{
public:
	
	CNetworkTask()
	{
		ListenSock.init(NLNET::IService::getInstance()->ConfigFile.getVar("TcpPort").asInt());

#if defined NL_OS_UNIX
		nice(2);
#endif // NL_OS_UNIX
	}

	virtual void run()
	{
		NLNET::SOCKET descmax;
		fd_set readers;
		timeval tv;

		while(true)
		{
			descmax = ListenSock.descriptor();
			FD_ZERO(&readers);
			FD_SET(ListenSock.descriptor(), &readers);

			{
				CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
				for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
				{
					nlassert(*it);
					if((*it)->type() != CEntity::Client)
						continue;

					CClient *c = (CClient *)(*it);

					nlassert(c->sock());
					FD_SET(c->sock()->descriptor(), &readers);
					descmax = std::max(descmax, c->sock()->descriptor());
				}
			}

			tv.tv_sec = 3600;		// 1 hour
			tv.tv_usec = 0;

			int res = ::select(descmax+1, &readers, 0, 0, &tv);
			
			switch(res)
			{
			case  0 : continue; // time-out expired, no results
			case -1 :
				if(NLNET::CSock::getLastError() == 4)
				{
					// we ignore the message(Interrupted system call) caused by a CTRL-C
					nldebug("Select failed: %s (code %u) but IGNORED", NLNET::CSock::errorString(NLNET::CSock::getLastError()).c_str(), NLNET::CSock::getLastError());
					continue;
				}
				nlerror("Select failed: %s (code %u)", NLNET::CSock::errorString(NLNET::CSock::getLastError()).c_str(), NLNET::CSock::getLastError());
			}

			if(FD_ISSET(ListenSock.descriptor(), &readers) != 0)
			{
				nlinfo("New client connected");

				NLNET::CTcpSock *sock = ListenSock.accept();
				sock->setNoDelay(true);
				sock->setNonBlockingMode(false);

				CEntityManager::instance().addClient(sock);
			}

			vector<uint8> IdToRemove;

			{
				CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
				for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
				{
					nlassert(*it);
					CClient *c = (CClient *)(*it);
					
					if((*it)->type() != CEntity::Client)
						continue;

					nlassert(c->sock());

					if(FD_ISSET(c->sock()->descriptor(), &readers) == 0)
						continue;

					CNetMessage msg(CNetMessage::Unknown, true);
					
					NLNET::CSock::TSockResult res = msg.receive(c->sock());

					switch(res)
					{
					case NLNET::CSock::Ok:
						netCallbacksHandler(c, msg);
						break;
					case NLNET::CSock::ConnectionClosed:
						nlinfo("Lost client eid %hu", (uint16)c->id());
						IdToRemove.push_back(c->id());
						break;
					default:
						nlwarning("Received failed from client eid %hu : %s (code %u)", (uint16)c->id(), NLNET::CSock::errorString(NLNET::CSock::getLastError()).c_str(), NLNET::CSock::getLastError());
						IdToRemove.push_back(c->id());
						break;
					}
				}
			}

			for(uint i = 0; i < IdToRemove.size(); i++)
			{
				CEntityManager::instance().remove(IdToRemove[i]);
			}
			IdToRemove.clear();
		}
	}

private:

	NLNET::CListenSock ListenSock;
};


//
// Variables
//



//
// Functions
//

CNetwork::CNetwork() : NetworkTask(0), NetworkThread(0)
{
}

void CNetwork::init()
{
	nlassert(!NetworkTask);
	nlassert(!NetworkThread);

	NetworkTask = new CNetworkTask();
	nlassert(NetworkTask);

	NetworkThread = NLMISC::IThread::create(NetworkTask);
	nlassert(NetworkThread);

	NetworkThread->start();
	updateCount = 0;
}

void CNetwork::reset()
{
	updateCount = 0;	
}


static FILE *fp = NULL;
static int rpos;
static int updateCount2 = 0;

void CNetwork::update()
{
	if(CEntityManager::instance().humanClientCount()==0) return;
	if(updateCount2==0)
	{
		fp = fopen("net.data","wb");
		rpos = 0;//rand()%10;
	}
	updateCount2++;

	{
		CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
		bool sendUpdateList = false;
		TTime currentTime = CTime::getLocalTime();
		unsigned int i;
		CEntityManager::EntityConstIt it;
		for(i=0,it = acces.value().begin(); it != acces.value().end(); it++,i++)
		{
			const dReal *pos = dBodyGetPosition((*it)->Body);
			CVector npos;
			
			npos.x = (float)pos[0];
			npos.y = (float)pos[1]; 
			npos.z = (float)pos[2];
			
			uint8 eid = (*it)->id();
			if(i>=CEntityManager::instance().IdUpdateList.size() || eid!=CEntityManager::instance().IdUpdateList[i])
				sendUpdateList = true;
			uint16 ping = (*it)->Ping.getSmoothValue();
			//msgout.serial(eid);
			
			CVector dpos = npos - (*it)->LastSent2OthersPos;
			if(dpos.norm()>100)
			{
				updateCount=0;
				break;
			}
		}
		if(sendUpdateList)
			CEntityManager::instance().sendUpdateList();
	}
	
	if((updateCount%MT_NETWORK_FULL_UPDATE_PERIODE)==0)
//	if(updateCount==0)
	{
		CNetMessage msgout(CNetMessage::FullUpdate);
		
		{
			CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
			
			TTime currentTime = CTime::getLocalTime();
			
			for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
			{
				const dReal *pos = dBodyGetPosition((*it)->Body);
				
				(*it)->Pos.x = (float)pos[0];
				(*it)->Pos.y = (float)pos[1]; 
				(*it)->Pos.z = (float)pos[2];
				
				uint8 eid = (*it)->id();
				uint16 ping = (*it)->Ping.getSmoothValue();
				//msgout.serial(eid);
				msgout.serial((*it)->Pos, ping);
				
				if((*it)->type() != CEntity::Bot)
					(*it)->LastSentPing.push(currentTime);
				
				(*it)->LastSent2MePos = (*it)->Pos;
				(*it)->LastSent2OthersPos = (*it)->Pos;
			}
		}
		
		UpdatePacketSize = msgout.length();
		
		CNetwork::instance().send(msgout);
	}
	else if((updateCount%MT_NETWORK_MY_UPDATE_FREQUENCE_RATIO)==0)
	{
		CNetMessage msgout(CNetMessage::Update);
		
		{
			CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
			
			TTime currentTime = CTime::getLocalTime();
			
			for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
			{
				const dReal *pos = dBodyGetPosition((*it)->Body);
				
				(*it)->Pos.x = (float)pos[0];
				(*it)->Pos.y = (float)pos[1]; 
				(*it)->Pos.z = (float)pos[2];
				
				uint8 eid = (*it)->id();
				uint16 ping = (*it)->Ping.getSmoothValue();
				//msgout.serial(eid);

				CVector dpos = (*it)->Pos - (*it)->LastSent2OthersPos;
				
				uint8 sx;
				uint8 dx;
				CVector sendDPos;
				packBit32 pb32;

				sendDPos.x = computeOut8_8fp(dpos.x,dx,sx);
				pb32.packBits(dx,4);
				pb32.packBits(sx,6);
				sendDPos.y = computeOut8_8fp(dpos.y,dx,sx);
				pb32.packBits(dx,4);
				pb32.packBits(sx,6);
				sendDPos.z = computeOut8_8fp(dpos.z,dx,sx);
				pb32.packBits(dx,4);
				pb32.packBits(sx,6);

				msgout.serial(pb32.bits);

				/*
				if((*it)->id()==rpos && dpos.z!=0)
				//if(dpos.z!=0)
				{
					sint8 dsx = sx;// - (*it)->LastSentSX;
					//fwrite(&dx,1,1,fp);
					fwrite(&dsx,1,1,fp);
					fflush(fp);
					(*it)->LastSentSX = sx;
				}
				*/
				
				
				if((*it)->type() != CEntity::Bot)
					(*it)->LastSentPing.push(currentTime);
				
				CVector newPos = (*it)->LastSent2OthersPos + sendDPos; 
				(*it)->LastSent2OthersPos = newPos;
				(*it)->LastSent2MePos = newPos;
			}
		}
		
		CNetwork::instance().send(msgout);
		UpdatePacketSize = msgout.length();
		
	}
	else
	{
		CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
		
		TTime currentTime = CTime::getLocalTime();
		
		for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
		{
			if((*it)->type() != CEntity::Bot)
			{
				CNetMessage msgout(CNetMessage::UpdateOne);
				const dReal *pos = dBodyGetPosition((*it)->Body);
				
				(*it)->Pos.x = (float)pos[0];
				(*it)->Pos.y = (float)pos[1]; 
				(*it)->Pos.z = (float)pos[2];
				
				uint8 eid = (*it)->id();
				uint16 ping = (*it)->Ping.getSmoothValue();
				//msgout.serial(eid);
				
				CVector dpos = (*it)->Pos - (*it)->LastSent2MePos;
				
				uint8 sx;
				uint8 dx;
				packBit32 pb32;
				CVector sendDPos;

				sendDPos.x = computeOut8_8fp(dpos.x,dx,sx);
				pb32.packBits(dx,4);
				pb32.packBits(sx,6);
				sendDPos.y = computeOut8_8fp(dpos.y,dx,sx);
				pb32.packBits(dx,4);
				pb32.packBits(sx,6);
				sendDPos.z = computeOut8_8fp(dpos.z,dx,sx);
				pb32.packBits(dx,4);
				pb32.packBits(sx,6);

				msgout.serial(pb32.bits);
				
				CVector newPos = (*it)->LastSent2MePos + sendDPos; 
				
				CNetwork::instance().send((*it)->id(),msgout,true);
				
				(*it)->LastSent2MePos = newPos;
			}

		}
	}
	updateCount++;
	CEntityManager::instance().flushRemoveList();
}


void CNetwork::release()
{
	if(fp)
		fclose(fp);
	if(!NetworkThread || !NetworkTask)
		return;

	NetworkThread->terminate();
	delete NetworkThread;
	NetworkThread = 0;
	delete NetworkTask;
	NetworkTask = 0;
	CNetwork::uninstance();
}


void CNetwork::send(uint8 eid, CNetMessage &msg, bool checkReady)
{
	CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
	for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
	{
		if((*it)->id() == eid)
		{
			nlassert((*it)->type() == CEntity::Client);
			CClient *c = (CClient *)(*it);
			if(!checkReady || c->networkReady())
			{
				bool sok = msg.send(c->sock());
				if(!sok)
					CEntityManager::instance().addClientToRemoveList(c);
			}
		}
	}
}

void CNetwork::send(CNetMessage &msg)
{
	CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
	for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
	{
		if((*it)->type() == CEntity::Client)
		{
			CClient *c = (CClient *)(*it);
			if(c->networkReady())
			{
				bool sok = msg.send(c->sock());
				if(!sok)
					CEntityManager::instance().addClientToRemoveList(c);
			}
		}
	}
}

void CNetwork::sendAllExcept(uint8 eid, CNetMessage &msg)
{
	CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
	for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
	{
		if((*it)->id() != eid && (*it)->type() == CEntity::Client)
		{
			CClient *c = (CClient *)(*it);
			if(c->networkReady())
			{
				bool sok = msg.send(c->sock());
				if(!sok)
					CEntityManager::instance().addClientToRemoveList(c);
			}
		}
	}
}

void CNetwork::sendChat(const string &msg)
{
	CNetMessage msgout(CNetMessage::Chat);
	msgout.serial(const_cast<string&>(msg));
	CNetwork::instance().send(msgout);
}

//
// Commands
//
