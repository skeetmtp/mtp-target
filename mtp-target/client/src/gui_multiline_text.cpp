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

#include "3d_task.h"
#include "time_task.h"
#include "resource_manager.h"
#include "gui_multiline_text.h"
#include "gui_object.h"
#include <nel/3d/u_material.h>

//
// Namespaces
//

using namespace std;
using namespace NL3D;
using namespace NLMISC;

#define MultilineStringYSpace 3
//
// Variables
//


//
// Functions
//
	
template<class OutIt> void Split( const std::string& s, char sep, OutIt dest ) 
{
	std::string::size_type left = 0;
	std::string::size_type right = left;
	while( left <= s.size())
	{
		right=left;
		for(;right != s.size() && s[right]!=sep;right++);
		*dest = s.substr( left, right-left );
		++dest;
		if(right==s.size()) break;
		left = right+1;
	}
}




CGuiMultilineText::CGuiMultilineText()
{
}

CGuiMultilineText::~CGuiMultilineText()
{
}


void CGuiMultilineText::Printf(float x, float y, CRGBA color,bool shaded,int size,int cursorIndex, CVector &cursorPos, const char *format ...)
{
	string str;
	NLMISC_CONVERT_VARGS (str, format, 256);

	Print(x,y,color,shaded,size,cursorIndex,cursorPos,str);
}

void CGuiMultilineText::Print(float x, float y, CRGBA color,bool shaded,int size,int cursorIndex, CVector &cursorPos, string str)
{
	if(str.size()==0)
	{
		cursorPos.x = x;
		cursorPos.y = y;
		return;
	}
	C3DTask::instance().textContext().setHotSpot (UTextContext::BottomLeft);
	C3DTask::instance().textContext().setColor (color);
	C3DTask::instance().textContext().setFontSize (size);
	C3DTask::instance().textContext().setShaded(shaded);
	C3DTask::instance().textContext().setKeep800x600Ratio (true);

	UTextContext::CStringInfo stringInfo = C3DTask::instance().textContext().getStringInfo(ucstring(str));

	UTextContext::CStringInfo defaultStringInfo = C3DTask::instance().textContext().getStringInfo(ucstring("Yg"));
	float StringHeight = defaultStringInfo.StringHeight; //to find max height
	float StringLine = defaultStringInfo.StringLine;
	
	
	std::vector<std::string> vstr;
	Split(str, '\n', std::back_inserter(vstr));
	int subStringStart = 0;
	for( std::vector<std::string>::size_type i = 0; i < vstr.size(); ++i ) 
	{
		int subStringLen = vstr[i].size();
		C3DTask::instance().textContext().printAt (CGuiObject::ToProportionalX(x), 1.0f - CGuiObject::ToProportionalY(y - StringLine + StringHeight), ucstring(ucstring(vstr[i])));

		if(subStringStart <= cursorIndex && cursorIndex <= subStringStart+subStringLen)
		{
			string subSubString = vstr[i].substr(0,cursorIndex-subStringStart);
			UTextContext::CStringInfo subStringInfo = C3DTask::instance().textContext().getStringInfo(ucstring(subSubString));
			cursorPos.y = y - size + StringHeight - StringLine + 1;
			cursorPos.x = x + subStringInfo.StringWidth;
		}

		y += StringHeight + MultilineStringYSpace;
		subStringStart += subStringLen+1;
	}
	C3DTask::instance().textContext().setKeep800x600Ratio (true);
}
	

CVector CGuiMultilineText::Size(bool shaded,int size,std::string str)
{
	C3DTask::instance().textContext().setHotSpot (UTextContext::BottomLeft);
	C3DTask::instance().textContext().setFontSize (size);
	C3DTask::instance().textContext().setShaded(shaded);
	C3DTask::instance().textContext().setKeep800x600Ratio (true);

	UTextContext::CStringInfo defaultStringInfo = C3DTask::instance().textContext().getStringInfo(ucstring("Yg"));
	float StringHeight = defaultStringInfo.StringHeight; //to find max height
	float StringLine = defaultStringInfo.StringLine;
	
	UTextContext::CStringInfo stringInfo = C3DTask::instance().textContext().getStringInfo(ucstring(str));
	CVector res;
	res.y = StringHeight;
	
	std::vector<std::string> vstr;
	Split(str, '\n', std::back_inserter(vstr));
	res.y *= vstr.size();

	if(vstr.size()>1)
		res.y += MultilineStringYSpace * (vstr.size()-1);
	
	res.x = 0;

	for( std::vector<std::string>::size_type i = 0; i < vstr.size(); ++i ) 
	{
		UTextContext::CStringInfo subStringInfo = C3DTask::instance().textContext().getStringInfo(ucstring(vstr[i]));
		if(subStringInfo.StringWidth>res.x)
			res.x = subStringInfo.StringWidth;
	}
	
	return res;
}



