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


#ifndef MTPT_GUI_CUSTOM_H
#define MTPT_GUI_CUSTOM_H


//
// Includes
//
#include "gui_toc.h"
#include <vector>



//
// Classes
//

class CGuiCustom : public NLMISC::CSingleton<CGuiCustom>
{
public:
	CGuiCustom();
	virtual ~CGuiCustom();
	virtual void update();
	virtual void render(const NLMISC::CVector &pos, NLMISC::CVector &maxSize);
	CGuiToc *getTocByLuaState(lua_State *L);
	
	bool load(const std::string &path);

	void onLogin(const std::string &name);
	void onLogout(const std::string &name);

	uint32 count() {return tocList.size();};

private:
	std::vector<CGuiToc *> tocList;
};


#endif
