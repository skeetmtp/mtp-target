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
// This is the main class that manages all other classes
//

#ifndef MTPT_GUI_LISTVIEW_H
#define MTPT_GUI_LISTVIEW_H

#if 0

//
// Includes
//
#include <nel/3d/u_material.h>

#include "gui_stretched_quad.h"
#include "gui_container.h"
#include "gui_box.h"



//
// Classes
//

class CGuiList : public CGuiContainer
{
public:
	
	CGuiList();
	virtual ~CGuiList();

	virtual CGuiObject::TGuiAlignment alignment();
	virtual void alignment(int alignment);
	
	float spacing();
	void spacing(float spacing);
	
	virtual void init(CGuiXml *xml,xmlNodePtr node);

	std::deque<guiSPG<CGuiObject> > elements;		
private:	
	float _spacing;
	CGuiStretchedQuad quad;
};

class CGuiListManager : public CSingleton<CGuiListManager>
{
public:
	virtual void init();
	virtual void update() { }
	virtual void render();
	virtual void release();
	
	NL3D::UTextureFile	*texture();
	NL3D::UMaterial material();

private:
	NL3D::UTextureFile	*_texture;
	NL3D::UMaterial _material;
};

#endif
#endif
