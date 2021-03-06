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

#include <nel/misc/debug.h>
#include <nel/misc/common.h>

#include "camera.h"
#include "3d_task.h"
#include "time_task.h"
#include "mtp_target.h"
#include "entity_manager.h"
#include "config_file_task.h"
#include "hud_task.h"
#include "level_manager.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;


//
// Functions
//

#define STAT 0

#if STAT
FILE *filestat = 0;
#endif

void lookAt(CMatrix &m, const CVector &From, const CVector &At, const CVector &Up)
{

////	TODO IL FAUT BLINDER

	CVector vectorY(At - From); // y vector
	vectorY.normalize();
	
	// Get the dot product, and calculate the projection of the y basis vector onto the up vector. The projection is the z basis vector.
	CVector vectorZ(Up - vectorY * (Up * vectorY));
	
	vectorZ.normalize();
	
	// the x basis vector
	CVector vectorX = vectorY ^ vectorZ;

	m.identity();
	m.setRot(vectorX, vectorY, vectorZ);
	m.setPos(From);
	m.scale (1.0f);

	//m.rotateX(-C3DTask::getInstance().mouseListener().MouseY);
	//m.rotateZ(C3DTask::getInstance().mouseListener().MouseX);
	//m.translate(CVector(0, C3DTask::getInstance().mouseListener().MouseWheel * 2 * GSCALE,0));
}


CCamera::CCamera()
{
	EId = 255;
	InitialPosition = CVector(0.0f, 1000.0f*GScale, 1000.0f*GScale);
	
	reset();

	ForcedSpeed = 1.0f;

	OpenHeight			= CConfigFileTask::getInstance().configFile().getVar("OpenHeight").asFloat() * GScale;
	OpenBackDist		= CConfigFileTask::getInstance().configFile().getVar("OpenBackDist").asFloat() * GScale;
	OpenTargetHeight	= CConfigFileTask::getInstance().configFile().getVar("OpenTargetHeight").asFloat() * GScale;
	OpenTargetBackDist	= CConfigFileTask::getInstance().configFile().getVar("OpenTargetBackDist").asFloat() * GScale;
	CloseHeight			= CConfigFileTask::getInstance().configFile().getVar("CloseHeight").asFloat() * GScale;
	CloseBackDist		= CConfigFileTask::getInstance().configFile().getVar("CloseBackDist").asFloat() * GScale;
	CloseTargetHeight	= CConfigFileTask::getInstance().configFile().getVar("CloseTargetHeight").asFloat() * GScale;
	CloseTargetBackDist	= CConfigFileTask::getInstance().configFile().getVar("CloseTargetBackDist").asFloat() * GScale;
	RotationSpeed		= CConfigFileTask::getInstance().configFile().getVar("CameraRotationSpeed").asFloat();
	
	CurrentHeight		= CloseHeight;
	CurrentBackDist		= CloseBackDist;
	CurrentTargetHeight   = CloseTargetHeight;
	CurrentTargetBackDist = CloseTargetBackDist;
	CurrentHeightSpeed  = 0;

	ActiveMatrix = &MatrixFollow;
	Facing = 0;
	MinDistFromStartPointToMove = 0.01f;
	MinDistFromStartPointToMoveVerticaly = 0.01f;

#if STAT
	filestat = fopen ("campos.csv", "wt");
#endif
}

CCamera::~CCamera()
{
#if STAT
	fclose (filestat);
#endif
}

void CCamera::setQuakeControl(bool on)
{
	if (on)
	{
		MatrixQuake = MatrixFollow;
		ActiveMatrix = &MatrixQuake;
	}
	else
	{
		ActiveMatrix = &MatrixFollow;
	}
}

void CCamera::setInitialPosition(const CVector &initialPosition)
{
	InitialPosition = initialPosition;
	//Position = InitialPosition;
}

void CCamera::reset()
{
	Facing = 0;
	Position = InitialPosition;
	LastEntityPosition = CVector::Null;
	CurrentLookAt   = CVector::Null;
	/*
	if(EId != 255)
		CEntityManager::getInstance()[EId].interpolator().reset();
	*/
	lookAt (MatrixFollow, Position, CurrentLookAt + CVector(0.0f, 0.1f, 0.0f),CVector(0.0f,0.0f,1.0f));
	CurrentLookAt = Position + CVector(0.0f, -10.0f*GScale, 0.0f);
}

void CCamera::setFollowedEntity(uint8 eid)
{
	EId = eid;
	if(EId != 255)
	{
		if(CEntityManager::getInstance()[EId].interpolator().available())
		{
			Position = CEntityManager::getInstance()[EId].interpolator().currentPosition() + CVector(0.0f, 50.0f*GScale, 100.0f*GScale);
			CurrentLookAt = CEntityManager::getInstance()[EId].interpolator().currentPosition();
		}
		else
		{
			Position = InitialPosition;
			CurrentLookAt = Position + CVector(0.0f, -10.0f*GScale, 0.0f);
		}
		CHudTask::getInstance().setDisplayViewedName(CEntityManager::getInstance()[EId].name());
	}
}

