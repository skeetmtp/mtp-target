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

#ifndef MTPT_ENTITY
#define MTPT_ENTITY


//
// Includes
//

#include <queue>
#include <ode/ode.h>
#include <set>

#include <nel/misc/value_smoother.h>
#include <nel/misc/rgba.h>

#include <nel/net/tcp_sock.h>

#include "module.h"
#include "../../common/lua_utility.h"

class CEntityProxy;

//
// Functions
//

class CEntity
{
public:

	enum TEntity { Unknown, Client, Bot };

	CEntity(uint8 eid);
	CEntity(uint8 eid, const std::string &name);
	virtual ~CEntity();

	void _luaInit();

	// switch the open close status of the entity
	// return true if the switch really occurs
	virtual bool openClose();
	virtual bool isOpen();
	virtual void isOpen(bool oc);
	
	virtual void update();
	virtual void reset();
	virtual void initBeforeStartLevel();
	
	virtual void display(NLMISC::CLog &log = *NLMISC::InfoLog) { }

	virtual void setForce(const NLMISC::CVector &clientForce);

	// accessors

	uint8					 id() const { return Id; }
	const std::string		&name() const { return Name; }
	void name(const std::string &name);
	void startPointId(uint8 id);
	bool isAdmin() const;
		
	TEntity					 type() const { return Type; }

	bool					 spectator() const { return Spectator; }
	void					 spectator(bool b) { Spectator = b; }
	
	CEntityProxy		*luaProxy;

	std::set<CModule *> collideModules;
	std::set<CEntity *> collideEntity;
	bool                collideWater;
public:

	// ugly public variables

	NLMISC::CRGBA		 Color;
	
	sint32				 CurrentScore;		// the current score of the current session
	sint32				 Score;				// the total score off all session
	float				 Time;				// time in second before total stop
	NLMISC::CVector		 Pos;				// position of the client
	NLMISC::CVector		 Force;				// the current client force received

	bool				 OpenClose;			// open = true, close = false (thx to cahanta)
	uint32				 NbOpenClose;		// how many time did the client press space key
	dBodyID				 Body;
	dGeomID				 Geom;
	bool				 FreezeCommand;

	NLMISC::CValueSmootherTemplate<uint16> Ping;
	std::queue<NLMISC::TTime> LastSentPing;

	bool				 Spectator;

	bool				 InGame;
	bool				 Ready;
	//	bool jointed;
	float				 Accel;
	float				 Friction;

	float				 LastVel[10];		// last velocity
	uint32				 LastVelPos;		// between 0 and 9
	uint8				 StartingPointId;
	NLMISC::CVector		 LastSentPos;

	uint8				 LastTouchedClientId;
	sint32				 PushScore;
	sint32				 StopedScore;
	bool				 FirstStop;

	bool				 OnTheWater;

	// just display the state of the last physic render (just for watch purpose)
	std::string			 ColState;

	// lua
	bool				 LuaInit;
	
private:

	void init ();
		
	TEntity				 Type;				// type of entity
	uint8				 Id;
	std::string			 Name;				// empty name mean that the client not already send login information
	bool                 _isAdmin;
};




#endif
