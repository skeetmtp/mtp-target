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

#include <nel/net/service.h>

#include "bot.h"
#include "main.h"
#include "client.h"
#include "network.h"
#include "variables.h"
#include "level_manager.h"
#include "entity_manager.h"
#include "session_manager.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;
using namespace NLNET;


//
// Types
//


//
// Declarations
//



//
// Variables
//

static uint8 WatchingId = 0;

//
// Functions
//

void CEntityManager::init()
{
	IdUpdateList.clear();
}

void CEntityManager::update()
{
	checkForcedClientCount();
	{
		EntityConstIt it;
		for(it = entities().begin(); it != entities().end(); it++)
		{
			(*it)->update();
		}
	}
}

void CEntityManager::add(CEntity *entity)
{
	uint tid = getThreadId();
	nlassert(tid==MainThreadId || tid==NetworkThreadId);
	std::list<CEntity *> *ClientToAddList;
	
	if(tid==MainThreadId)
		ClientToAddList = &ClientToAddMainThread;
	else
		ClientToAddList = &ClientToAddNetworkThread;
	
	list<CEntity *>::iterator it2;
	for(it2=ClientToAddList->begin(); it2!=ClientToAddList->end();it2++)
	{
		CEntity *e = *it2;
		if(e==entity)
		{
			nlwarning("client 0x%p %d %s still in add list",e,e->id(),e->name().c_str());
			return;
		}
	}
	
	ClientToAddList->push_back(entity);
}

void CEntityManager::remove(uint8 eid)
{
	uint tid = getThreadId();
	nlassert(tid==MainThreadId || tid==NetworkThreadId);
	std::list<uint8> *ClientToRemoveList;

	if(tid==MainThreadId)
		ClientToRemoveList = &ClientToRemoveMainThread;
	else
		ClientToRemoveList = &ClientToRemoveNetworkThread;

	list<uint8>::iterator it1;
	for(it1=ClientToRemoveList->begin(); it1!=ClientToRemoveList->end();it1++)
	{
		uint8 iteid = *it1;
		if(iteid==eid)
		{
			nlwarning("client %d still in remove list",eid);
			return;			
		}

	}
	
	ClientToRemoveList->push_back(eid);

}

void CEntityManager::_add(std::list<CEntity *> &addList)
{
	list<CEntity *>::iterator it2;
	for(it2=addList.begin(); it2!=addList.end();it2++)
	{
		CEntity *e = *it2;
		entities().push_back(e);
	}
	addList.clear();	
	
}

void CEntityManager::_remove(std::list<uint8> &removeList)
{
	list<uint8>::iterator it1;
	for(it1=removeList.begin(); it1!=removeList.end();it1++)
	{
		uint8 eid = *it1;
		if(eid == 255)
			nlwarning("Can't remove client because eid 255 is not valid");
		
		CEntity *c = 0;
		
		{
			EntityIt it;
			for( it = entities().begin(); it != entities().end(); it++)
			{
				if((*it)->id() == eid)
				{
					// only unlink the client from the list
					c = (*it);
					entities().erase(it);
					break;
				}
			}
		}
		
		if(!c)
		{
			nlwarning("Can't remove client because eid %hu is not found", (uint16)eid);
		}
		else
		{
			nlinfo("Removing client eid %hu name '%s'", (uint16)c->id(), c->name().c_str());
			CSessionManager::instance().editMode(0);
			
			// TODO clientConnected(c->Cookie, false);
			
			CNetMessage msgout(CNetMessage::Logout);
			msgout.serial(eid);
			CNetwork::instance().send(msgout);		
			delete c;
		}
		
	}
	removeList.clear();
	
}




void CEntityManager::flushAddRemoveList()
{
	uint tid = getThreadId();
	nlassert(tid==MainThreadId || tid==NetworkThreadId);
	
	if(tid==MainThreadId)
	{
		if(ClientToAddMainThread.size()==0 && ClientToRemoveMainThread.size()==0)
			return;
		bool paused = pauseAllThread();
		if(!paused)
			return;
		_add(ClientToAddMainThread);
		_remove(ClientToRemoveMainThread);
		updateIdUpdateList();
		resumeAllThread();
	}
	else
	{
		if(ClientToAddNetworkThread.size()==0 && ClientToRemoveNetworkThread.size()==0)
			return;
		bool paused = pauseAllThread();
		if(!paused)
			return;
		_add(ClientToAddNetworkThread);
		_remove(ClientToRemoveNetworkThread);
		updateIdUpdateList();
		resumeAllThread();
	}		
	
}


