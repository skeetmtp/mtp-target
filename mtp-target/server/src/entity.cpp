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

#include <nel/misc/path.h>

#include "main.h"
#include "entity.h"
#include "physics.h"
#include "variables.h"
#include "session_manager.h"
#include "level_manager.h"
#include "entity_lua_proxy.h"
#include "lua_engine.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;


//
// Types
//



//
// Declarations
//



//
// Variables
//


//
// Functions
//

void CEntity::init ()
{
	pausePhysics();
	Type = Unknown;
	Id = 255;
	CurrentScore = 0;
	Score = 0;
	Time = 0.0f;
	OpenClose = false;
	NbOpenClose = 0;
	FreezeCommand = true;
	InGame = false;
	Ready = false;
	LastVelPos = 0;
	StartingPointId = 255;
	OnTheWater = false;
	Replay = CVector::Null;
	Accel = 0.0f;
	Friction = 0.0f;
	LuaInit = false;
	Pos = CVector::Null;
	Force = CVector::Null;
	for(uint i = 0; i < 10; i++) LastVel[i] = 0.0f;
	LastTouchedClientId = 0;
	PushScore = 0;
	StopedScore = 0;
	FirstStop = 0;
	Spectator = true;

	nlassert(World);

	// create body
	Body = dBodyCreate(World);
	nlassert(Body);

	// set default position
	dBodySetPosition(Body, Pos.x, Pos.y, Pos.z);

	// set default angle
	dMatrix3 rot;
	dRFromAxisAndAngle(rot, dRandReal()*2.0f-1.0f, dRandReal()*2.0f-1.0f, dRandReal()*2.0f-1.0f, dRandReal()*10.0f-5.0f);
	dBodySetRotation(Body, rot);

	dMass m;
	dReal radius = 0.01f;
	dMassSetSphere(&m, SphereDensity, radius);
    dBodySetMass(Body, &m);
	{
		CSynchronized<dSpaceID>::CAccessor acces(&Space);
		Geom = dCreateSphere(acces.value(), radius);
	}
	nlassert(Geom);

	dGeomSetCategoryBits(Geom,MT_CLIENT_SPECTRUM);	// this is a client
	dGeomSetCollideBits(Geom,MT_CLIENT_SPECTRUM|MT_SCENE_SPECTRUM); // this client collide with other clients and with the scene
	dGeomSetData(Geom, (void *)this);
	
	dGeomSetBody(Geom, Body);
	resumePhysics();
	luaProxy = NULL;
}

CEntity::CEntity(uint8 eid)
{
	nlassert(eid!=255);
	init();
	Id = eid;
	Type = Client;
	_luaInit();
}

CEntity::CEntity(uint8 eid, const std::string &name)
{
	nlassert(eid!=255);
	init();
	Id = eid;
	Name = name;
	Type = Bot;
	Spectator = false;
	_luaInit();
}

void CEntity::_luaInit()
{
	luaProxy = new CEntityProxy(CLuaEngine::instance().session(),this);
}


CEntity::~CEntity()
{
	pausePhysics();
	
	if(luaProxy)
		delete luaProxy;
	luaProxy = NULL;

	if(Geom)
	{
		// to be sure, set the data to dummy before erasing
		dGeomSetData(Geom, (void *)0xDEADBEEF);
		dGeomDestroy(Geom);
		Geom = 0;
	}

	if(Body)
	{
		dBodyDestroy(Body);
		Body = 0;
	}
	resumePhysics();
}

void CEntity::startPointId(uint8 id)
{
	StartingPointId = id % CLevelManager::instance().currentLevel().getStartPointCount();
	CVector startPos = CLevelManager::instance().currentLevel().getStartPoint(StartingPointId)->position();
	dBodySetPosition(Body, startPos.x, startPos.y, startPos.z);
	dGeomSetPosition(Geom, startPos.x, startPos.y, startPos.z);
}

void CEntity::reset() 
{
	collideModules.clear();
	collideEntity.clear();
	collideWater = false;
}

void CEntity::update() 
{
		H_AUTO(CEntityUpdate);
	set<CModule *>::iterator mit;
	for(mit=collideModules.begin();mit!=collideModules.end();mit++)
		CLuaEngine::instance().entitySceneCollideEvent(this,*mit);	
	collideModules.clear();

	set<CEntity *>::iterator eit;
	for(eit=collideEntity.begin();eit!=collideEntity.end();eit++)
		CLuaEngine::instance().entityEntityCollideEvent(this,*eit);	
	collideEntity.clear();

	if(collideWater)
		CLuaEngine::instance().entityWaterCollideEvent(this);	
	collideWater = false;
	
	if(luaProxy)
		luaProxy->call("update");
}

void CEntity::initBeforeStartLevel()
{
	if(luaProxy)
		luaProxy->call("init");
}

bool CEntity::isOpen()
{
	return OpenClose;
}

void CEntity::isOpen(bool open)
{
	OpenClose = open;
}

bool CEntity::openClose()
{
		if(FreezeCommand)
	{
		nlinfo("Client '%s' tries to %s but he is frozen", Name.c_str(), (OpenClose?"open":"close"));
		return false;
	}

	if(NbOpenClose >= MaxOpenClose)
	{
		nlinfo("Client '%s' tries to %s but he reached the MaxOpenClose limit", Name.c_str(), (OpenClose?"open":"close"));
		return false;
	}

	OpenClose = !OpenClose;
	NbOpenClose++;

	if(NbOpenClose >= MaxOpenClose)
	{
		// no more openclose, reset movement accel and vel
		//dBodySetLinearVel(Body, 0.0f, 0.0f, 0.0f);
		dBodySetAngularVel(Body, 0.0f, 0.0f, 0.0f);
		Force = CVector::Null;
	}

	nlinfo("Client '%s' is now %s", Name.c_str(), (OpenClose?"open":"close"));

	// reset all physics values
	if(OpenClose)
	{
		const dReal *vel = dBodyGetLinearVel(Body);
		dBodySetLinearVel(Body, vel[0], vel[1], 0.0f);
		dBodySetAngularVel(Body, 0.0f, 0.0f, 0.0f);
		dMatrix3 R;
		dRSetIdentity(R);
		dBodySetRotation(Body, R);
	}
	return true;
}


void CEntity::setForce(const CVector &clientForce)
{
	// ignoring client update if not in session
	if(CSessionManager::instance().endTime() != 0 || CSessionManager::instance().startTime() == 0 || CTime::getLocalTime() < CSessionManager::instance().startTime())
	{
		//nlinfo("can't update user, no session");
		return;
	}
	
	// can't move because no more open close left
	if(NbOpenClose >= MaxOpenClose)
	{
		//nlinfo("can't update user, max open close reached");
		return;
	}
	
	// client can't move anymore
	if(FreezeCommand)
	{
		//nlinfo("can't update user, freezed");
		return;
	}
	
	Force = clientForce;
	
	if(OpenClose)
	{
		//ForceX *= OpenAccelCoef;
		//ForceY *= OpenAccelCoef;
		//ForceZ *= OpenAccelCoef;
	}
	else
	{
		//nlinfo("%f %f %f (%f %f)", ForceX, ForceY, ForceZ, Accel, CloseAccelCoef);
		float currentAccel = CloseAccelCoef * Accel;
		Force *= currentAccel;
		//nlinfo("clientForce ( %f %f %f )", clientForce.x, clientForce.y, clientForce.z);
		//nlinfo("Force = %f %f %f (%f * %f)", Force.x, Force.y, Force.z, CloseAccelCoef,Accel);
	}
	Accel = 0.0f;
}


//
// Commands
//


//
//
//
