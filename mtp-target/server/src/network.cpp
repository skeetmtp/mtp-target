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
#include <nel/net/buf_server.h>
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
using namespace NLNET;
using namespace NLMISC;


//
// Variables
//

#if OLD_NETWORK
CSynchronized<PauseFlags> networkPauseFlags("networkPauseFlags");
void checkNetworkPaused();
#endif // OLD_NETWORK


//
// Thread
//

#if OLD_NETWORK
CNetworkTask::CNetworkTask()
{
	ListenSock.init(NLNET::IService::getInstance()->ConfigFile.getVar("TcpPort").asInt());

#if defined NL_OS_UNIX
	nice(2);
#endif // NL_OS_UNIX
}

void CNetworkTask::run()
{
	NLNET::SOCKET descmax;
	fd_set readers;
	timeval tv;

	NetworkThreadId = myGetThreadId();
	while(true)
	{
		{
			CSynchronized<PauseFlags>::CAccessor acces(&networkPauseFlags);
			acces.value().ackPaused = true;
		}
		
		descmax = ListenSock.descriptor();
		FD_ZERO(&readers);
		FD_SET(ListenSock.descriptor(), &readers);

		{
			for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
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

		checkNetworkPaused();
		
		if(FD_ISSET(ListenSock.descriptor(), &readers) != 0)
		{
			nlinfo("New client connected");

			NLNET::CTcpSock *sock = ListenSock.accept();
			sock->setNoDelay(true);
			sock->setNonBlockingMode(false);

			CEntityManager::instance().addClient(sock);
			//we must flush list now to be ready to receive incoming message from this client
			CEntityManager::instance().flushAddRemoveList();
		}
		

		{
			for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
			{
				nlassert(*it);
				CClient *c = (CClient *)(*it);
				
				if((*it)->type() != CEntity::Client || CEntityManager::instance().inRemoveList(c->id()))
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
					CEntityManager::instance().remove(c->id());
					break;
				default:
					nlwarning("Received failed from client eid %hu : %s (code %u)", (uint16)c->id(), NLNET::CSock::errorString(NLNET::CSock::getLastError()).c_str(), NLNET::CSock::getLastError());
					CEntityManager::instance().remove(c->id());
					break;
				}
			}
		}

		//TODO SKEET : check why we cannot let only main thread loop do the flush job ?
		//CEntityManager::instance().flushAddRemoveList();
		
	}
}
#endif // OLD_NETWORK


	

//
// Variables
//



//
// Functions
//

void cbDisconnection(TSockId from, void *arg)
{
	for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
	{
		nlassert(*it);
		if((*it)->type() != CEntity::Client)
			continue;
		CClient *c = (CClient *)(*it);
		nlassert(c->sock());
			
		if(c->sock() != from)
			continue;
		CEntityManager::instance().remove(c->id());
		break;
	}
}

void cbConnection(TSockId from, void *arg)
{
	// new player comes in
	CEntityManager::instance().addClient(from);
#if OLD_NETWORK
	//we must flush list now to be ready to receive incoming message from this client
	CEntityManager::instance().flushAddRemoveList();
#endif // OLD_NETWORK
}

CNetwork::CNetwork() :
#if OLD_NETWORK
	NetworkTask(0), NetworkThread(0)
#else
	BufServer(0)
#endif // OLD_NETWORK
{
	Version = 0;
}

void CNetwork::init()
{
#if OLD_NETWORK
	nlassert(!NetworkTask);
	nlassert(!NetworkThread);

	NetworkTask = new CNetworkTask();
	nlassert(NetworkTask);

	NetworkThread = NLMISC::IThread::create(NetworkTask);
	nlassert(NetworkThread);

	NetworkThread->start();
#else
	nlassert(!BufServer);

	BufServer = new CBufServer();
	BufServer->init(NLNET::IService::getInstance()->ConfigFile.getVar("TcpPort").asInt());
	BufServer->setConnectionCallback(cbConnection, 0);
	BufServer->setDisconnectionCallback(cbDisconnection, 0);
#endif // OLD_NETWORK

	updateCount = 0;

	MinDeltaToSendFullUpdate = NLNET::IService::getInstance()->ConfigFile.getVar("MinDeltaToSendFullUpdate").asFloat();
	DisableNetworkOptimization = NLNET::IService::getInstance()->ConfigFile.getVar("DisableNetworkOptimization").asInt()!=0;
	Version = NLNET::IService::getInstance()->ConfigFile.getVar("NetworkVersion").asInt();
}

void CNetwork::reset()
{
	updateCount = 0;	
}