void CEntityManager::reset()
{
	EntityConstIt it;
	for(it = entities().begin(); it != entities().end(); it++)
	{
		(*it)->reset();
	}
}

void CEntityManager::release()
{
	CNetwork::instance().release();
}

uint8 CEntityManager::findNewId()
{
	uint8 ni = 0;
	uint8 baseId = 0;
	uint tid = getThreadId();
	nlassert(tid==MainThreadId || tid==NetworkThreadId);

	if(tid==NetworkThreadId)
		baseId=128;

	bool nidFound = false;
	for(ni=baseId;ni<baseId+127;ni++)
	{
		bool nidOk = true;
		EntityConstIt it;
		for(it = entities().begin(); it != entities().end() && nidOk; it++)
		{
			if((*it)->id() == ni)
				nidOk = false;
		}
		if(tid==MainThreadId)
		{
			list<CEntity *>::iterator it2;
			for(it2=ClientToAddMainThread.begin(); it2!=ClientToAddMainThread.end() && nidOk;it2++)
			{
				CEntity *e = *it2;
				if(e->id()==ni)
					nidOk = false;
			}
		}
		else
		{
			list<CEntity *>::iterator it2;
			for(it2=ClientToAddNetworkThread.begin(); it2!=ClientToAddNetworkThread.end() && nidOk;it2++)
			{
				CEntity *e = *it2;
				if(e->id()==ni)
					nidOk = false;
			}			
		}
		if(nidOk)
			return ni;
	}
	nlerror("More than 127 entities, can't add a new one");

	/*
	while(true)
	{
		EntityConstIt it;
		for(it = entities().begin(); it != entities().end(); it++)
		{
			if((*it)->id() == ni)
				break;
		}
		if(it == entities().end())
			return ni;
		ni++;
		if(ni == 255)
		{
			nlerror("More than 254 entities, can't add a new one");
		}	
	}
	*/
	return 255;
}

void CEntityManager::addBot(const string &name, bool isAutomaticBot)
{
	uint8 eid = findNewId();
	CEntity *e = (CEntity *) new CBot(eid, name, isAutomaticBot);
	nlassert(e);

	nlinfo("Adding bot eid %hu name '%s'", (uint16)e->id(), e->name().c_str());

	uint8 nbc = nbEntities();
	if(nbc >= NbMaxClients)
	{
		nlinfo("cannot add bot , too much client(%d)",nbc);
		return;
	}
		
	
	add(e);

	login(e);
}



void CEntityManager::addClient(NLNET::CTcpSock *sock)
{
	nlassert(sock);

	uint8 eid = findNewId();
	nlassert(eid != 255);

	nlinfo("Adding client eid %hu from '%s'", (uint16)eid, sock->remoteAddr().asString().c_str());

	CEntity *e = (CEntity *) new CClient(eid, sock);
	nlassert(e);

	add(e);
}


void CEntityManager::updateIdUpdateList()
{
	int minId = 0;
	IdUpdateList.clear();
	for(int i=0;i<255;i++)
	{
		for(CEntityManager::EntityConstIt it = entities().begin(); it != entities().end(); it++)
		{
			uint8 eid = (*it)->id();
			if(eid==i)
				IdUpdateList.push_back(eid);
		}			
	}
}


