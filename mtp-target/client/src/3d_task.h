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

#ifndef MTPT_3D_TASK_H
#define MTPT_3D_TASK_H


//
// Includes
//

#include <nel/3d/u_scene.h>
#include <nel/3d/u_instance.h>
#include <nel/3d/u_text_context.h>
#include <nel/3d/u_texture.h>

#include "mouse_listener.h"


//
// Classes
//

class C3DTask : public CSingleton<C3DTask>, public ITask
{
public:
	
	virtual void init();
	virtual void update();
	virtual void render();
	virtual void release();

	virtual std::string name() { return "C3DTask"; }
	
	NL3D::UDriver		&driver() const { nlassert(Driver); return *Driver; }
	NL3D::UScene		&scene() const { nlassert(Scene); return *Scene; }
	NL3D::UTextContext	&textContext() const { nlassert(TextContext); return *TextContext; }
	C3dMouseListener	&mouseListener() const { nlassert(MouseListener); return *MouseListener; }

	uint16				screenWidth() const { return ScreenWidth; }
	uint16				screenHeight() const { return ScreenHeight; }
	
	bool				kbPressed(NLMISC::TKey key) const;
	bool				kbDown(NLMISC::TKey key) const;
	std::string			kbGetString() const;

	void				clear();
		
	void				captureCursor(bool b);

private:
	
	NL3D::UDriver			*Driver;
	NL3D::UTextContext		*TextContext;
	NL3D::UScene			*Scene;
	
	uint16					 ScreenWidth;
	uint16					 ScreenHeight;
	NLMISC::CRGBA			 AmbientColor;
	NLMISC::CRGBA			 ClearColor;
	C3dMouseListener		*MouseListener;
	NL3D::UInstance			*LevelParticle;
	//bool					 CaptureCursor;
};

#endif
