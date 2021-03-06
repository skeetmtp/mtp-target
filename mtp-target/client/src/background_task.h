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

#ifndef MTPT_BACKGROUND_TASK_H
#define MTPT_BACKGROUND_TASK_H


//
// Includes
//



//
// Classes
//

class CBackgroundTask : public NLMISC::CSingleton<CBackgroundTask>, public ITask
{
public:

	virtual void init();
	virtual void update();
	virtual void render();
	virtual void release();

	virtual std::string name() const { return "CBackgroundTask"; }
	
private:

	float _dx1;
	float _dy1;
	float _dx2;
	float _dy2;
	
	NL3D::UTextureFile	*Background;
	NL3D::UTextureFile	*Target;
	NL3D::UTextureFile	*Logo;
	NL3D::UInstance		LogoParticle;
};

#endif