void CEntityManager::login(CEntity *e)
{
	bool self;
	uint8 nid = e->id();
	string n = e->name();
	
	// TODO clientConnected(c->Cookie, true);
	
	{
		// send info about the new client to everybody but not the new client
		CNetMessage msgout(CNetMessage::Login);
		self = false;
		msgout.serial(self);
		msgout.serial(nid);
		msgout.serial(n);
		msgout.serial(e->Score);
		msgout.serial(e->Color);
		msgout.serial(e->Texture);
		bool s = e->spectator();
		msgout.serial(s);
		CNetwork::instance().sendAllExcept(nid, msgout);
	}

	if(e->type() == CEntity::Client)
	{
		// send the new id for the new client
		CNetMessage msgout(CNetMessage::Login);
		self = true;
		msgout.serial(self);
		msgout.serial(nid);
		msgout.serial(n);
		msgout.serial(e->Score);
		msgout.serial(e->Color);
		msgout.serial(e->Texture);
		bool s = e->spectator();
		msgout.serial(s);
		string CurrentLevel;
		if(CLevelManager::instance().haveCurrentLevel())
			CLevelManager::instance().levelName();
		msgout.serial(CurrentLevel);
		float timeBeforeTimeout;
		if (CSessionManager::instance().startTime() == 0)
		{
			timeBeforeTimeout = 0;
		}
		else
		{
			timeBeforeTimeout = (float)((TTime)TimeTimeout - CTime::getLocalTime()-CSessionManager::instance().startTime())/1000.0f;
		}
		msgout.serial(timeBeforeTimeout);
		CNetwork::instance().send(nid, msgout);
		
		// send the welcome message to the new client
		CNetMessage msgout2(CNetMessage::Chat);
		CConfigFile::CVar &welcome = IService::getInstance()->ConfigFile.getVar("WelcomeMessage");
		for(sint i = 0; i < welcome.size(); i++)
		{
			string str = welcome.asString(i);
			msgout2.serial(str);
		}
		CNetwork::instance().send(nid, msgout2);
		
		// send all entities informations to the new client
		{
			self = false;
			nlinfo ("There's %d people", entities().size());
			for(EntityConstIt it = entities().begin(); it != entities().end(); it++)
			{
				if((*it) != e)
				{
					msgout.clear();
					msgout.setHeader(CNetMessage::Login);
					msgout.serial(self);
					uint8 eid = (*it)->id();
					msgout.serial(eid);
					string name = (*it)->name();
					msgout.serial(name);
					msgout.serial((*it)->Score);
					msgout.serial((*it)->Color);
					msgout.serial((*it)->Texture);
					bool s = e->spectator();
					msgout.serial(s);
					CNetwork::instance().send(nid, msgout);
				}
			}
		}
		
		// now the client can receive position update
		((CClient*)e)->networkReady(true);
	}
	updateIdUpdateList();
}

CEntity *CEntityManager::getByName(const std::string &name)
{
	CEntity *res = NULL;
	for(EntityConstIt it = entities().begin(); it != entities().end(); it++)
	{
		if((*it)->name() == name)
		{
			res = *it;
			break;
		}
	}
	return res;
}


void CEntityManager::remove(const string &name)
{
	if(name.empty())
	{
		nlwarning("Can't remove a client with empty name");
		return;
	}

	/*
	uint8 eid = 0;
	{
		CEntities::CReadAccessor acces(entities());
		for(EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
		{
			if((*it)->name() == name)
			{
				eid =(*it)->id();
				break;
			}
		}
	}
	*/
	CEntity *e = getByName(name);
	if(e)
		remove(e->id());
	else
		nlwarning("Can't remove a client %s : not found",name.c_str());
}

CEntity *CEntityManager::getById(uint8 eid)
{
	if(eid == 255)
	{
		nlwarning("Can't find client because eid 255 is not valid");
		return NULL;
	}
	
	CEntity *res = 0;
	
	{
		EntityIt it;
		for( it = entities().begin(); it != entities().end(); it++)
		{
			if((*it)->id() == eid)
			{
				// only unlink the client from the list
				res = (*it);
				return res;
			}
		}
	}
	
	nlwarning("Can't find client eid %hu ", (uint16)eid);
	return NULL;
	
}

void CEntityManager::removeBot()
{
	remove("bot");
}

void CEntityManager::checkForcedClientCount()
{
	uint clientCount;
	{
		clientCount = entities().size();
	}

	if(clientCount < ForcedClientCount)
	{
		uint newAutomaticBotCount = ((uint)ForcedClientCount) - clientCount;
		for(uint i=0;i<newAutomaticBotCount;i++)
			addBot("", true);
	}
	else
	{
		uint oldAutomaticBotCount = clientCount - ((uint)ForcedClientCount);
		bool botFound = true;
		while(botFound && oldAutomaticBotCount)
		{
			botFound = false;
			uint8 oldBotId = 0;
			{
				for(EntityConstIt it = entities().begin(); it != entities().end(); it++)
				{
					if((*it)->type() == CEntity::Bot)
					{
						CBot *b = (CBot *) (*it);
						if (b->isAutomaticBot())
						{
							nlinfo("Removing automatic bot : %s", b->name().c_str());
							oldBotId = b->id();
							botFound = true;
							break;
						}
					}
				}
			}
			if(botFound)
			{
				remove(oldBotId);
				oldAutomaticBotCount--;
			}
		}
	}
}

