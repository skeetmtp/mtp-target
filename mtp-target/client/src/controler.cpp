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

#include "stdpch.h"

#include <nel/misc/file.h>
#include <nel/misc/path.h>
#include <nel/misc/bitmap.h>

#include "3d_task.h"
#include "editor_task.h"
#include "time_task.h"
#include "controler.h"
#include "mtp_target.h"
#include "network_task.h"
#include "level_manager.h"
#include "entity_manager.h"

using namespace std;
using namespace NLMISC;
using namespace NL3D;

#define PIQUE_SPEED (0.05f)
#define ROT_SPEED_OPEN (0.05f)
#define ROT_SPEED_CLOSE (0.5f)

static bool			 CaptureState = false;

CVector ControlerFreeLookPos(0,0,0);
CMatrix ControlerCamMatrix;


CControler::CControler()
{
	EId = 255;

	FirstFrame = true;
	Accel = CVector(0,0,0);
	VirtualFrameTime = 0.0f;
	ViewedEId = 0;

	MissedTime = 0.0f;
	RotZ = 0.0f;
}

CControler::~CControler()
{
}

void CControler::setControledEntity(uint8 eid)
{
	EId = eid;
	//camera.setFollowedEntity(entity);
}

uint8 CControler::getControledEntity() const
{
	return EId;
}

void CControler::swapOpenClose()
{
	CVector dir = CEntityManager::instance()[EId].interpolator.direction();
	dir.z = 0;
	dir.normalize();
	
	RotZ = (float)acos (dir.y);
	if(dir.x>0.0f)
		RotZ = -RotZ;
	RotZ += (float)NLMISC::Pi;
	
	//RotZ  = 0;
	Pique = 1.0f;
	Accel = CVector::Null;
}

