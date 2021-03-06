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

#ifndef MTPT_START_POINT_COMMON
#define MTPT_START_POINT_COMMON


//
// Includes
//

#include <vector>

#include "lua_utility.h"
#include "lua_nel.h"
#include "lunar.h"

#include "editable_element_common.h"

//
// Classes
//

class CStartPointCommon : public CEditableElementCommon
{
public:
	CStartPointCommon();
	virtual ~CStartPointCommon();

	virtual void init(const std::string &name, const std::string &shapeName, uint8 id, const NLMISC::CVector &position, const NLMISC::CAngleAxis &rotation);
	virtual void update(const NLMISC::CVector &pos, const NLMISC::CVector &rot);
	virtual std::string toLuaString();
	
	
};






#endif
