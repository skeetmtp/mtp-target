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

#include <deque>
#include <string>

#include <nel/misc/quat.h>
#include <nel/misc/common.h>

#include <nel/3d/u_instance_material.h>

#include "3d/water_height_map.h"
#include "3d/water_pool_manager.h"

#include "global.h"
#include "entity.h"
#include "mtp_target.h"
#include "resource_manager.h"

#include "stdpch.h"

#include <deque>

#include <nel/misc/quat.h>
#include <nel/misc/common.h>

#include <nel/3d/u_instance_material.h>

#include "3d/water_height_map.h"
#include "3d/water_pool_manager.h"

#include "entity.h"
#include "global.h"
#include "3d_task.h"
#include "time_task.h"
#include "mtp_target.h"
#include "font_manager.h"
#include "level_manager.h"
#include "config_file_task.h"
#include "../../common/constant.h"

using namespace std;
using namespace NLMISC;
using namespace NL3D;



CEntity::CEntity()
{
	Type = Unknown;
	TraceParticle = 0;
	Rank = 255;
	LastSent2MePos = CVector::Null;
	LastSent2OthersPos = CVector::Null;
	_interpolator = 0;
	Ready = false;
	luaProxy = 0;
}

void CEntity::swapOpenClose()
{
	OpenClose = !OpenClose;

	if (OpenClose)
	{
		ParticuleActivated = 1;
		if(!CloseMesh.empty())
			CloseMesh.hide();
		if(!OpenMesh.empty())
			OpenMesh.show();
		SoundsDescriptor.play(CSoundManager::CEntitySoundsDescriptor::BallOpen);
		SoundsDescriptor.stop(CSoundManager::CEntitySoundsDescriptor::BallClose);
	}
	else
	{
		ParticuleActivated = 0;
		if(!CloseMesh.empty())
			CloseMesh.show();
		if(!OpenMesh.empty())
			OpenMesh.hide();
		SoundsDescriptor.stop(CSoundManager::CEntitySoundsDescriptor::BallOpen);
		SoundsDescriptor.play(CSoundManager::CEntitySoundsDescriptor::BallClose);
	}

	if(!OpenClose)
	{
		ObjMatrix.rotateX(1.0f);
		ObjMatrix.rotateY(1.0f);
	}

	if(isLocal())
		CMtpTarget::instance().controler().swapOpenClose();
}

CExtendedInterpolator &CEntity::interpolator() const
{
	nlassert(_interpolator);
	return *_interpolator;
}

void CEntity::close()
{
	OpenClose = false;
	
	ParticuleActivated = 0;
	if(!CloseMesh.empty())
		CloseMesh.show();
	if(!OpenMesh.empty())
		OpenMesh.hide();
}

void CEntity::update()
{
	interpolator().update();

//	nlinfo("set matrix for %hu", (uint16)id());
				
	if(!OpenMesh.empty())
		OpenMesh.setMatrix(interpolator().getMatrix());

	if(!CloseMesh.empty())
		CloseMesh.setMatrix(interpolator().getMatrix());

	{
/*ace todo		if (OnWater && WaterModel)
		{
			//nlinfo("water set pos begin");
			NLMISC::CVector pos = m.getPos();
			pos.z = WaterModel->getAttenuatedHeight(NLMISC::CVector2f(pos.x, pos.y), CMtpTarget::instance().controler().Camera.getMatrix()->getPos());
			m.setPos(pos);
			//nlinfo("water set pos end");
		}
*//*		else
		{
			CWaterHeightMap &whm = GetWaterPoolManager().getPoolByID(0);
			const float waterRatio = whm.getUnitSize(); 
			const float invWaterRatio = 1.0f / waterRatio;
			sint px = (sint) (_position.x * invWaterRatio);
			sint py = (sint) (_position.y * invWaterRatio);			
			whm.perturbate(px, py, 2, 1.0f*GScale);
		}
*/
	}

	SoundsDescriptor.update3d(ObjMatrix.getPos(), CVector(0,0,0)); // todo : velocity

	if(!TraceParticle.empty())
	{
		TraceParticle.setPos(interpolator().position());
		
		// we activate
		if (ParticuleActivated != -1 && TraceParticle.isSystemPresent())
		{
			//if (ParticuleActivated == 0 || CMtpTarget::instance().controler().getControledEntity() != this)
			
			
			//TraceParticle->activateEmitters(ParticuleActivated==1);
			if(ParticuleActivated==1)
				fadeParticleColorTo(CRGBA(255,255,255,255),1);
			else
				fadeParticleColorTo(CRGBA(0,0,0,0),1);

			//if(ParticuleActivated==1 && CMtpTarget::instance().controler().getControledEntity()!=id())
			//	TraceParticle->show();

			ParticuleActivated = -1;
		}
	}

	fadeParticleColorUpdate();
}