//static FILE *fp = 0;
//static int rpos;
//static int updateCount2 = 0;

void CNetwork::update()
{
	static uint8 pingnb = 0;

#if OLD_NETWORK
	if(CEntityManager::instance().humanClientCount()==0) return;
	//	if(updateCount2==0)
	//	{
	//		fp = fopen("net.data","wb");
	//		rpos = 0;//rand()%10;
	//	}
	//	updateCount2++;
#else
	BufServer->update();
	while(BufServer->dataAvailable())
	{
		TSockId sockid;
		CNetMessage msg(CNetMessage::Unknown, true);
		BufServer->receive(msg, &sockid);

		try
		{
			uint32 nbs = 0;
			msg.serial(nbs);

			uint8 t = 0;
			msg.serial(t);
			msg.type((CNetMessage::TType)t);

			{
				for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
				{
					nlassert(*it);
					CClient *c = (CClient *)(*it);
					
#if OLD_NETWORK
					if((*it)->type() != CEntity::Client || CEntityManager::instance().inRemoveList(c->id()))
						continue;
#else
					if((*it)->type() != CEntity::Client)
						continue;
#endif // OLD_NETWORK

					nlassert(c->sock());

					if(c->sock() != sockid)
						continue;

					netCallbacksHandler(c, msg);
				}
			}
			
			//TODO SKEET : check why we cannot let only main thread loop do the flush job ?
			//CEntityManager::instance().flushAddRemoveList();
		}
		catch(Exception &e)
		{
			nlwarning("Malformed Message type '%hu' : %s", (uint16)msg.type(), e.what());
		}
	}
#endif // OLD_NETWORK

	{
		{
			TTime currentTime = CTime::getLocalTime();
			unsigned int i;
			CEntityManager::EntityConstIt it;
			for(i=0,it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++,i++)
			{
				const dReal *pos = dBodyGetPosition((*it)->Body);
				CVector npos;
				
				npos.x = (float)pos[0];
				npos.y = (float)pos[1]; 
				npos.z = (float)pos[2];
				
				uint8 eid = (*it)->id();
				uint16 ping = (*it)->Ping.getSmoothValue();
				//msgout.serial(eid);
				
				CVector dpos = npos - (*it)->LastSent2OthersPos;
				if(dpos.norm()>MinDeltaToSendFullUpdate)
				{
					updateCount=0;
					break;
				}
			}
		}

		if((updateCount%MT_NETWORK_FULL_UPDATE_PERIODE)==0 || DisableNetworkOptimization)
	//	if(updateCount==0)
		{
			CNetMessage msgout(CNetMessage::FullUpdate);
			
			{
				msgout.serial(pingnb);

				TTime currentTime = CTime::getLocalTime();
				
				for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
				{
					const dReal *pos = dBodyGetPosition((*it)->Body);
					
					(*it)->Pos.x = (float)pos[0];
					(*it)->Pos.y = (float)pos[1]; 
					(*it)->Pos.z = (float)pos[2];
					
					uint8 eid = (*it)->id();
					uint16 ping = (*it)->Ping.getSmoothValue();
					msgout.serial(eid);
					msgout.serial((*it)->Pos, ping);
					
					if((*it)->type() != CEntity::Bot)
					{
						(*it)->LastSentPing.push(make_pair(pingnb, currentTime));
						nlinfo("*********** send the ping %u %"NL_I64"u", pingnb, currentTime);
					}
					
					(*it)->LastSent2MePos = (*it)->Pos;
					(*it)->LastSent2OthersPos = (*it)->Pos;
				}
			}
			
			UpdatePacketSize = msgout.length();
			
			CNetwork::instance().send(msgout);
			pingnb++;
		}
		else if((updateCount%MT_NETWORK_MY_UPDATE_FREQUENCE_RATIO)==0)
		{
			CNetMessage msgout(CNetMessage::Update);
			
			{
				msgout.serial(pingnb);
				
				TTime currentTime = CTime::getLocalTime();
				
				for(list<uint8>::iterator it1=CEntityManager::instance().IdUpdateList.begin();it1!=CEntityManager::instance().IdUpdateList.end();it1++)
				{
					uint8 eid = *it1;
					for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
					{
						if((*it)->id()==eid)
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
							{
								(*it)->LastSentPing.push(make_pair(pingnb, currentTime));
								nlinfo("*********** send the ping %u %"NL_I64"u", pingnb, currentTime);
							}

							CVector newPos = (*it)->LastSent2OthersPos + sendDPos; 
							(*it)->LastSent2OthersPos = newPos;
							(*it)->LastSent2MePos = newPos;
						}
					}
				}
			}
			
			CNetwork::instance().send(msgout);
			UpdatePacketSize = msgout.length();
			pingnb++;
		}
		else
		{
			
			TTime currentTime = CTime::getLocalTime();
			
			for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
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
	}
	updateCount++;
}


