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
#include "font_manager.h"
#include "gui_text.h"
#include "gui_multiline_text.h"
#include "gui_stretched_quad.h"
#include "gui_xml.h"


//
// Namespaces
//

using namespace std;
using namespace NL3D;
using namespace NLMISC;


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



void CGuiTextManager::init()
{
	string res;
	res = CResourceManager::instance().get("cursor.tga");
	_cursorTexture = C3DTask::instance().driver().createTextureFile(res);
	nlassert(_cursorTexture);
	_cursorTexture->setWrapS(UTexture::Clamp);
	_cursorTexture->setWrapT(UTexture::Clamp);
	
	_cursorMaterial = C3DTask::instance().driver().createMaterial ();
	_cursorMaterial.setTexture(_cursorTexture);
	_cursorMaterial.setBlend(true);
	_cursorMaterial.setZFunc(UMaterial::always);
	_cursorMaterial.setDoubleSided();
	
	res = CResourceManager::instance().get("entry.tga");
	_entryTexture = C3DTask::instance().driver().createTextureFile(res);
	nlassert(_entryTexture);
	_entryTexture->setWrapS(UTexture::Clamp);
	_entryTexture->setWrapT(UTexture::Clamp);
	
	_entryMaterial = C3DTask::instance().driver().createMaterial ();
	_entryMaterial.setTexture(_entryTexture);
	_entryMaterial.setBlend(true);
	_entryMaterial.setZFunc(UMaterial::always);
	_entryMaterial.setDoubleSided();
	
	CGuiText::XmlRegister();
	CGuiTextPercent::XmlRegister();
}
	
void CGuiTextManager::render()
{
}

void CGuiTextManager::release()
{
	
}


UMaterial CGuiTextManager::cursorMaterial()
{
	return _cursorMaterial;
}

UMaterial CGuiTextManager::entryMaterial()
{
	return _entryMaterial;
}

//
//
//
int CGuiTextCursor::_FrameCount = 0;

void CGuiTextCursor::Reset()
{
	_FrameCount = 0;
}

void CGuiTextCursor::Render(const CVector &position,int height)
{
	_FrameCount++;
	if(_FrameCount<30)
	{
		CGuiStretchedQuad quad;
		quad.stretched(false);
		quad.material(CGuiTextManager::instance().cursorMaterial());
		quad.size(CVector(2,(float)height,0));
		quad.position(position);
		quad.render();
	}
	
	_FrameCount = _FrameCount % 60;
	
}


//
//
//

void CGuiText::_init(const string &text)
{
	this->text = text;
	cursorIndex(0);
	_isEditable  = false;
	_isEntry     = false;	
	_isPassword  = false;
	_isMultiline = true;
	_fontSize    = 12;
}

CGuiText::CGuiText(const string &text)
{
	_init(text);
}

CGuiText::CGuiText()
{
	_init("");
}

CGuiText::~CGuiText()
{
	
}

void CGuiText::insert(char c)
{
	string insertedString;
	insertedString += c;
	text = text.substr(0,cursorIndex()) + insertedString + text.substr(cursorIndex(),text.size());
	cursorIndex(cursorIndex()+1);
	CGuiTextCursor::Reset();	
}

static string strToPassword(string str,bool active)
{
	if(!active) return str;

	string res = "";
	for(uint i=0;i<str.size();i++)
	{
		if(str[i]!='\n')
			res+='*';
		else
			res+='\n';
	}
	return res;
}

static string strToMultiline(string str,bool active)
{
	if(active) return str;

	string res = "";
	for(uint i=0;i<str.size();i++)
	{
		if(str[i]!='\n')
			res+=str[i];
	}
	return res;
}