void CEntity::collisionWithWater(bool col)
{
	if(col && interpolator().onWater())
		return;

	if(col && !interpolator().onWater())
	{
		// launch the splatch sound
		SoundsDescriptor.play(CSoundManager::CEntitySoundsDescriptor::Splash);

/*
		CWaterHeightMap &whm = GetWaterPoolManager().getPoolByID(0);
		const float waterRatio = whm.getUnitSize(); 
		const float invWaterRatio = 1.0f / waterRatio;
		sint px = (sint) (_position.x * invWaterRatio);
		sint py = (sint) (_position.y * invWaterRatio);
		whm.perturbate(px, py, 2, 2.0f);
*/

		close ();

	}
}

void CEntity::renderName() const
{
	// display name of other people than me
	if(CMtpTarget::instance().controler().getControledEntity() != id())
	{
		CFontManager::instance().printf3D(color(), interpolator().position(),0.01f, name().c_str());
	}
}

void CEntity::id(uint8 nid) 
{ 
	nlassert(!_interpolator);
	_interpolator = new CExtendedInterpolator(MT_NETWORK_UPDATE_PERIODE);
	Id = nid;
}

void CEntity::reset()
{
	//nlinfo(">> 0x%p::CEntity::reset() (eid %u)",this, Id);
	interpolator().entity(this);
	
	if(!TraceParticle.empty())
	{
		C3DTask::instance().scene().deleteInstance(TraceParticle);
	}

	if(!CloseMesh.empty())
	{
		nlinfo(">>   C3DTask::instance().scene().deleteInstance(CloseMesh);");
		CloseMesh.hide();
		//C3DTask::instance().scene().deleteInstance(CloseMesh);
	}
	
	if(!OpenMesh.empty())
	{
		OpenMesh.hide();
		//C3DTask::instance().scene().deleteInstance(OpenMesh);
	}

	CSoundManager::instance().unregisterEntity(SoundsDescriptor);

	Type = Unknown;
	Name.clear();
	Color.set(255,255,255);
	Texture = "";
	CurrentScore = 0;
	TotalScore = 0;
	Ping = 0;
	OpenClose = false;
	ObjMatrix.identity();
	ObjMatrix.rotateX(1.0f);
	ObjMatrix.rotateY(1.0f);
	ParticuleActivated = 0;
	Ready = false;
}

void CEntity::init(TEntity type, const std::string &name, sint32 totalScore, CRGBA &color, const std::string &texture, const std::string &meshname, bool spectator, bool isLocal)
{
	nlassert(type != Unknown);

	Type = type;
	Name = name;
	TotalScore = totalScore;
	Color = color;
	Texture = texture;
	MeshName = meshname;
	Spectator = spectator;
	Ready = false;
	interpolator().reset();
	setIsLocal(isLocal);

	CSoundManager::instance().registerEntity(SoundsDescriptor);
}

void CEntity::luaInit()
{
	if(luaProxy)
	{
		delete luaProxy;
		luaProxy = 0;
	}
	if(CLevelManager::instance().levelPresent())
	{
		luaProxy = new CEntityProxy(CLevelManager::instance().currentLevel().luaState(),this);	
		nlinfo("CEntity::luaInit(), luaState=0x%p , proxy = 0x%p",CLevelManager::instance().currentLevel().luaState(),luaProxy);
	}
	else
		nlwarning("lua init : no level loaded");
}