void CControler::update()
{
	if(EId==255)
		return;

	if(FirstFrame)
	{
		FirstFrame = false;
		Accel = CVector::Null;
		VirtualFrameTime = 0.0f;
	}

	CVector	deltaAccel(CVector::Null);
	CVector	deltaDirection(CVector::Null);
	float	deltaPique = 0.0f;
	float	deltaRot = 0.0f;
	float	speed = CEntityManager::instance()[EId].interpolator.speed().norm();
	float	speedRatio = 2.0f;
	{
		CMatrix *mat = Camera.getMatrix();
		CVector lookAt;
		CVector right;
		lookAt = mat->getJ();
		right  = mat->getI();

		if (!FollowEntity && !CEditorTask::instance().enable())
		{			
			CVector dv(0,0,0);
			if (C3DTask::instance().kbDown(KeyUP))
			{
				dv.y += 1.0f;
			}
			if (C3DTask::instance().kbDown(KeyDOWN))
			{
				dv.y -= 1.0f;
			}
			if (C3DTask::instance().kbDown(KeyLEFT))
			{
				dv.x -= 1.0f;
			}
			if (C3DTask::instance().kbDown(KeyRIGHT))
			{
				dv.x += 1.0f;
			}
			if (C3DTask::instance().kbDown(KeyPRIOR))
			{
				dv.z += 1.0f;
			}
			if (C3DTask::instance().kbDown(KeyNEXT))
			{
				dv.z -= 1.0f;
			}

			dv /= 4.0f;

			if (C3DTask::instance().kbDown(KeySHIFT))
				dv *= 13.0f;

			if (C3DTask::instance().kbDown(KeyCONTROL))
				dv *= 40.0f;
			
			dv *= (float)CTimeTask::instance().deltaTime();

			float mouseX = C3DTask::instance().mouseListener().MouseX;
			float mouseY = -C3DTask::instance().mouseListener().MouseY;
			
			CMatrix m2;
			m2.identity();
			m2.rotateZ(mouseX);
			m2.rotateX(mouseY);
			dv = m2 * dv;
			ControlerFreeLookPos += dv;

			ControlerCamMatrix.identity();
			ControlerCamMatrix.translate(ControlerFreeLookPos);
			ControlerCamMatrix.rotateZ(mouseX);
			ControlerCamMatrix.rotateX(mouseY);
//			nlinfo("set camera matrix");
			C3DTask::instance().scene().getCam()->setMatrix(ControlerCamMatrix);			
		}
		else
		{
			if(!CEntityManager::instance()[EId].openClose())
			{
				/*
				if (C3DTask::instance().kbDown(mtLEFT))
					deltaRot += ROT_SPEED_CLOSE;
				if (C3DTask::instance().kbDown(mtRIGHT))
					deltaRot -= ROT_SPEED_CLOSE;
				*/
				if (C3DTask::instance().kbDown(KeyLEFT))
					deltaAccel -= right * speed * speedRatio;
				if (C3DTask::instance().kbDown(KeyRIGHT))
					deltaAccel += right * speed * speedRatio;

				if (C3DTask::instance().kbDown(KeyUP))
					deltaAccel += lookAt;
				if (C3DTask::instance().kbDown(KeyDOWN))
					deltaAccel -= lookAt;
			}
			else
			{
				if (C3DTask::instance().kbDown(KeyUP))
					deltaPique -= PIQUE_SPEED;
				if (C3DTask::instance().kbDown(KeyDOWN))
					deltaPique += PIQUE_SPEED;

				if (C3DTask::instance().kbDown(KeyLEFT))
					deltaRot += ROT_SPEED_OPEN;
				if (C3DTask::instance().kbDown(KeyRIGHT))
					deltaRot -= ROT_SPEED_OPEN;
			}
		}
		
		if (C3DTask::instance().kbPressed(KeyESCAPE))
			exit(0);

		if (C3DTask::instance().kbPressed(KeyF1))
		{
			if (C3DTask::instance().kbDown(KeySHIFT))
			{
//				mtpTarget::instance().World.switchPhysic();
				CLevelManager::instance().currentLevel().switchStartPositions();
			}
			else
			{
//				mtpTarget::instance().World.switchLevel();
				CLevelManager::instance().currentLevel().switchLevel();
			}
		}

		if (C3DTask::instance().kbPressed(KeyF3))
		{
			UDriver::TPolygonMode p = C3DTask::instance().driver().getPolygonMode ();
			p = UDriver::TPolygonMode(((int)p+1)%3);
			C3DTask::instance().driver().setPolygonMode (p);
		}
		

		
		if (C3DTask::instance().kbDown(KeyCONTROL) && C3DTask::instance().kbPressed(KeyF4))
		{
			if (C3DTask::instance().kbDown(KeySHIFT))
			{
//				mtpTarget::instance().World.switchStartPosition();
			}
			else
			{
				DisplayDebug = !DisplayDebug;
			}
		}

		if (C3DTask::instance().kbPressed(KeyF5))
			CNetworkTask::instance().command("forceEnd");
			//mtNetSendCommand("forceEnd");

		if (C3DTask::instance().kbPressed(KeyF6))
			CNetworkTask::instance().command("reset");
			//mtNetSendCommand("reset");

		if (C3DTask::instance().kbPressed(KeyF7))
		{
			FollowEntity = !FollowEntity;
			CMtpTarget::instance().controler().Camera.getMatrix()->getPos(ControlerFreeLookPos);
		}

		if (C3DTask::instance().kbPressed(KeyF8))
		{
			// F8 -> release/capture the mouse cursor
			if (!CaptureState)
			{
				C3DTask::instance().driver().setCapture(false);
				C3DTask::instance().driver().showCursor(true);
				C3DTask::instance().mouseListener().removeFromServer(C3DTask::instance().driver().EventServer);
			}
			else
			{
				C3DTask::instance().driver().setCapture(true);
				C3DTask::instance().driver().showCursor(false);
				C3DTask::instance().mouseListener().addToServer(C3DTask::instance().driver().EventServer);
			}
			CaptureState = !CaptureState;
		}

		if (C3DTask::instance().kbPressed(KeyF9))
		{
/*			ViewedEId++;
			if(ViewedEId==mtpTarget::instance().World.count())
				ViewedEId=0;
			CEntity *entity= mtpTarget::instance().World.get(ViewedEId);
			camera.setFollowedEntity(entity);
*/
			ViewedEId = CEntityManager::instance().findPreviousEId(ViewedEId);
			Camera.setFollowedEntity(ViewedEId);
		}

		if (C3DTask::instance().kbPressed(KeyF10))
		{
/*			if(ViewedEId==0)
				ViewedEId=mtpTarget::instance().World.count()-1;
			else
				ViewedEId--;

			CEntity *entity= mtpTarget::instance().World.get(ViewedEId);
			camera.setFollowedEntity(entity);
*/
			ViewedEId = CEntityManager::instance().findNextEId(ViewedEId);
			Camera.setFollowedEntity(ViewedEId);
		}

		if (C3DTask::instance().kbPressed(KeyF11))
		{
			CMtpTarget::instance().resetFollowedEntity();
		}

		if (C3DTask::instance().kbPressed(KeyLCONTROL) || C3DTask::instance().kbPressed(KeyRCONTROL) || C3DTask::instance().kbPressed(KeyCONTROL))
		{
			CNetworkTask::instance().openClose();
		}
	}

	float dt = (float)CTimeTask::instance().deltaTime();

	VirtualFrameTime += (float)CTimeTask::instance().deltaTime();
	Accel     += deltaAccel * ( (dt + MissedTime) / NetVirtualFrameDuration );
	Pique     += deltaPique * ( (dt + MissedTime) / NetVirtualFrameDuration ); 
	RotZ      += deltaRot   * ( (dt + MissedTime) / NetVirtualFrameDuration );
	MissedTime = 0.0f;

//	nlinfo ("pik %f", Pique);

/*	if(Pique<0.0f)
		Pique=0.0f;
	if(Pique>1.0f)
		Pique=1.0f;*/
	
	Pique = max(Pique, 0.0f);
	Pique = min(Pique, 1.0f);

	CVector rotation = CEntityManager::instance()[EId].interpolator.rotation();
	rotation.x  = Pique;
	//rotation.z  = RotZ;
	rotation.y  = deltaRot;
	CEntityManager::instance()[EId].interpolator.rotation(rotation);

	if(VirtualFrameTime >= NetVirtualFrameDuration)
	{
		//TODO gerer la diff de temps entre _virtualFrameTime et NetVirtualFrameDuration
		if(!CEntityManager::instance()[EId].openClose())
		{
			CNetworkTask::instance().force(CVector(Accel.x, Accel.y, 0.0f));
		}
		else
		{
			float realRotz = RotZ - (float)NLMISC::Pi / 2.0f;
			CNetworkTask::instance().force(CVector(realRotz, 0.0f, Pique));
		}
	
		Accel = CVector::Null;
		MissedTime = VirtualFrameTime - NetVirtualFrameDuration;
		VirtualFrameTime = 0.0f;
	}

	Camera.update();
}
