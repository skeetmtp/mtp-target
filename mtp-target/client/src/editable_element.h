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

#ifndef MTPT_EDITABLE_ELEMENT
#define MTPT_EDITABLE_ELEMENT


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
#include "../../common/editable_element_common.h"


//
// Namespaces
//

using NLMISC::CVector;
using NLMISC::CMatrix;


//
// Classes
//

class CEditableElement
{
public:
	CEditableElement();
	virtual ~CEditableElement();

	//todo merge with common update()
	//void update(NLMISC::CVector pos,NLMISC::CVector rot,uint32 selectedBy);

	//virtual void init(const std::string &name,uint8 id, NLMISC::CVector position, NLMISC::CAngleAxis rotation);

	NL3D::UInstance *mesh();
	virtual void renderSelection();

	void show();
	void hide();

	//virtual void position(NLMISC::CVector pos);

protected:
	NL3D::UInstance *Mesh;
	NL3D::UMaterial *mat;
};


#endif