uint8 CCamera::getFollowedEntity()
{
	return EId;
}


static float lerp(float src,float dst,float pos)
{
	return (1.0f - pos) * src + pos * dst;
}

static float rotLerp(float rsrc,float rdst,float pos)
{
	double nrsrc = fmod((double)rsrc, 2*NLMISC::Pi) + 2*NLMISC::Pi;
	nrsrc = fmod((double)nrsrc, 2*NLMISC::Pi);
	double nrdst = fmod((double)rdst, 2*NLMISC::Pi) + 2*NLMISC::Pi;
	nrdst = fmod((double)nrdst, 2*NLMISC::Pi);
	if(nrsrc<nrdst)
	{
		if(fabs(nrdst - nrsrc) > NLMISC::Pi)
			nrdst -= 2*NLMISC::Pi;
	}
	else
	{
		if(fabs(nrsrc - nrdst) > NLMISC::Pi)
			nrsrc -= 2*NLMISC::Pi;
	}
	double ipos = 1.0f - pos;

	float res = (float) (nrdst * pos + nrsrc * ipos);
	//nlinfo("%f => %f (%f) = %f",nrsrc,nrdst,pos,res);
	return res;
}

void CCamera::update()
{
	bool updated;
	if(EId == 255) return;

	float heightSpeed = 1.0f * CEntityManager::getInstance()[EId].interpolator().currentSmoothDirection().z / 0.5f;
	if(heightSpeed > 0.0f)
		heightSpeed = 0.0f;
	
	double deltaTime = CTimeTask::getInstance().deltaTime();
	double lpos = 3 * deltaTime / 1.0f;
	if(lpos > 1.0f)
		lpos = 1.0f;
	CurrentHeightSpeed = lerp(CurrentHeightSpeed,heightSpeed,(float)lpos);
		
	if(CEntityManager::getInstance()[EId].interpolator().available())
	{
		bool openClose = CEntityManager::getInstance()[EId].openClose();
		CVector distFromStart = CVector::Null;
		if(CEntityManager::getInstance()[EId].startPointId()!=255 && CLevelManager::getInstance().levelPresent())
			distFromStart = CEntityManager::getInstance()[EId].interpolator().currentPosition() - CLevelManager::getInstance().currentLevel().startPosition(CEntityManager::getInstance()[EId].startPointId());
		if(distFromStart.norm()<MinDistFromStartPointToMoveVerticaly && !openClose)
			CurrentHeightSpeed = 0;

		if(openClose)
			updated = updateRampe(OpenBackDist, OpenHeight, OpenTargetBackDist, OpenTargetHeight);
		else
			updated = updateRampe(CloseBackDist, CloseHeight - CurrentHeightSpeed, CloseTargetBackDist, CloseTargetHeight);
	}
	//if(updated)
	{
		CVector up(0.0f, 0.0f, 1.0f);
		if((CurrentLookAt) !=up)
			lookAt(MatrixFollow, Position, CurrentLookAt, up);
		else
			lookAt(MatrixFollow, Position, CurrentLookAt + CVector(0,0.00001f, 0), up);
	}

	if(!C3DTask::getInstance().levelParticle().empty())
		C3DTask::getInstance().levelParticle().setPos(Position);
}

