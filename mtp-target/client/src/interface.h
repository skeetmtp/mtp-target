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
// This class manages all the visual interface (chat, login, etc...)
//

#ifndef MT_INTERFACE_H
#define MT_INTERFACE_H

#if 0

//
// Includes
//

#include "global.h"


//
// Classes
//

class mtInterface
{
public:
	mtInterface();

	//void init();
	void render (uint8 eid);

	//void loginScreen(std::string &OutLogin,std::string &OutPassword);
	typedef enum { eDisplayNone, eDisplayBeforeFirstSession, eDisplayWaitReady, eDisplayStartSession, eDisplayGame, eDisplayEndSession, eDisplayMax } eDisplay;
	void display(eDisplay display);
	//void altimeterSetRange(float minValue, float maxValue);
	//void altimeterSetValue(float value);
	void setPartTime(float t);
//	void addChatLine(const std::string &text);
//	void updateChat();
//	void displayChat(bool large = false);
	//void network();
	//void displayBackground();
	void setDisplayViewedName(std::string name);

	std::string LevelName, FullLevelName, Author;
	std::string string1, string2;

private:
	std::string _viewedName;
	eDisplay displaySelected;

	float animTime;

	float altimeterMinValue, altimeterMaxValue;
	float altimeterValue;

	float partTime;
};

#endif
#endif
