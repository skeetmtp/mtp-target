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
// This class manages an entity (player or bot)
//

#ifndef MTPT_PARTICLE
#define MTPT_PARTICLE


//
// Includes
//

#include <deque>
#include <string>

#include <nel/misc/vector.h>
#include <nel/misc/matrix.h>

#include <nel/3d/u_instance.h>
#include <nel/3d/u_particle_system_instance.h>

#include "global.h"


//
// Includes
//

#include <deque>
#include <string>

#include <nel/misc/vector.h>
#include <nel/misc/matrix.h>

#include <nel/3d/u_instance.h>
#include <nel/3d/u_particle_system_instance.h>

#include "sound_manager.h"
#include "interpolator.h"
#include "editable_element.h"
#include "../../common/particles_common.h"
#include "particles_lua_proxy.h"


//
// Namespaces
//

using NLMISC::CVector;
using NLMISC::CMatrix;


class CParticlesProxy;

//
// Classes
//

class CParticles : public CParticlesCommon
{
public:
	CParticles();
	virtual ~CParticles();
	
	virtual void enabled(bool b);

	virtual void init(const std::string &name, const std::string &fileName, uint8 id, const NLMISC::CVector &position, const NLMISC::CVector &scale, const NLMISC::CAngleAxis &rotation);
	virtual void renderSelection();
	virtual void update(const NLMISC::CVector &pos, const NLMISC::CVector &rot);
	
	virtual void position(const NLMISC::CVector &pos);

	CParticlesProxy *luaProxy;
	
private:
	NL3D::UParticleSystemInstance Particle;
};


#endif
