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

#ifndef MTPT_EDITABLE_ELEMENT_COMMON
#define MTPT_EDITABLE_ELEMENT_COMMON


//
// Includes
//

#include <vector>
#include <ode/ode.h>

#include "lua_utility.h"
#include "lua_nel.h"
#include "lunar.h"


//
// Classes
//

class CEditableElementCommon
{
public:
	enum TType
	{
		Unknown = 0,
		Module,
		StartPosition,
	};		
	
	CEditableElementCommon();
	
	virtual ~CEditableElementCommon();

	virtual void init(const std::string &name,uint8 id, NLMISC::CVector position, NLMISC::CAngleAxis rotation);
	virtual void update(NLMISC::CVector pos,NLMISC::CVector rot) = 0;
	virtual std::string toLuaString() = 0;
	virtual bool intersect(NLMISC::CVector rayStart,NLMISC::CVector rayEnd,NLMISC::CVector &rayHit,const NLMISC::CMatrix &mat);
	
	virtual NLMISC::CVector position() const {return Position;}
	virtual void position(NLMISC::CVector pos) {Position = pos; _changed = true;}

	TType type() { return _type;}
	uint8 id() {return _id;}
	const std::string	&name() const { return Name; }
	bool isKindOf(TType type);

	bool changed() {return _changed;}
	void changed(bool c) {_changed = c;}

protected:
	std::vector<NLMISC::CVector> Normals;
	std::vector<NLMISC::CVector> Vertices;
	std::vector<int>			 Indices;
	uint32 NbFaces;
	
	std::string			Name;
	std::string			ShapeName;
	NLMISC::CVector		Position;
	NLMISC::CAngleAxis	Rotation;
	bool				_changed;	
	TType				_type;
	uint8				_id;
	
};






#endif