void CEntityManager::openClose(uint8 eid)
{
	bool found = false;
	for(EntityConstIt it = entities().begin(); it != entities().end(); it++)
	{
		if((*it)->id() == eid)
		{
			(*it)->openClose();
			found = true;
			break;
		}
	}

	if(found)
	{
		// send the new state to all entities
		CNetMessage msgout(CNetMessage::OpenClose);
		msgout.serial(eid);
		CNetwork::instance().send(msgout);
	}
}
 
uint8 CEntityManager::nbEntities()
{
	uint8 nb;
	{
		nb = entities().size();
	}
	return nb;
}

bool CEntityManager::connected(const std::string &name)
{
	for(EntityConstIt it = entities().begin(); it != entities().end(); it++)
	{
		if((*it)->name() == name)
		{
			(*it)->openClose();
			return true;
		}
	}
	return false;
}

string CEntityManager::check(const string &login, const string &password, bool dontCheck, sint32 &score)
{
	score = 0;

	nlinfo("Check new client '%s'", login.c_str());

	if(login.empty())
		return "Bad login, it must not be empty";
	
	if(!dontCheck && password.empty())
		return "Bad password, it must not be empty";
	
	if(login.size() > 20)
		return "Bad login, it must be <= 20 char";
	
	if(!dontCheck && password.size() > 20)
		return "Bad password, it must be <= 20 char";
	
	uint8 nbc = nbEntities();
	
	// check if server not full
	if(nbc > NbMaxClients)
	{
		return "Server full, "+toString(nbc)+" clients connected";
	}
	
	CConfigFile::CVar &accounts = IService::getInstance()->ConfigFile.getVar("Accounts");
	
	for(uint i = 0; i < (uint)accounts.size(); i += 3)
	{
		if(accounts.asString(i) == login)
		{
			if(dontCheck || accounts.asString(i+1) == password)
			{
				// check if not already online
				if(connected(login))
				{
					return "Client "+login+" already online";
				}

				score = accounts.asInt(i+2);

				return "";
			}
			else
			{
				return "Bad password";
			}
		}
	}

	// unknown user, add it
	accounts.setAsString(login, accounts.size());
	accounts.setAsString(password, accounts.size());
	accounts.setAsString("0", accounts.size());
	IService::getInstance()->ConfigFile.save();

	return "";
}


//
// Commands
//

NLMISC_COMMAND(addBot, "add a bot", "[<name>]")
{
	CEntityManager::instance().addBot((args.size() > 0) ? args[0] : "bot", false);
	return true;
}
 
NLMISC_COMMAND(displayEntities, "display info about all entities", "")
{
	if(args.size() != 0) return false;
	
	log.displayNL("Displaying %d entities:", CEntityManager::instance().entities().size());
	for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
	{
		(*it)->display(log);
	}
	return true;
}

NLMISC_COMMAND(kick, "kick a client from the server", "[<eid>|<name>]")
{
	if(args.size() != 1) return false;
	
	nlinfo("kick %s", args[0].c_str());
	
	uint8 val = atoi(args[0].c_str());
	if(val>0)
		CEntityManager::instance().remove(val);
	else
	{
		CEntityManager::instance().remove(args[0]);
		CEntity *e = CEntityManager::instance().getByName(args[0]);
		if(e && e->type() == CEntity::Client)
			val = e->id();
		else
			val = 255;
	}

	if(val!=255)
	{
		string reason = toString("You have been kicked !");
		CNetMessage msgout(CNetMessage::Error);
		msgout.serial(reason);
		CNetwork::instance().send(val, msgout);
	}
	
	return true;
}

NLMISC_COMMAND(watch, "watch a client", "[<eid>]")
{
	if(args.size() > 1) return false;
	WatchingId = (args.size() == 0) ? 0 : atoi(args[0].c_str());
	return true;
}

NLMISC_DYNVARIABLE(uint32, NbEntities, "Number of entities (player+bot)")
{
	if(!get) return;
	
	*pointer = CEntityManager::instance().entities().size();
}

NLMISC_DYNVARIABLE(uint32, NbClients, "Number of players")
{
	if(!get) return;

	uint32 nb = 0;
	for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
	{
		if((*it)->type() == CEntity::Client)
		{
			nb++;
		}
	}
	*pointer = nb;
}

