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
// This class controles the keyboard input
//

#ifndef MT_CONTROLER_H
#define MT_CONTROLER_H


//
// Includes
//

#include <nel/misc/vector.h>

#include "camera.h"
#include "entity.h"


//
// Classes
//

class CControler
{
public:
	CControler();
	~CControler();

	void	swapOpenClose();
	void	setControledEntity(uint8 eid);
	uint8	getControledEntity() const;
	void	update();
	void	reset();
	
	CCamera		Camera;
	
	uint8		ViewedEId;	// the id of the entity that we currently view (followed)

private:

	uint8		EId;

	NLMISC::CVector Accel;

	float			Pique;
	float			RotZ;

	float			VirtualFrameTime;
	bool			FirstFrame;

	float			MissedTime;
};

#endif
