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

#include <nel/misc/path.h>

#include "editable_element_common.h"
#include "lua_utility.h"

//
// Namespaces
//

using namespace std;
using namespace NLMISC;


//
// Types
//


//
// Declarations
//


//
// Variables
//


//
// Functions
//

CEditableElementCommon::CEditableElementCommon()
{
	_type = Unknown;
	_changed = false;
}

CEditableElementCommon::~CEditableElementCommon()
{
	
}


bool CEditableElementCommon::isKindOf(TType type)
{
	return _type == type;
}


void CEditableElementCommon::init(const std::string &name,uint8 id, NLMISC::CVector position, NLMISC::CAngleAxis rotation)
{
	Name = name;
	Position = position;
	Rotation = rotation;
	_id = id;
	_changed = false;

	NbFaces = 0;
	Normals.clear();
	Vertices.clear();
	Indices.clear();
}


bool CEditableElementCommon::intersect(NLMISC::CVector rayStart,NLMISC::CVector rayEnd,NLMISC::CVector &rayHit,const NLMISC::CMatrix &mat)
{
	//return true;
	//CMatrix mat = mesh()->getMatrix();
	CMatrix imat = mat;
	imat.invert();
	//rayEnd = imat * rayEnd;
	//rayStart = imat * rayStart;
	
	uint32 i;
	for(i = 0; i<NbFaces; i++)
	{
		CTriangle tri;
		tri.V0 = mat * Vertices[Indices[i*3+0]];
		tri.V1 = mat * Vertices[Indices[i*3+1]];
		tri.V2 = mat * Vertices[Indices[i*3+2]];
		
		CPlane p;
		p.make(tri.V0,tri.V1,tri.V2);
		CVector hit;
		bool res = tri.intersect(rayStart,rayEnd,hit,p);
		if(res)
		{
			rayHit = hit;
			return true;
		}
	}
	
	
	return false;
}

