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

#ifndef MTPT_GUI_BOX_H
#define MTPT_GUI_BOX_H


//
// Includes
//
#include "gui_stretched_quad.h"
#include "gui_container.h"



//
// Classes
//

class CGuiBox : public CGuiContainer
{
public:
	
	CGuiBox();
	virtual ~CGuiBox();

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

class CGuiVBox : public CGuiBox
{
public:
	
	CGuiVBox();
	virtual ~CGuiVBox();
	
	virtual void _render(NLMISC::CVector pos,NLMISC::CVector &maxSize);	
	
	virtual float _width();
	virtual float _height();

	static void XmlRegister();
	static CGuiObject *Create();
	virtual void init(CGuiXml *xml,xmlNodePtr node);

private:
	
};

class CGuiHBox : public CGuiBox
{
public:
	
	CGuiHBox();
	virtual ~CGuiHBox();
	
	virtual void _render(NLMISC::CVector pos,NLMISC::CVector &maxSize);	
	
	virtual float _width();
	virtual float _height();

	static void XmlRegister();
	static CGuiObject *Create();
	virtual void init(CGuiXml *xml,xmlNodePtr node);
private:
	
};

class CGuiBoxManager : public CSingleton<CGuiBoxManager>
{
public:
	virtual void init();
	virtual void update() { }
	virtual void render();
	virtual void release();
	
	NL3D::UTextureFile	*texture();
	NL3D::UMaterial *material();
	
	
private:
	NL3D::UTextureFile	*_texture;
	NL3D::UMaterial *_material;
	
};

#endif
