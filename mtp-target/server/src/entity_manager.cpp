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
	checkForcedClientCount();
}

void CEntityManager::update()
{
	CEntities::CReadAccessor acces(entities());
	EntityConstIt it;
	for(it = acces.value().begin(); it != acces.value().end(); it++)
	{
		(*it)->update();
	}
}

void CEntityManager::reset()
{
	CEntities::CReadAccessor acces(entities());
	EntityConstIt it;
	for(it = acces.value().begin(); it != acces.value().end(); it++)
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
	CEntities::CReadAccessor acces(entities());
	while(true)
	{
		EntityConstIt it;
		for(it = acces.value().begin(); it != acces.value().end(); it++)
		{
			if((*it)->id() == ni)
				break;
		}
		if(it == acces.value().end())
			return ni;
		ni++;
		if(ni == 255)
		{
			nlerror("More than 254 entities, can't add a new one");
		}	
	}
}

void CEntityManager::addBot(const string &name, bool isAutomaticBot)
{
	uint8 eid = findNewId();
	CEntity *e = (CEntity *) new CBot(eid, name, isAutomaticBot);
	nlassert(e);

	nlinfo("Adding bot eid %hu name '%s'", (uint16)e->id(), e->name().c_str());
	
	// add the client in the list
	{
		CEntities::CWriteAccessor acces(entities());
		acces.value().push_back(e);
	}

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

	// add the client in the list
	{
		CEntities::CWriteAccessor acces(entities());
		acces.value().push_back(e);
	}
	checkForcedClientCount();
}



void CEntityManager::sendUpdateList()
{
	CNetMessage msgout(CNetMessage::UpdateList);

	{
		CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
		
		TTime currentTime = CTime::getLocalTime();
		
		for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
		{
			uint8 eid = (*it)->id();
			msgout.serial(eid);
		}
		CNetwork::instance().send(msgout);
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
			CEntities::CReadAccessor acces(entities());
			nlinfo ("There's %d people", acces.value().size());
			for(EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
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
					bool s = e->spectator();
					msgout.serial(s);
					CNetwork::instance().send(nid, msgout);
				}
			}
		}
		// now the client can receive position update
		((CClient*)e)->networkReady(true);
	}

	sendUpdateList();
}

void CEntityManager::remove(const string &name)
{
	if(name.empty())
	{
		nlwarning("Can't remove a client with empty name");
		return;
	}

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
	remove(eid);
}

void CEntityManager::remove(uint8 eid)
{
	if(eid == 255)
	{
		nlwarning("Can't remove client because eid 255 is not valid");
		return;
	}

	CEntity *c = 0;

	{
		CEntities::CWriteAccessor acces(entities());
		for(EntityIt it = acces.value().begin(); it != acces.value().end(); it++)
		{
			if((*it)->id() == eid)
			{
				// only unlink the client from the list
				c = (*it);
				acces.value().erase(it);
				break;
			}
		}
	}

	if(!c)
	{
		nlwarning("Can't remove client because eid %hu is not found", (uint16)eid);
		return;
	}

	nlinfo("Removing client eid %hu name '%s'", (uint16)c->id(), c->name().c_str());
	CSessionManager::instance().editMode(0);
	
	// TODO clientConnected(c->Cookie, false);

	CNetMessage msgout(CNetMessage::Logout);
	msgout.serial(eid);
	CNetwork::instance().send(msgout);

	delete c;

	checkForcedClientCount();
}

void CEntityManager::removeBot()
{
	remove("bot");
}

void CEntityManager::checkForcedClientCount()
{
	uint clientCount;
	{
		CEntities::CReadAccessor acces(entities());
		clientCount = acces.value().size();
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
				CEntities::CReadAccessor acces(entities());
				for(EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
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
	CEntities::CReadAccessor acces(entities());
	for(EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
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
		CEntities::CReadAccessor acces(entities());
		nb = acces.value().size();
	}
	return nb;
}

bool CEntityManager::connected(const std::string &name)
{
	CEntities::CReadAccessor acces(entities());
	for(EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
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
	
	CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
	log.displayNL("Displaying %d entities:", acces.value().size());
	for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
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
		CEntityManager::instance().remove(args[0]);
	
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
	
	CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
	*pointer = acces.value().size();
}

NLMISC_DYNVARIABLE(uint32, NbClients, "Number of players")
{
	if(!get) return;

	uint32 nb = 0;
	CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
	for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
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
		CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
		for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
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
		CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
		for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
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
		CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
		for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
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
		CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
		for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
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
		CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
		for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
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
	CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
	CEntityManager::EntityConstIt it;
	
	uint res = 0;
	for(it = acces.value().begin(); it != acces.value().end(); it++)
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


void CEntityManager::addClientToRemoveList(CClient *c)
{
	c->networkReady(false);
	IdToRemove.push_back(c->id());	
}

void CEntityManager::flushRemoveList()
{
	for(uint i = 0; i < IdToRemove.size(); i++)
	{
		CEntityManager::instance().remove(IdToRemove[i]);
	}
	IdToRemove.clear();	
}