NLMISC_DYNVARIABLE(string, Watch1, "")
{
	if(!get) return;
	if(!WatchingId)
	{
		*pointer = "NoWatch                                                                       ";
		return;
	}

	{
		for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
		{
			if((*it)->id() == WatchingId)
			{
				*pointer  = "Id:"+NLMISC::toString((*it)->id());
				*pointer += " Name:"+(*it)->name();
				if((*it)->type() == CEntity::Bot) *pointer += " Bot";
				else if((*it)->type() == CEntity::Client) *pointer += " Client";
				else *pointer += " ???";
				if ((*it)->type() == CEntity::Client)
				{
					*pointer += " TCPFrom:";
					*pointer += ((CClient*)(*it))->sock()->remoteAddr().asString();
				}
				*pointer += " Ping:"+NLMISC::toString((*it)->Ping.getSmoothValue());
				return;
			}
		}
		*pointer = "Entity "+NLMISC::toString(WatchingId)+" not found                                       ";
	}
}

NLMISC_DYNVARIABLE(string, Watch2, "")
{
	if(!get) return;
	if(!WatchingId)
	{
		*pointer = "NoWatch                                                                       ";
		return;
	}
	
	{
		for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
		{
			if((*it)->id() == WatchingId)
			{
				*pointer  = "CurScore:"+NLMISC::toString((*it)->CurrentScore);
				*pointer += " TotScore:"+NLMISC::toString((*it)->Score);
				if((*it)->FreezeCommand) *pointer += " FreezeCommand";
				if((*it)->OnTheWater) *pointer += " OnTheWater";
				if((*it)->InGame) *pointer += " InGame";
				else *pointer += " OutSession";
				if((*it)->OpenClose) *pointer += " Open";
				else *pointer += " Close";
				if((*it)->Ready) *pointer += " IsReady";
				else *pointer += " IsNotReady";
				*pointer += " NbOC:"+NLMISC::toString((*it)->NbOpenClose);
				*pointer += " Frict:"+NLMISC::toString((*it)->Friction);
				*pointer += " Accel:"+NLMISC::toString((*it)->Accel);
				
				return;
			}
		}
		*pointer = "Entity "+NLMISC::toString(WatchingId)+" not found                                       ";
	}
}

NLMISC_DYNVARIABLE(string, Watch3, "")
{
	if(!get) return;
	if(!WatchingId)
	{
		*pointer = "NoWatch                                                                       ";
		return;
	}
	
	{
		for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
		{
			if((*it)->id() == WatchingId)
			{
				*pointer += "Pos:("+NLMISC::toString("%.2f",(*it)->Pos.x)+","+NLMISC::toString("%.2f",(*it)->Pos.y)+","+NLMISC::toString("%.2f",(*it)->Pos.z)+")";
				*pointer += " Forc:("+NLMISC::toString("%.2f",(*it)->Force.x)+","+NLMISC::toString("%.2f",(*it)->Force.y)+","+NLMISC::toString("%.2f",(*it)->Force.z)+")";
				*pointer += " "+(*it)->ColState;
				return;
			}
		}
		*pointer = "Entity "+NLMISC::toString(WatchingId)+" not found                                       ";
	}
}


NLMISC_DYNVARIABLE(string, Watch4, "")
{
	if(!get) return;
	if(!WatchingId)
	{
		*pointer = "NoWatch                                                                       ";
		return;
	}
	
	{
		for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
		{
			if((*it)->id() == WatchingId)
			{
				const dReal *vec = dBodyGetPosition((*it)->Body);
				*pointer += "PPos:("+NLMISC::toString("%.2f",vec[0])+","+NLMISC::toString("%.2f",vec[1])+","+NLMISC::toString("%.2f",vec[2])+")";
				vec = dBodyGetLinearVel((*it)->Body);
				*pointer += " PLinVel:("+NLMISC::toString("%.2f",vec[0])+","+NLMISC::toString("%.2f",vec[1])+","+NLMISC::toString("%.2f",vec[2])+")";
				*pointer += " PForce:("+NLMISC::toString("%.2f",vec[0])+","+NLMISC::toString("%.2f",vec[1])+","+NLMISC::toString("%.2f",vec[2])+")";
				vec = dBodyGetTorque((*it)->Body);
				return;
			}
		}
		*pointer = "Entity "+NLMISC::toString(WatchingId)+" not found                                       ";
	}
}

