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
// This class manages an entity (player or bot)
//

#ifndef MTPT_ENTITY
#define MTPT_ENTITY


//
// Includes
//

#include <deque>
#include <string>

#include <nel/misc/vector.h>
#include <nel/misc/matrix.h>

#include <nel/3d/u_instance.h>
#include <nel/3d/u_particle_system_instance.h>

#include "global.h"


//
// Includes
//

#include <deque>
#include <string>

#include <nel/misc/vector.h>
#include <nel/misc/matrix.h>

#include <nel/3d/u_instance.h>
#include <nel/3d/u_particle_system_instance.h>

#include "sound_manager.h"
#include "interpolator.h"
#include "entity_lua_proxy.h"


//
// Namespaces
//

using NLMISC::CVector;
using NLMISC::CMatrix;


//
// Classes
//

class CEntityManager;
class CEntityProxy;

class CEntity
{
public:

	enum TEntity { Unknown, Player };	// Player is bot or client
	
	void		update();
	void		renderName() const;
	void		close();
	void		swapOpenClose();
	void		collideWhenFly(NLMISC::CVector &pos);
	void		collisionWithWater(bool col);
	bool        openClose() {return OpenClose;};
	bool		isLocal();
	void		setIsLocal(bool local);
	
	// accessor

	uint8				 id() const { return Id; }
	uint8				 rank() const { return Rank; }
	void				 rank(uint8 r) { Rank = r; }
	TEntity				 type() const { return Type; }
	const std::string	&name() const { return Name; }
	sint32				 currentScore() const { return CurrentScore; }
	void				 currentScore(sint32 score) { CurrentScore = score; }
	sint32				 totalScore() const { return TotalScore; }
	void				 totalScore(sint32 score);
	const NLMISC::CRGBA	&color() const { return Color; }
	void  color(const NLMISC::CRGBA &col);
	const std::string	&texture() const { return Texture; }
	uint16				 ping() const { return Ping; }
	void				 ping(uint16 ping) { Ping = ping; }
	
	bool				 spectator() const { return Spectator; }
	void				 spectator(bool b) { Spectator = b; }
	bool				 ready() const { return Ready; }
	void				 ready(bool b) { Ready = b; }
	
	void				 load3d();

	void				 fadeParticleColorTo(const NLMISC::CRGBA &color,float duration);

	CExtendedInterpolator &interpolator() const;

	uint8				 startPointId() const { return StartPointId; }
	void				 startPointId(uint8 spid);
	
	
	NL3D::UInstance CloseMesh, OpenMesh;
	CEntityProxy		*luaProxy;
	
	NL3D::UParticleSystemInstance TraceParticle;
	NL3D::UParticleSystemInstance ImpactParticle;
	
	NLMISC::CVector		 LastSent2MePos;
	NLMISC::CVector		 LastSent2OthersPos;

	bool				 addOpenCloseKey;
	CCrashEvent			 addCrashEventKey;
private:
	
	NLMISC::CRGBA   FadeParticleColor;
	NLMISC::CRGBA   FadeParticleStartColor;
	float           FadeParticleDuration;
	float           FadeParticleStartTime;

	TEntity			Type;
	uint8			Id;
	uint8			StartPointId;
	uint8			Rank;
	std::string		Name;
	std::string		MeshName;
	NLMISC::CRGBA	Color;
	std::string		Texture;
	sint32			CurrentScore;
	sint32			TotalScore;
	uint16			Ping;
	bool			OpenClose; // open=true, close=false

	bool			showCollideWhenFly;
	NLMISC::CVector	showCollideWhenFlyPos;
	
	CMatrix			ObjMatrix;
	CExtendedInterpolator *_interpolator;
	
	// private ctor because only CEntityManager can create a CEntity
	CEntity();
	virtual ~CEntity() { delete _interpolator; _interpolator = 0; }

	void reset();
	void sessionReset();
	void luaInit();
	void init(TEntity type, const std::string &name, sint32 totalScore, NLMISC::CRGBA &color, const std::string &texture, const std::string &meshname, bool spectator, bool isLocal);
	void id(uint8 nid);
	void fadeParticleColorUpdate();
	
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// old stuff to clean



	CSoundManager::CEntitySoundsDescriptor	SoundsDescriptor;

	sint	ParticuleActivated;

	bool	Spectator;
	bool    Ready;

	friend class CEntityManager;
};


#endif
