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
// This class manages the mouse input
//

#ifndef MTPT_GUI_MOUSE_LISTENER_H
#define MTPT_GUI_MOUSE_LISTENER_H


//
// Includes
//

#include <nel/misc/types_nl.h>
#include <nel/misc/event_listener.h>
#include <nel/misc/vector.h>
#include <nel/misc/matrix.h>
#include <nel/misc/time_nl.h>

#include <nel/3d/u_camera.h>
#include <nel/3d/viewport.h>
#include <nel/3d/frustum.h>
#include <nel/3d/u_3d_mouse_listener.h>


//
// Classes
//

/**
 * CGuiMouseListener is a listener that handle a 3d matrix with mouse events.
 * This can be the view matrix, or the matrix of any object.
 */
class CGuiMouseListener : public NLMISC::IEventListener
{
public:
	/** 
	  * Constructor. 
	  */
	CGuiMouseListener();

	virtual ~CGuiMouseListener();

	void init ();
	void update();

	/** 
	  * Register the listener to the server.
	  */
	void addToServer (NLMISC::CEventServer& server);

	/** 
	  * Unregister the listener to the server.
	  */
	void removeFromServer (NLMISC::CEventServer& server);

	void captureCursor(bool b) {_captureCursor = b;};
	
	bool	ButtonDown;
	bool	RightButtonDown;
	bool	Pressed;
	bool	Clicked;
	double  LastClickedTime;
	bool	DoubleClicked;
	float	MouseX, MouseY;
	float	PressedX, PressedY;
	sint	MouseWheel;

	NLMISC::CVector position();
	NLMISC::CVector pressedPosition();
	
private:
	/// Internal use
	virtual void operator ()(const NLMISC::CEvent& event);
private:
	bool    LastButtonDown;   
	bool    _captureCursor;
	bool    _cursorCaptured;
	
	NLMISC::CEventListenerAsync	_AsyncListener;
};


#endif
