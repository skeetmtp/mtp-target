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
	entities().clear();
	for(uint i = 0; i < 256; i++)
	{
		entities().push_back(new CEntity);
	}
}

void CEntityManager::init()
{
	for(uint i = 0; i < 256; i++)
	{
		entities()[i]->id(i);
		entities()[i]->reset();
	}
	updateListId.clear();
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

void CEntityManager::add(uint8 eid, const std::string &name, sint32 totalScore, CRGBA &color, bool spectator)
{
	nlassert(!exist(eid));

	entities()[eid]->init (CEntity::Player, name, totalScore, color, "pingoo", spectator);
	nlinfo("CEntityManager::add(%d,%s)",eid,name.c_str());
}

void CEntityManager::remove(uint8 eid)
{
	nlassert(exist(eid));
	
	if(CMtpTarget::instance().controler().Camera.getFollowedEntity() == eid)
		CMtpTarget::instance().controler().Camera.setFollowedEntity(255);

	entities()[eid]->reset();
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

void CEntityManager::resetReady()
{
	for(uint i = 0; i < 256; i++)
	{
		if(entities()[i]->type() != CEntity::Unknown)
		{
			entities()[i]->ready(false);
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
