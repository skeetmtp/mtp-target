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
#include "resource_manager.h"
#include "gui_progress_bar.h"
#include "gui_text.h"
#include "gui_xml.h"

#include <nel/3d/u_material.h>

//
// Namespaces
//

using namespace std;
using namespace NL3D;
using namespace NLMISC;


//
// Variables
//


//
// Functions
//
	

void CGuiProgressBarManager::init()
{
	string res;

	res = CResourceManager::instance().get("progress.tga");
	_textureProgress = C3DTask::instance().driver().createTextureFile(res);
	nlassert(_textureProgress);
	
	_materialProgress = C3DTask::instance().driver().createMaterial ();
	_materialProgress->setTexture(_textureProgress);
	_materialProgress->setBlend(true);
	_materialProgress->setZFunc(UMaterial::always);
	_materialProgress->setDoubleSided();
	
	res = CResourceManager::instance().get("progress-bar.tga");
	_textureProgressBar = C3DTask::instance().driver().createTextureFile(res);
	nlassert(_textureProgress);
	
	_materialProgressBar = C3DTask::instance().driver().createMaterial ();
	_materialProgressBar->setTexture(_textureProgressBar);
	_materialProgressBar->setBlend(true);
	_materialProgressBar->setZFunc(UMaterial::always);
	_materialProgressBar->setDoubleSided();
	
	CGuiProgressBar::XmlRegister();
}


void CGuiProgressBarManager::render()
{
	
	
}

void CGuiProgressBarManager::release()
{
	
}


//
//
//
CGuiProgressBar::CGuiProgressBar()
{
	_percent = 0.1f;
	quad.material(CGuiProgressBarManager::instance()._materialProgress);
	CGuiTextPercent *text = new CGuiTextPercent;
	text->ptrValue(&_percent);
	element(text);
	_ptrValue = NULL;
}

CGuiProgressBar::~CGuiProgressBar()
{
	
}

void CGuiProgressBar::_render(CVector pos,CVector &maxSize)
{
	if(_ptrValue)
		_percent = *_ptrValue;
	CVector globalPos = globalPosition(pos,maxSize);
	CVector expSize = expandSize(maxSize);

	_percent = min(1.0f,_percent);
	_percent = max(0.0f,_percent);
	
	float ipos = 1.0f - _percent;
	
	quad.size(expSize);
	quad.position(globalPos);
	quad.material(CGuiProgressBarManager::instance()._materialProgress);
	quad.render();

	if(_percent>0.0f)
	{
		float barWidth  = (expSize.x-4)*_percent;
		float barHeight = expSize.y-4;
		quad.size(CVector(barWidth,barHeight,0));
		quad.position(globalPos+CVector(2,2,0));
		quad.material(CGuiProgressBarManager::instance()._materialProgressBar);
		quad.render();
	}

	CGuiBin::renderElement(pos,maxSize);
	maxSize = expSize;	
}


float CGuiProgressBar::percent()
{
	return _percent;
}

void CGuiProgressBar::percent(float percent)
{
	_percent = percent;
	if(_ptrValue)
		*_ptrValue = percent;
}

void CGuiProgressBar::ptrValue(float *ptrValue)
{
	_ptrValue = ptrValue;
}




void CGuiProgressBar::XmlRegister()
{
	CGuiObjectManager::instance().registerClass("CGuiProgressBar",CGuiProgressBar::Create);
}

CGuiObject *CGuiProgressBar::Create()
{
	CGuiObject *res = new CGuiProgressBar;
	
	return res;	
}

void CGuiProgressBar::init(CGuiXml *xml,xmlNodePtr node)
{
	CGuiBin::init(xml,node);
}

