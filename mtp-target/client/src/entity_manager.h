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
// This class manages all entities using a 256 entries static array.
// Using that static array to be able to be multithread safe.
// The entry 255 is always empty because 255 is the code for "unknown" entity.
//

#ifndef MTPT_ENTITY_MANAGER
#define MTPT_ENTITY_MANAGER


//
// Includes
//

#include <nel/misc/reader_writer.h>
#include "entity.h"


//
// Classes
//

class CEntityManager : public CSingleton<CEntityManager>, public ITask
{
public:
	typedef NLMISC::CRWSynchronized <std::vector <CEntity *> >	CEntities;
	
	virtual void init();
	virtual void update();
	virtual void render();
	virtual void release() { 	CEntityManager::uninstance();	}
		
	void renderNames();
	virtual std::string name() { return "CEntityManager"; }

	void	add(uint8 eid, const std::string &name, sint32 totalScore, NLMISC::CRGBA &color, bool spectator);
	void	remove(uint8 eid);
	bool	exist(uint8 eid) ;

	void	startSession();
	void	reset();
	
	// set all spectator to no more spectator
	void	resetSpectator();
	void	resetReady();
		
	void	load3d();

	CEntity &operator [](uint8 eid);

	void	getEIdSortedByScore(std::vector<uint8> &eids) ;

	uint8	findFirstEId() ;
	uint8	findNextEId(uint8 eid) ;
	uint8	findPreviousEId(uint8 eid) ;

	uint8	size();

	std::list <uint8 > updateListId;
	CEntities *entities() { return &Entities; }

	friend class CSingleton<CEntityManager>;
protected:
	CEntityManager();
private:

	//CEntity		Entities[256];
	CEntities Entities;
	
};

#endif
