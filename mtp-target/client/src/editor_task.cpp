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


#include "3d_task.h"
#include "time_task.h"
#include "controler.h"
#include "mtp_target.h"
#include "network_task.h"
#include "level_manager.h"
#include "entity_manager.h"
#include "gui.h"
#include "editor_task.h"

using namespace std;
using namespace NLMISC;
using namespace NL3D;



//
// Functions
//
	

void CEditorTask::init()
{
	_enable = false;
	_testFrame = NULL;
	_selectedModule = NULL;
	_mouseX = 0;
	_mouseY = -0.5f;
	_lastUpdateTime = 0;
}


void CEditorTask::_mouseSelectModule() 
{
	bool mousePressed = CGuiObjectManager::instance().mouseListener().Pressed;
	CVector mousePressedPos = CGuiObjectManager::instance().mouseListener().pressedPosition();
	uint32 i;
	if(mousePressed)
	{
		_selectedModule = NULL;
		//nlinfo("editor ray test");
		CViewport vp = C3DTask::instance().scene().getViewport();
		CMatrix camMat = C3DTask::instance().scene().getCam()->getMatrix();
		CFrustum frustum = C3DTask::instance().scene().getCam()->getFrustum();
		C3DTask::instance().driver().setFrustum(frustum);
		CVector rayStart,rayDir,rayEnd;
		float x = mousePressedPos.x / C3DTask::instance().screenWidth();
		float y = 1.0f - mousePressedPos.y / C3DTask::instance().screenHeight();
		vp.getRayWithPoint(x,y,rayStart,rayDir,camMat,frustum);
		rayDir.normalize();
		rayEnd = rayStart + rayDir * 10000;
		list<CModule *> bboxModules;
		for(i=0;i<CLevelManager::instance().currentLevel().getModuleCount();i++)
		{
			CModule *module = CLevelManager::instance().currentLevel().getModule(i);
			UInstance *mesh = module->mesh();
			CAABBox bbox;
			mesh->getShapeAABBox(bbox);
			CAABBox tbbox = CAABBox::transformAABBox(mesh->getMatrix(),bbox);
			bool intersect = tbbox.intersect(rayStart,rayEnd);
			if(intersect)
			{
				bboxModules.push_back(module);
				nlinfo("bbox intersect %s 0x%p ",module->name().c_str(),module);
			}
		}
		
		//test plus precis :
		
		CModule *nearestModule = NULL;
		float nearestModuleDist = 10000.0f;
		list<CModule *>::iterator it;
		for(it=bboxModules.begin();it!=bboxModules.end();it++)
		{
			CModule *module = *it;
			CVector rayHit;
			bool intersect = module->intersect(rayStart,rayEnd,rayHit);
			if(intersect)
			{
				CVector rayDirHit = rayHit - rayStart;
				float dist = rayDirHit * rayDir;
				if(dist>0.0f && dist<nearestModuleDist)
				{
					nearestModuleDist = dist;
					nearestModule = module;
					//nlinfo("tri intersect %s 0x%p dist = %f (%f %f %f)",nearestModule->name().c_str(),nearestModule,nearestModuleDist,rayHit.x,rayHit.y,rayHit.z);
				}
			}
		}
		if(nearestModule)
		{
			_selectedModule = nearestModule;
			nlinfo("tri intersect %s 0x%p dist = %f",nearestModule->name().c_str(),nearestModule,nearestModuleDist);
		}
	}
}

void CEditorTask::update() 
{
	if(C3DTask::instance().kbPressed(KeyF4))
	{
		enable(!enable());
	}

	if(_enable)
	{
		_mouseSelectModule();

		if (C3DTask::instance().kbPressed(KeyNUMPAD4))
		{
			_mouseX += 0.5f;
		}
		if (C3DTask::instance().kbPressed(KeyNUMPAD6))
		{
			_mouseX -= 0.5f;
		}
		if (C3DTask::instance().kbPressed(KeyNUMPAD8))
		{
			_mouseY += 0.1f;
		}
		if (C3DTask::instance().kbPressed(KeyNUMPAD5))
		{
			_mouseY -= 0.1f;
		}
		
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

		//float	mouseX = 0;//CGuiObjectManager::instance().mouseListener().MouseX;
		//float	mouseY = -0.5f;//-CGuiObjectManager::instance().mouseListener().MouseY;

		CMatrix m2;
		m2.identity();
		m2.rotateZ(_mouseX);
		//m2.rotateX(mouseY);
		dv = m2 * dv;
		ControlerFreeLookPos += dv;
		
		ControlerCamMatrix.identity();
		ControlerCamMatrix.translate(ControlerFreeLookPos);
		ControlerCamMatrix.rotateZ(_mouseX);
		ControlerCamMatrix.rotateX(_mouseY);
		//			nlinfo("set camera matrix");
		C3DTask::instance().scene().getCam()->setMatrix(ControlerCamMatrix);			

		if(selectedModule() && dv.norm()!=0.0f)
		{
			CVector oldPos = selectedModule()->position();
			selectedModule()->position(oldPos + dv);
		}

		double time = CTimeTask::instance().time();
		if((time-_lastUpdateTime)>0.1f)
		{
			for(uint i=0;i<CLevelManager::instance().currentLevel().getModuleCount();i++)
			{
				CModule *m = CLevelManager::instance().currentLevel().getModule(i);
				if(m->changed())
				{
					CNetworkTask::instance().moduleUpdate(m);
					m->changed(false);
				}
			}
			_lastUpdateTime = time;
		}
	}
}

void CEditorTask::render()
{
	C3DTask::instance().driver().setFrustum(C3DTask::instance().scene().getCam()->getFrustum());
	C3DTask::instance().driver().setMatrixMode3D(*C3DTask::instance().scene().getCam());
	if(_selectedModule)
	{
		_selectedModule->renderBBox();
	}
}

void CEditorTask::release()
{
}

void CEditorTask::enable(bool e)
{
	if(e==_enable) return;
	if(e)
	{
		nlassert(!_testFrame);
		guiSPG<CGuiXml> xml = NULL;
		xml = CGuiXmlManager::instance().Load("editor.xml");
		_testFrame = (CGuiFrame *)xml->get("editorFrame");
		CGuiObjectManager::instance().objects.push_back(_testFrame);	
	}
	else
	{
		_testFrame = NULL;
	}
	_enable = e;
	FollowEntity = !e;
//	CMtpTarget::instance().controler().Camera.getMatrix()->getPos(ControlerFreeLookPos);
}

bool CEditorTask::enable()
{
	return _enable;
}

void CEditorTask::reset()
{
	_selectedModule = NULL;
}

