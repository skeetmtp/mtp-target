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

#ifndef MTPT_START_POINT
#define MTPT_START_POINT


//
// Includes
//

#include <vector>
#include <ode/ode.h>

#include "../../common/start_point_common.h"

//
// Classes
//

class CStartPoint : public CStartPointCommon
{
public:
	CStartPoint();
	CStartPoint(const std::string &name, const NLMISC::CVector &position, const NLMISC::CAngleAxis &rotation, uint8 id);
	virtual ~CStartPoint();

	
	
};






#endif
