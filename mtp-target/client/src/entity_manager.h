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

class CEntityInitData
{

public:

	CEntityInitData()
	{
		eid = 255;
		name = "unk";
		totalScore = 0;
		color = NLMISC::CRGBA(255,255,255,255);
		spectator = false;
		isLocal = false;
	}

	CEntityInitData(uint8 _eid, const std::string &_name, sint32 _totalScore, NLMISC::CRGBA &_color, const std::string &_texture, bool _spectator, bool _isLocal):eid(_eid),name(_name),totalScore(_totalScore),color(_color),texture(_texture),spectator(_spectator),isLocal(_isLocal)
	{
	}

	CEntityInitData(const CEntityInitData &e) : eid(e.eid),name(e.name),totalScore(e.totalScore),color(e.color),texture(e.texture),spectator(e.spectator),isLocal(e.isLocal)
	{
	}

	uint8 eid;
	std::string name;
	sint32 totalScore;
	NLMISC::CRGBA color;
	std::string texture;
	bool spectator;
	bool isLocal;
};

class CEntityManager : public CSingleton<CEntityManager>, public ITask
{
public:
//	typedef NLMISC::CRWSynchronized <std::vector <CEntity *> >	CEntities;
	typedef std::vector <CEntity *> CEntities;
	
	virtual void init();
	virtual void update();
	virtual void render();
	virtual void release() { 	CEntityManager::uninstance();	}
		
	void renderNames();
	virtual std::string name() { return "CEntityManager"; }

	void	add(uint8 eid, const std::string &name, sint32 totalScore, NLMISC::CRGBA &color, const std::string &texture, bool spectator, bool isLocal);
	void	remove(uint8 eid);
	void	flushAddRemoveList();
	void	_add(std::list<CEntityInitData> &addList);
	void	_remove(std::list<uint8> &removeList);

	bool	exist(uint8 eid) ;

	void	startSession();
	void	reset();
	void	luaInit();
		
	// set all spectator to no more spectator
	void	resetSpectator();
	void	everybodyReady(bool state);
		
	void	load3d();

	CEntity &operator [](uint8 eid);
	CEntity *getByName(std::string &name);
	
	void	getEIdSortedByScore(std::vector<uint8> &eids) ;

	uint8	findFirstEId() ;
	uint8	findNextEId(uint8 eid) ;
	uint8	findPreviousEId(uint8 eid) ;

	uint8	size();

	std::list <uint8 > updateListId;
	CEntities &entities() { return Entities; }

	friend class CSingleton<CEntityManager>;
protected:
	CEntityManager();
private:

	//CEntity		Entities[256];
	CEntities Entities;
	std::list<CEntityInitData> ClientToAddTaskManagerThread;
	std::list<CEntityInitData> ClientToAddNetworkThread;
	std::list<uint8> ClientToRemoveTaskManagerThread;
	std::list<uint8> ClientToRemoveNetworkThread;
	
};

#endif
