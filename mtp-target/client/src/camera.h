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
// This class manages the camera
//

#ifndef MT_CAMERA_H
#define MT_CAMERA_H


//
// Includes
//

#include <nel/misc/vector.h>
#include <nel/misc/matrix.h>


//
// Namespaces
//

using NLMISC::CVector;
using NLMISC::CMatrix;


//
// Classes
//

class CCamera
{
public:
	CCamera();
	~CCamera();

	void		 setInitialPosition(const CVector &initialPosition);
	void		 setFollowedEntity(uint8 eid);
	void		 setQuakeControl(bool on);

	void		 reset();
	uint8		 getFollowedEntity();
	
	void		 update();
	bool		 updateRampe(float backDist,float height,float targetBackDist,float targetHeight);
	//void		 updateAir(float deltaTime);
	
	CMatrix		*getMatrix();
	CMatrix		*getMatrixQuake();
	CMatrix		*getMatrixFollow();

	CVector		 project(const CVector &point);

	float		 minDistFromStartPointToMove();
	void		 minDistFromStartPointToMove(float dist);
	float		 minDistFromStartPointToMoveVerticaly();
	void		 minDistFromStartPointToMoveVerticaly(float dist);
	
	float		 ForcedSpeed;

private:

	CMatrix		 MatrixFollow;
	CMatrix		 MatrixQuake;
	CMatrix		*ActiveMatrix;

	uint8		 EId;

	CVector		 InitialPosition;
	CVector		 Position;
	CVector		 LastEntityPosition;
	CVector		 CurrentLookAt;
	CVector		 Velocity;

	float		 OpenHeight;
	float		 OpenBackDist;
	float		 OpenTargetHeight;
	float		 OpenTargetBackDist;
	float		 CloseHeight;
	float		 CloseBackDist;
	float		 CloseTargetHeight;
	float		 CloseTargetBackDist;
	float		 Facing;
	float		 RotationSpeed;
	
	bool		 InverseMouse;

	float		 CurrentHeight;
	float		 CurrentBackDist;
	float		 CurrentTargetHeight;
	float		 CurrentTargetBackDist;

	float		 CurrentHeightSpeed;
	float		 MinDistFromStartPointToMove;
	float		 MinDistFromStartPointToMoveVerticaly;
	
};

#endif