void CGuiText::_render(const CVector &pos,CVector &maxSize)
{
	CVector globalPos = globalPosition(pos,maxSize);
	CVector cursorPos(0,0,0);

	CVector expSize = expandSize(maxSize);

	if(isEntry())
	{
		CGuiStretchedQuad quad;
		quad.material(CGuiTextManager::instance().entryMaterial());
		quad.size(expSize);
		quad.position(globalPosition(pos,maxSize));
		quad.render();
	}

	if(isEntry())
		globalPos += CVector(2,2,0);

	text = strToMultiline(text,isMultiline());
	CGuiMultilineText::Print(globalPos.x , globalPos.y, CRGBA(0,0,0,255), false, _fontSize, cursorIndex(), cursorPos, strToPassword(text,isPassword()));

		
	if(focused() && isEntry())
	{
		CGuiTextCursor::Render(cursorPos,_fontSize);
		if(isEditable())
		{
			string res = C3DTask::instance().kbGetString();
			if(res.size())
			{
				for (const char *src = res.c_str(); *src != '\0';src++)
				{
					if (*src == 8)
					{ // Backspace
						if (text.length() > 0 && cursorIndex()>0)
						{
							text = text.substr(0,cursorIndex()-1) + text.substr(cursorIndex(),text.size());
							cursorIndex(cursorIndex()-1);
						}
						continue;
					}

					if(isprint(*src))
						insert(*src);
					if((*src)==13 && isMultiline())
						insert('\n');
				}
			}
			
			if (C3DTask::instance().kbPressed(KeyDELETE))
			{
				if (text.length() > 0 && cursorIndex()<text.size())
				{
					text = text.substr(0,cursorIndex()) + text.substr(cursorIndex()+1,text.size());
				}
			}

			if (C3DTask::instance().kbPressed(KeyUP))
				cursorUp();
			if (C3DTask::instance().kbPressed(KeyDOWN))
				cursorDown();
			if (C3DTask::instance().kbPressed(KeyLEFT))
				cursorLeft();
			if (C3DTask::instance().kbPressed(KeyRIGHT))
				cursorRight();
			if (C3DTask::instance().kbPressed(KeyHOME))
				cursorHome();
			if (C3DTask::instance().kbPressed(KeyEND))
				cursorEnd();
		}
	}


	maxSize = expSize;
	
}


void CGuiText::cursorHome()
{
	std::vector<std::string> vstr;
	Split(text, '\n', std::back_inserter(vstr));
	
	uint pos = 0;
	uint column;
	uint line;
	uint home;
	for(uint j=0;j<vstr.size();j++)
	{
		home = pos;
		for(uint i=0;i<=vstr[j].size();i++,pos++)
		{
			if( pos==cursorIndex() )
			{
				column = i;
				line = j;
				cursorIndex(home);
				return;
			}
		}
	}
	
}

void CGuiText::cursorEnd()
{
	std::vector<std::string> vstr;
	Split(text, '\n', std::back_inserter(vstr));
	
	uint pos = 0;
	uint column;
	uint line;
	uint home;
	for(uint j=0;j<vstr.size();j++)
	{
		home = pos;
		for(uint i=0;i<=vstr[j].size();i++,pos++)
		{
			if( pos==cursorIndex() )
			{
				column = i;
				line = j;
				cursorIndex(home + vstr[j].size());
				return;
			}
		}
	}
	
}

void CGuiText::cursorUp()
{
	std::vector<std::string> vstr;
	Split(text, '\n', std::back_inserter(vstr));

	uint pos = 0;
	uint column;
	uint line;
	for(uint j=0;j<vstr.size();j++)
	{
		for(uint i=0;i<=vstr[j].size();i++,pos++)
		{
			if( pos==cursorIndex() )
			{
				column = i;
				line = j;
			}
		}
	}
	
	pos=0;
	for(uint j=0;j<vstr.size();j++)
	{
		if( j==(line-1) )
		{
			if( column>vstr[j].size() )
				cursorIndex(pos + vstr[j].size());
			else
				cursorIndex(pos + column);
		}
		else
			pos += vstr[j].size()+1;
	}
}


void CGuiText::cursorDown()
{
	std::vector<std::string> vstr;
	Split(text, '\n', std::back_inserter(vstr));
	
	uint pos = 0;
	uint column;
	uint line;
	for(uint j=0;j<vstr.size();j++)
	{
		for(uint i=0;i<=vstr[j].size();i++,pos++)
		{
			if( pos==cursorIndex() )
			{
				column = i;
				line = j;
			}
		}
	}
	
	pos=0;
	for(uint j=0;j<vstr.size();j++)
	{
		if( j==(line+1) )
		{
			if( column>vstr[j].size() )
				cursorIndex(pos + vstr[j].size());
			else
				cursorIndex(pos + column);
		}
		else
			pos += vstr[j].size()+1;
	}
}