void CNetwork::release()
{
//	if(fp)
//		fclose(fp);
#if OLD_NETWORK
	if(!NetworkThread || !NetworkTask)
		return;

	NetworkThread->terminate();
	delete NetworkThread;
	NetworkThread = 0;
	delete NetworkTask;
	NetworkTask = 0;
#else
	if(BufServer)
	{
		delete BufServer;
		BufServer = 0;
	}
#endif // OLD_NETWORK
	CNetwork::uninstance();
}


void CNetwork::send(uint8 eid, CNetMessage &msg, bool checkReady)
{
	for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
	{
		if((*it)->id() == eid)
		{
			if((*it)->type() != CEntity::Client)
				nlwarning("(*it)->type() != CEntity::Client (%s)",(*it)->name().c_str());
			//nlassert((*it)->type() == CEntity::Client);
			CClient *c = (CClient *)(*it);
			if(!checkReady || c->networkReady())
			{
				bool sok = msg.send(BufServer, c->sock());
				if(!sok)
					CEntityManager::instance().remove(c->id());
			}
		}
	}
}

void CNetwork::send(CNetMessage &msg)
{
	for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
	{
		if((*it)->type() == CEntity::Client)
		{
			CClient *c = (CClient *)(*it);
			if(c->networkReady())
			{
				bool sok = msg.send(BufServer, c->sock());
				if(!sok)
					CEntityManager::instance().remove(c->id());
			}
		}
	}
}

void CNetwork::sendAllExcept(uint8 eid, CNetMessage &msg)
{
	for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
	{
		if((*it)->id() != eid && (*it)->type() == CEntity::Client)
		{
			CClient *c = (CClient *)(*it);
			if(c->networkReady())
			{
				bool sok = msg.send(BufServer, c->sock());
				if(!sok)
					CEntityManager::instance().remove(c->id());
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

void CNetwork::sendChat(uint8 eid,const string &msg)
{
	CNetMessage msgout(CNetMessage::Chat);
	msgout.serial(const_cast<string&>(msg));
	CNetwork::instance().send(eid,msgout);
}


void checkNetworkPaused()
{
#if OLD_NETWORK
	{
		bool pause;
		{
			CSynchronized<PauseFlags>::CAccessor acces(&networkPauseFlags);
			pause = acces.value().pauseCount>0;
			if(pause)
				acces.value().ackPaused = true;
		}
		while (pause) 
		{
			nlSleep(10);
			{
				CSynchronized<PauseFlags>::CAccessor acces(&networkPauseFlags);
				pause = acces.value().pauseCount>0;
				if(pause)
					acces.value().ackPaused = true;
			}
		}
	}
	{
		CSynchronized<PauseFlags>::CAccessor acces(&networkPauseFlags);
		acces.value().ackPaused = false;
	}	
#endif // OLD_NETWORK
}

bool pauseNetwork(bool waitAck)
{
#if OLD_NETWORK
	bool pause;
	{
		CSynchronized<PauseFlags>::CAccessor acces(&networkPauseFlags);
		pause = acces.value().pauseCount>0;
	}
	if(!pause) 
	{
		bool ackPaused;
		{
			CSynchronized<PauseFlags>::CAccessor acces(&networkPauseFlags);
			ackPaused = false;
			acces.value().pauseCount++;
		}
		if(!waitAck) return true;
		while(!ackPaused)
		{
			nlSleep(10);
			{
				CSynchronized<PauseFlags>::CAccessor acces(&networkPauseFlags);
				ackPaused = acces.value().ackPaused;
			}
		}
	}
	else
		return false;
#endif // OLD_NETWORK
	return true;
}

bool isNetworkPaused()
{
#if OLD_NETWORK
	bool ackPaused;
	{
		CSynchronized<PauseFlags>::CAccessor acces(&networkPauseFlags);
		ackPaused = acces.value().ackPaused;
	}
	return ackPaused;
#else
	return true;
#endif // OLD_NETWORK
}

void resumeNetwork()
{
#if OLD_NETWORK
	CSynchronized<PauseFlags>::CAccessor acces(&networkPauseFlags);
	if(acces.value().pauseCount>0) 
	{
		acces.value().pauseCount--;
		nlassert(acces.value().pauseCount>=0);
	}	
#endif // OLD_NETWORK
}