void CEntity::load3d()
{
	nlinfo(">> 0x%p::CEntity::load3d()",this);
	string TextureFilename;
	bool ok;
	if(!Texture.empty())
	{
		TextureFilename = CResourceManager::instance().get("ping_ball_"+Texture+".tga", ok);
		if(!ok) TextureFilename = "";
	}

	if(CloseMesh.empty())
	{
		string res = CResourceManager::instance().get("entity_"+MeshName+"_close.shape");
		CloseMesh = C3DTask::instance().scene().createInstance(res);
	}
	for(uint i = 0; i < CloseMesh.getNumMaterials(); i++)
	{
		CloseMesh.getMaterial(i).setDiffuse(Color);
		CloseMesh.getMaterial(i).setAmbient(Color);
		if(!TextureFilename.empty())// && i == 0)
			CloseMesh.getMaterial(i).setTextureFileName(TextureFilename);
	}
	CloseMesh.show();
	CloseMesh.setTransformMode (UTransformable::DirectMatrix);


	if(OpenMesh.empty())
	{
		string res = CResourceManager::instance().get("entity_"+MeshName+"_open.shape");
		OpenMesh = C3DTask::instance().scene().createInstance(res);
	}
	for(uint i = 0; i < OpenMesh.getNumMaterials(); i++)
	{
		OpenMesh.getMaterial(i).setDiffuse(Color);
		OpenMesh.getMaterial(i).setAmbient(Color);
		if(!TextureFilename.empty())// && i == 0)
			OpenMesh.getMaterial(i).setTextureFileName(TextureFilename);
	}
	OpenMesh.hide();
	OpenMesh.setTransformMode (UTransformable::DirectMatrix);


	if(TraceParticle.empty() && CConfigFileTask::instance().configFile().getVar("DisplayParticle").asInt() == 1)
	{
		string res = CResourceManager::instance().get("trace.ps");
		TraceParticle.cast(C3DTask::instance().scene().createInstance(res));
		TraceParticle.setTransformMode (UTransformable::RotQuat);
		TraceParticle.setOrderingLayer(2);
		TraceParticle.activateEmitters(true);
		TraceParticle.show();
		TraceParticle.setUserColor(CRGBA(0,0,0,0));
		ParticuleActivated = 0;
	}
}


bool CEntity::isLocal()
{ 
	return CMtpTarget::instance().controler().getControledEntity() == Id;
}

void CEntity::setIsLocal(bool local)
{
	if(local)
	{
		CMtpTarget::instance().controler().setControledEntity(id());
		interpolator().dt(MT_NETWORK_MY_UPDATE_PERIODE);
	}
	else
		interpolator().dt(MT_NETWORK_UPDATE_PERIODE);
}


void CEntity::fadeParticleColorTo(const NLMISC::CRGBA &color,float duration)
{
	if(FadeParticleColor==color || duration<=0) return;

	FadeParticleColor = color;
	FadeParticleStartColor = TraceParticle.getUserColor();
	FadeParticleDuration = duration;
	FadeParticleStartTime = (float)CTimeTask::instance().time();
}

void CEntity::fadeParticleColorUpdate()
{
	float time = (float)CTimeTask::instance().time();
	float lpos = (time  - FadeParticleStartTime) / FadeParticleDuration;
	if(lpos<0 || 1<lpos) return;

	CRGBA newCol;
	newCol.blendFromui(FadeParticleStartColor,FadeParticleColor,(uint)(256 * lpos));
	if(!TraceParticle.empty())
		TraceParticle.setUserColor(newCol);
}


void  CEntity::color(const NLMISC::CRGBA &col) 
{ 
	Color = col; 
	if(!CloseMesh.empty())
	{
		for(uint i = 0; i < CloseMesh.getNumMaterials(); i++)
		{
			CloseMesh.getMaterial(i).setDiffuse(Color);
			CloseMesh.getMaterial(i).setAmbient(Color);
		}
	}
	if(!OpenMesh.empty())
	{
		for(uint i = 0; i < CloseMesh.getNumMaterials(); i++)
		{
			OpenMesh.getMaterial(i).setDiffuse(Color);
			OpenMesh.getMaterial(i).setAmbient(Color);
		}
	}
	
}