void CGuiText::cursorLeft()
{
	if(cursorIndex()>0)
	{
		cursorIndex(cursorIndex()-1);
		CGuiTextCursor::Reset();				
	}
}

void CGuiText::cursorRight()
{
	cursorIndex(cursorIndex()+1);
	if(cursorIndex()>text.size())
		cursorIndex(text.size());
	else
		CGuiTextCursor::Reset();				
}



float CGuiText::_width()
{
	float w = CGuiMultilineText::Size(false, _fontSize,text).x;
	if(isEntry())
		return w + 4;
	else
		return w;
}

float CGuiText::_height()
{
	float h = CGuiMultilineText::Size(false, _fontSize,text).y;
	if(isEntry())
		return h + 4;
	else
		return h;
}


void CGuiText::isEditable(bool isEditable)
{
	_isEditable = isEditable;
}

bool CGuiText::isEditable()
{
	return _isEditable;
}

void CGuiText::isEntry(bool isEntry)
{
	_isEntry = isEntry;
}

bool CGuiText::isEntry()
{
	return _isEntry;
}

void CGuiText::isPassword(bool isPassword)
{
	_isPassword = isPassword;
}

bool CGuiText::isPassword()
{
	return _isPassword;
}

void CGuiText::isMultiline(bool isMultiline)
{
	_isMultiline = isMultiline;
}

bool CGuiText::isMultiline()
{
	return _isMultiline;
}

void CGuiText::fontSize(uint fontSize)
{
	_fontSize = fontSize;
}

uint CGuiText::fontSize()
{
	return _fontSize;
}

void CGuiText::cursorIndex(int cursorIndex)
{
	uint ucursorIndex = cursorIndex;
	if(cursorIndex<0)
		ucursorIndex = 0;
	if(_cursorIndex!=ucursorIndex)
	{
		_cursorIndex = ucursorIndex;
		CGuiTextCursor::Reset();				
	}
}

uint CGuiText::cursorIndex()
{
	return _cursorIndex;
}


void CGuiText::XmlRegister()
{
	CGuiObjectManager::instance().registerClass("CGuiText",CGuiText::Create);
}

CGuiObject *CGuiText::Create()
{
	CGuiObject *res = new CGuiText;
	
	return res;	
}

void CGuiText::init(CGuiXml *xml,xmlNodePtr node)
{
	CGuiObject::init(xml,node);
	xml->getString(node,"string",text);
	bool b;
	if(xml->getBool(node,"isEntry",b))
		isEntry(b);
	if(xml->getBool(node,"isEditable",b))
		isEditable(b);
	if(xml->getBool(node,"isPassword",b))
		isPassword(b);
	if(xml->getBool(node,"isMultiline",b))
		isMultiline(b);
}




//
//
//


CGuiTextPercent::CGuiTextPercent():CGuiText()
{
	_ptrValue = NULL;
}

CGuiTextPercent::~CGuiTextPercent()
{
	
}

void CGuiTextPercent::_render(const CVector &pos,CVector &maxSize)
{
	char ch[256];
	if(_ptrValue)
		sprintf(ch,"%.0f%%",*_ptrValue * 100);
	else
		sprintf(ch,"0%");
	text = ch;
	CGuiText::_render(pos,maxSize);
}

void CGuiTextPercent::ptrValue(float *ptrValue)
{
	_ptrValue = ptrValue;
}

void CGuiTextPercent::XmlRegister()
{
	CGuiObjectManager::instance().registerClass("CGuiTextPercent",CGuiTextPercent::Create);
}

CGuiObject *CGuiTextPercent::Create()
{
	CGuiObject *res = new CGuiTextPercent;
	
	return res;	
}

void CGuiTextPercent::init(CGuiXml *xml,xmlNodePtr node)
{
	CGuiText::init(xml,node);
}