NLMISC_DYNVARIABLE(string, Watch5, "")
{
	if(!get) return;
	if(!WatchingId)
	{
		*pointer = "NoWatch                                                                       ";
		return;
	}
	
	{
		for(CEntityManager::EntityConstIt it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
		{
			if((*it)->id() == WatchingId)
			{
				const dReal *vec = dBodyGetRotation((*it)->Body);
				*pointer += "PRot:("+NLMISC::toString("%.2f",vec[0])+","+NLMISC::toString("%.2f",vec[1])+","+NLMISC::toString("%.2f",vec[2])+")";
				vec = dBodyGetAngularVel((*it)->Body);
				*pointer += " PAngVel:("+NLMISC::toString("%.2f",vec[0])+","+NLMISC::toString("%.2f",vec[1])+","+NLMISC::toString("%.2f",vec[2])+")";
				vec = dBodyGetForce((*it)->Body);
				*pointer += " PTorque:("+NLMISC::toString("%.2f",vec[0])+","+NLMISC::toString("%.2f",vec[1])+","+NLMISC::toString("%.2f",vec[2])+")";
				
				return;
			}
		}
		*pointer = "Entity "+NLMISC::toString(WatchingId)+" not found                                       ";
	}
}


uint CEntityManager::humanClientCount()
{
	CEntityManager::EntityConstIt it;
	
	uint res = 0;
	for(it = entities().begin(); it != entities().end(); it++)
	{
		if((*it)->type() == CEntity::Client)
		{
			res++;
		}
		else if ((*it)->type() == CEntity::Bot)
		{
			CBot *b = (CBot *)(*it);
			if(!b->isAutomaticBot())
				res++;
		}
	}
	return res;
}



bool CEntityManager::nameExist(std::string name)
{
	CEntityManager::EntityConstIt it;

	for(CEntityManager::EntityConstIt it = entities().begin(); it != entities().end(); it++)
	{
		if((*it)->name() == name)
		{
			return true;
		}
	}
	
	return false;
}

void CEntityManager::saveAllValidReplay()
{
	CEntityManager::EntityConstIt it;
	// close all replay file
	for(it = entities().begin(); it != entities().end(); it++)
	{
		if((*it)->type() == CEntity::Client)
		{
			CClient *c = (CClient *)(*it);
			if(c->ReplayFile)
			{
				fprintf(c->ReplayFile, "%d AU %.1f %s %d\n", c->id(), 0.0f, c->name().c_str(), c->CurrentScore);
				fclose(c->ReplayFile);
				c->ReplayFile = 0;
				if(c->CurrentScore <IService::getInstance()->ConfigFile.getVar("SavedReplayMinimumScore").asInt())
					CFile::deleteFile(c->ReplayFilename.c_str());
			}
		}
		//		else
		//			nlinfo("bot do score = %d ",c->CurrentScore);
	}
}


bool CEntityManager::everyBodyReady()
{
	bool res = true;
	vector<uint8> IdToRemove;
	vector<string> msgs;
	{
		CEntityManager::EntityConstIt it;
		
		bool allReady = true;
		// send the message to all entities
		for(it = entities().begin(); it != entities().end(); it++)
		{
			if((*it)->type()==CEntity::Client && (*it)->WaitingReady && !(*it)->Ready )
			{
				if( (float)(CTime::getLocalTime() - (*it)->WaitingReadyTimeoutStart)/1000.0f > 20) //timeout
				{
					IdToRemove.push_back((*it)->id());
					string timeoutMsg = toString("kick %s : wait for ready timeout",(*it)->name().c_str());
					msgs.push_back(timeoutMsg);
				}
				res = false;
				break;
			}
		}	
	}
	for(uint i = 0; i < IdToRemove.size(); i++)
	{
		string reason = "waiting for ready has timed out";
		CNetMessage msgout(CNetMessage::Error);
		msgout.serial(reason);
		CNetwork::instance().send(IdToRemove[i], msgout);

		CEntityManager::instance().remove(IdToRemove[i]);
	}
	IdToRemove.clear();
	for(uint i = 0; i < msgs.size(); i++)
	{
		CNetwork::instance().sendChat(msgs[i]);
	}
	msgs.clear();
	
	return res;
}


float CEntityManager::slowerVelocity()
{

	float res = 1000;
	CEntityManager::EntityConstIt it;
	
	for(it = entities().begin(); it != entities().end(); it++)
	{
		CEntity *c = *it;

		float val = c->meanVelocity();
		{
			if(val<res)
				res = val;
		}
	}
	

	return res;
}
