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
	CEntities::CWriteAccessor acces(entities());
	acces.value().clear();
	for(uint i = 0; i < 256; i++)
	{
		acces.value().push_back(new CEntity);
	}
}

void CEntityManager::init()
{
	CEntities::CReadAccessor acces(entities());
	for(uint i = 0; i < 256; i++)
	{
		acces.value()[i]->id(i);
		acces.value()[i]->reset();
	}
	updateListId.clear();
}

void CEntityManager::update()
{
	// do some security check
	CEntities::CReadAccessor acces(entities());
	for(uint i = 0; i < 256; i++)
	{
		nlassert(acces.value()[i]->Id == i);
	}
	nlassert(acces.value()[255]->Type == CEntity::Unknown);

	for(uint i = 0; i < 256; i++)
	{
		if(acces.value()[i]->type() != CEntity::Unknown)
			acces.value()[i]->update();
	}
}

void CEntityManager::add(uint8 eid, const std::string &name, sint32 totalScore, CRGBA &color, bool spectator)
{
	CEntities::CWriteAccessor acces(entities());
	nlassert(!exist(eid));

	acces.value()[eid]->init (CEntity::Player, name, totalScore, color, "pingoo", spectator);
	nlinfo("CEntityManager::add(%d,%s)",eid,name.c_str());
}

void CEntityManager::remove(uint8 eid)
{
	nlassert(exist(eid));
	
	if(CMtpTarget::instance().controler().Camera.getFollowedEntity() == eid)
		CMtpTarget::instance().controler().Camera.setFollowedEntity(255);

	CEntities::CWriteAccessor acces(entities());
	acces.value()[eid]->reset();
}

bool CEntityManager::exist(uint8 eid)
{
	nlassert(eid != 255);
	CEntities::CReadAccessor acces(entities());
	return acces.value()[eid]->Type != CEntity::Unknown;
}

CEntity &CEntityManager::operator [](uint8 eid)
{
	nlassert(exist(eid));
	CEntities::CReadAccessor acces(entities());
	return *acces.value()[eid]; //todo prevent external code to access entites without using accessor
}

void CEntityManager::getEIdSortedByScore(vector<uint8> &eids) 
{
	eids.clear();

	CEntities::CReadAccessor acces(entities());
	for (uint i = 0; i < 255; i++)
	{
		if (acces.value()[i]->type() == CEntity::Unknown)
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
	CEntities::CReadAccessor acces(entities());
	uint8 neid = eid;
	while(++neid != eid)
		if(acces.value()[neid]->type() != CEntity::Unknown)
			return neid;
	return eid;
}

uint8 CEntityManager::findPreviousEId(uint8 eid) 
{
	CEntities::CReadAccessor acces(entities());
	uint8 neid = eid;
	while(--neid != eid)
		if(acces.value()[neid]->type() != CEntity::Unknown)
			return neid;
	return 255;
}

void CEntityManager::startSession()
{
	CEntities::CReadAccessor acces(entities());
	for(uint i = 0; i < 256; i++)
	{
		if(acces.value()[i]->type() != CEntity::Unknown)
		{
			acces.value()[i]->close();
		}
	}
}

void CEntityManager::reset()
{
	CEntities::CReadAccessor acces(entities());
	for(uint i = 0; i < 256; i++)
	{
		if(acces.value()[i]->type() != CEntity::Unknown)
		{
			acces.value()[i]->interpolator().reset();
		}
	}
}

void CEntityManager::resetSpectator()
{
	CEntities::CReadAccessor acces(entities());
	for(uint i = 0; i < 256; i++)
	{
		if(acces.value()[i]->type() != CEntity::Unknown)
		{
			acces.value()[i]->spectator(false);
		}
	}
}

void CEntityManager::resetReady()
{
	CEntities::CReadAccessor acces(entities());
	for(uint i = 0; i < 256; i++)
	{
		if(acces.value()[i]->type() != CEntity::Unknown)
		{
			acces.value()[i]->ready(false);
		}
	}
}

uint8 CEntityManager::size()
{
	CEntities::CReadAccessor acces(entities());
	uint8 nb = 0;
	for(uint i = 0; i < 256; i++)
	{
		if(acces.value()[i]->type() != CEntity::Unknown)
		{
			nb++;
		}
	}
	return nb;
}

void CEntityManager::renderNames()
{
	CEntities::CReadAccessor acces(entities());
	for(uint i = 0; i < 256; i++)
	{
		if(acces.value()[i]->type() != CEntity::Unknown)
			acces.value()[i]->renderName();
	}	
}

void CEntityManager::render()
{
}

void CEntityManager::load3d()
{
	CEntities::CReadAccessor acces(entities());
	for(uint i = 0; i < 256; i++)
	{
		if(acces.value()[i]->type() != CEntity::Unknown && !acces.value()[i]->spectator())
			acces.value()[i]->load3d();
	}
}

//
// Commands
//