bool CCamera::updateRampe(float backDist,float height,float targetBackDist,float targetHeight)
{
	double deltaTime = CTimeTask::getInstance().deltaTime();
	bool res = true;
	double lpos = RotationSpeed * deltaTime / 1.0f;
	if(lpos>1.0f)
		lpos=1.0f;
	float ilpos = 1.0f - (float)lpos;

	CurrentBackDist			= lerp(CurrentBackDist, backDist, (float)lpos);
	CurrentHeight			= lerp(CurrentHeight, height, (float)lpos);
	CurrentTargetBackDist	= lerp(CurrentTargetBackDist, targetBackDist, (float)lpos);
	CurrentTargetHeight		= lerp(CurrentTargetHeight, targetHeight, (float)lpos);

	bool openClose = CEntityManager::getInstance()[EId].openClose();
	CVector distFromStart = CVector::Null;
	if(CEntityManager::getInstance()[EId].startPointId()!=255 && CLevelManager::getInstance().levelPresent())
		distFromStart = CEntityManager::getInstance()[EId].interpolator().currentPosition() - CLevelManager::getInstance().currentLevel().startPosition(CEntityManager::getInstance()[EId].startPointId());
	float facing;
	if(distFromStart.norm()>MinDistFromStartPointToMove || openClose)
	{
		facing = rotLerp(Facing, (float)CEntityManager::getInstance()[EId].interpolator().facing(), (float)lpos);
	}
	else
	{
		CVector dir = InitialPosition;
		dir.z = 0;
		dir.normalize();
		facing = (float)acos(dir.y);
		if(dir.x>0.0f)
			facing = - facing;
		//nlinfo("dir = %f %f %f , facing = %f",dir.x,dir.y,dir.z,facing);
	}
	//nlinfo("%f => %f (%f) = %f",_facing,_entity->_facing,lpos,facing);
	Facing = facing;

	CMatrix rotMat;
	CMatrix rotMatNoZoom;
	rotMat.identity();
	rotMat.rotateZ(Facing);

//	if (_allowMouse == 2 || _allowMouse == 1)
	{
		rotMat.rotateZ(C3DTask::getInstance().mouseListener().MouseX);

		static const float eps = 0.1f;
		if (C3DTask::getInstance().mouseListener().MouseY < -(float)NLMISC::Pi/2.0f+eps)
			C3DTask::getInstance().mouseListener().MouseY = -(float)NLMISC::Pi/2.0f+eps;

		if (C3DTask::getInstance().mouseListener().MouseY > (float)NLMISC::Pi/2.0f-eps)
			C3DTask::getInstance().mouseListener().MouseY = (float)NLMISC::Pi/2.0f-eps;

		if (InverseMouse)
			rotMat.rotateX(C3DTask::getInstance().mouseListener().MouseY);
		else
			rotMat.rotateX(-C3DTask::getInstance().mouseListener().MouseY);
		
		if (C3DTask::getInstance().mouseListener().MouseWheel < 0)
			C3DTask::getInstance().mouseListener().MouseWheel = 0;
		
		//CVector zoomTans = -getMatrix()->getJ()*(float)(C3DTask::getInstance().mouseListener().MouseWheel)/2.0f;
		//zoomTans.x = 0;
		rotMatNoZoom = rotMat;
		CVector zoomTans = CVector(0,(float)(C3DTask::getInstance().mouseListener().MouseWheel)/2.0f,0);
		rotMat.translate(zoomTans);

		float minMouseAngleToDisplayPart = 0.35f;
		bool displayParticle = fabs(C3DTask::getInstance().mouseListener().MouseX)>minMouseAngleToDisplayPart || C3DTask::getInstance().mouseListener().MouseY<-0.53f ||  C3DTask::getInstance().mouseListener().MouseY>0.4f || C3DTask::getInstance().mouseListener().MouseWheel>4;
	}
	
	Position = CEntityManager::getInstance()[EId].interpolator().currentPosition() + CurrentHeight * CVector(0,0,1) + CurrentBackDist * (rotMatNoZoom * CVector(0,1,0));
	CurrentLookAt = CEntityManager::getInstance()[EId].interpolator().currentPosition() + CurrentTargetHeight * CVector(0,0,1)  + CurrentTargetBackDist * (rotMatNoZoom * CVector(0,1,0));
	CVector up(0.0f, 0.0f, 1.0f);
	if((CurrentLookAt) !=up)
		lookAt(MatrixFollowNoZoom, Position, CurrentLookAt, up);
	else
		lookAt(MatrixFollowNoZoom, Position, CurrentLookAt + CVector(0,0.00001f, 0), up);

	Position = CEntityManager::getInstance()[EId].interpolator().currentPosition() + CurrentHeight * CVector(0,0,1) + CurrentBackDist * (rotMat * CVector(0,1,0));
	CurrentLookAt = CEntityManager::getInstance()[EId].interpolator().currentPosition() + CurrentTargetHeight * CVector(0,0,1)  + CurrentTargetBackDist * (rotMat * CVector(0,1,0));
	return res;
}


CMatrix *CCamera::getMatrix()
{
	return ActiveMatrix;
}

CMatrix *CCamera::getMatrixNoZoom()
{
	return &MatrixFollowNoZoom;
}

CMatrix *CCamera::getMatrixQuake()
{
	return &MatrixQuake;
}

CMatrix *CCamera::getMatrixFollow()
{
	return &MatrixFollow;
}

CVector CCamera::project(const CVector &point)
{
	return CVector::Null;
}


float CCamera::minDistFromStartPointToMove()
{
	return MinDistFromStartPointToMove;
}

void CCamera::minDistFromStartPointToMove(float dist)
{
	MinDistFromStartPointToMove = dist;
}

float CCamera::minDistFromStartPointToMoveVerticaly()
{
	return MinDistFromStartPointToMoveVerticaly;
}

void CCamera::minDistFromStartPointToMoveVerticaly(float dist)
{
	MinDistFromStartPointToMoveVerticaly = dist;
}

