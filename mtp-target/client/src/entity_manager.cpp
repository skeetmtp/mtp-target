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

#include "main.h"
#include "time_task.h"
#include "mtp_target.h"
#include "entity_manager.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;


//
// Variables
//


//
// Functions
//
CEntityManager::CEntityManager()
{
	entities().clear();
	for(uint i = 0; i < 256; i++)
	{
		entities().push_back(new CEntity);
	}
}

/*
void CEntityManager::add(uint8 eid)
{
	uint tid = getThreadId();
	nlassert(tid==TaskManagerThreadId || tid==NetworkThreadId);
	if(tid==TaskManagerThreadId)
		ClientToAddTaskManagerThread.push_back(eid);
	else
		ClientToAddNetworkThread.push_back(eid);
}

void CEntityManager::remove(uint8 eid)
{
	uint tid = getThreadId();
	nlassert(tid==TaskManagerThreadId || tid==NetworkThreadId);
	if(tid==TaskManagerThreadId)
		ClientToRemoveTaskManagerThread.push_back(eid);
	else
		ClientToRemoveNetworkThread.push_back(eid);
	
}
*/

void CEntityManager::init()
{
	for(uint i = 0; i < 256; i++)
	{
		entities()[i]->id(i);
		entities()[i]->reset();
	}
	updateListId.clear();

	ClientToAddTaskManagerThread.clear();
	ClientToAddNetworkThread.clear();
	ClientToRemoveTaskManagerThread.clear();
	ClientToRemoveNetworkThread.clear();
}

void CEntityManager::update()
{
	// do some security check
	for(uint i = 0; i < 256; i++)
	{
		nlassert(entities()[i]->Id == i);
	}
	nlassert(entities()[255]->Type == CEntity::Unknown);

	for(uint i = 0; i < 256; i++)
	{
		if(entities()[i]->type() != CEntity::Unknown)
			entities()[i]->update();
	}
}

void CEntityManager::add(uint8 eid, const std::string &name, sint32 totalScore, CRGBA &color, bool spectator, bool isLocal)
{
	nlassert(!exist(eid));
	uint tid = getThreadId();
	nlassert(tid==TaskManagerThreadId || tid==NetworkThreadId);
	
	CEntityInitData entityToAdd(eid,name,totalScore,color,spectator,isLocal);
	std::list<CEntityInitData> *ClientToAddList;
	if(tid==TaskManagerThreadId)
		ClientToAddList = &ClientToAddTaskManagerThread;
	else
		ClientToAddList = &ClientToAddNetworkThread;

	list<CEntityInitData>::iterator it2;
	for(it2=ClientToAddList->begin(); it2!=ClientToAddList->end();it2++)
	{
		CEntityInitData e = *it2;
		if(e.eid==eid)
		{
			nlwarning("client %d %s still in add list",eid,name.c_str());
			return;
		}
	}
	ClientToAddList->push_back(entityToAdd);
}

void CEntityManager::remove(uint8 eid)
{
	nlassert(exist(eid));
	uint tid = getThreadId();
	nlassert(tid==TaskManagerThreadId || tid==NetworkThreadId);
	std::list<uint8> *ClientToRemoveList;
	if(tid==TaskManagerThreadId)
		ClientToRemoveList = &ClientToRemoveTaskManagerThread;
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

void CEntityManager::_add(std::list<CEntityInitData> &addList)
{
	list<CEntityInitData>::iterator it2;
	for(it2=addList.begin(); it2!=addList.end();it2++)
	{
		CEntityInitData e = *it2;
		nlassert(!exist(e.eid));
		entities()[e.eid]->init(CEntity::Player, e.name, e.totalScore, e.color, "pingoo", e.spectator,e.isLocal);
		nlinfo("CEntityManager::add(%d,%s)",e.eid,e.name.c_str());
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
		
		nlassert(exist(eid));
		if(CMtpTarget::instance().controler().Camera.getFollowedEntity() == eid)
			CMtpTarget::instance().controler().Camera.setFollowedEntity(255);
		
		entities()[eid]->reset();
	}
	removeList.clear();

}


void CEntityManager::flushAddRemoveList()
{
	uint tid = getThreadId();
	nlassert(tid==TaskManagerThreadId || tid==NetworkThreadId);
	
	if(tid==TaskManagerThreadId)
	{
		if(ClientToAddTaskManagerThread.size()==0 && ClientToRemoveTaskManagerThread.size()==0)
			return;
		bool paused = pauseAllThread();
		if(!paused)
			return;
		_add(ClientToAddTaskManagerThread);
		_remove(ClientToRemoveTaskManagerThread);
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
		resumeAllThread();
	}		
	
}



bool CEntityManager::exist(uint8 eid)
{
	nlassert(eid != 255);
	return entities()[eid]->Type != CEntity::Unknown;
}

CEntity &CEntityManager::operator [](uint8 eid)
{
	nlassert(exist(eid));
	return *entities()[eid]; //todo prevent external code to access entites without using accessor
}

void CEntityManager::getEIdSortedByScore(vector<uint8> &eids) 
{
	eids.clear();

	for (uint i = 0; i < 255; i++)
	{
		if (entities()[i]->type() == CEntity::Unknown)
			continue;

		eids.push_back(i);
	}
}

uint8 CEntityManager::findFirstEId() 
{
	return findNextEId(255);
}


uint8 CEntityManager::findNextEId(uint8 eid) 
{
	uint8 neid = eid;
	while(++neid != eid)
		if(entities()[neid]->type() != CEntity::Unknown)
			return neid;
	return eid;
}

uint8 CEntityManager::findPreviousEId(uint8 eid) 
{
	uint8 neid = eid;
	while(--neid != eid)
		if(entities()[neid]->type() != CEntity::Unknown)
			return neid;
	return 255;
}

void CEntityManager::startSession()
{
	for(uint i = 0; i < 256; i++)
	{
		if(entities()[i]->type() != CEntity::Unknown)
		{
			entities()[i]->close();
		}
	}
}

void CEntityManager::reset()
{
	for(uint i = 0; i < 256; i++)
	{
		if(entities()[i]->type() != CEntity::Unknown)
		{
			entities()[i]->interpolator().reset();
		}
	}
}

void CEntityManager::resetSpectator()
{
	for(uint i = 0; i < 256; i++)
	{
		if(entities()[i]->type() != CEntity::Unknown)
		{
			entities()[i]->spectator(false);
		}
	}
}

void CEntityManager::everybodyReady(bool state)
{
	for(uint i = 0; i < 256; i++)
	{
		if(entities()[i]->type() != CEntity::Unknown)
		{
			entities()[i]->ready(state);
		}
	}
}

uint8 CEntityManager::size()
{
	uint8 nb = 0;
	for(uint i = 0; i < 256; i++)
	{
		if(entities()[i]->type() != CEntity::Unknown)
		{
			nb++;
		}
	}
	return nb;
}

void CEntityManager::renderNames()
{
	for(uint i = 0; i < 256; i++)
	{
		if(entities()[i]->type() != CEntity::Unknown)
			entities()[i]->renderName();
	}	
}

void CEntityManager::render()
{
}

void CEntityManager::load3d()
{
	for(uint i = 0; i < 256; i++)
	{
		if(entities()[i]->type() != CEntity::Unknown && !entities()[i]->spectator())
			entities()[i]->load3d();
	}
}

//
// Commands
//
