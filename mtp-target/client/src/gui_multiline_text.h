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

#ifndef MTPT_GUI_MULTILINE_TEXT_H
#define MTPT_GUI_MULTILINE_TEXT_H


//
// Includes
//
#include <nel/3d/u_text_context.h>


template<class OutIt> void Split( const std::string& s, char sep, OutIt dest ) ;

//
// Classes
//

class CGuiMultilineText
{
public:

	CGuiMultilineText();
	virtual ~CGuiMultilineText();

	static void Printf(float x, float y, int cursorIndex, NLMISC::CVector &cursorPos, const char *format ...);
	static void Print(float x, float y, int cursorIndex, NLMISC::CVector &cursorPos, const std::string &str);
	static NLMISC::CVector Size(bool shaded,int size, const std::string &str);
		
	
private:
	
};


#endif
