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

#ifndef MTPT_EDITABLE_ELEMENT
#define MTPT_EDITABLE_ELEMENT


//
// Includes
//

#include <vector>
#include <ode/ode.h>

#include "../../common/lua_utility.h"
#include "../../common/lua_nel.h"
#include "../../common/lunar.h"


//
// Classes
//

class CEditableElement
{
public:
	enum TType
	{
		Unknown = 0,
		Module,
		StartPosition,
	};		
	
	CEditableElement();
	
	virtual ~CEditableElement();

	bool changed();
	TType type() { return _type;}
	NLMISC::CVector position() const {return Position;}

	virtual void update(NLMISC::CVector pos,NLMISC::CVector rot) = 0;
	virtual std::string &toLuaString() = 0;
protected:
	
	NLMISC::CVector		Position;
	NLMISC::CAngleAxis	Rotation;
	bool				_changed;	
	TType _type;
	
};






#endif
