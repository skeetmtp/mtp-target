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

#if 0

#include <nel/3d/u_scene.h>
#include <nel/3d/u_driver.h>
#include <nel/3d/u_texture.h>
#include <nel/3d/u_material.h>
#include <nel/3d/u_text_context.h>

#include "3d_task.h"
#include "time_task.h"
#include "interface.h"
#include "mtp_target.h"
#include "network_task.h"
#include "font_manager.h"
#include "entity_manager.h"
#include "resource_manager.h"
#include "config_file_task.h"

//
// Namespaces
//

using namespace std;
using namespace NLMISC;
using namespace NL3D;


//
// Variables
//

//static UTexture *nelBack   = 0;
//static UTexture *nelTarget = 0;
//static UTexture *nelLogo   = 0;

//static char Login[32];
//static char Password[32];
//static int State;

//#define CHAT_LINES 3
//#define CHAT_COLUMNS 80

//static std::list<std::string> ChatText;
//static std::string ChatInput = "";
//static std::list<std::string>::reverse_iterator CurrentChatLine = ChatText.rbegin();

const int fontHeight = 20;
const int fontWidth = 22;


//
// Functions
//

/*
void mtInterface::addChatLine(const std::string &text)
{
	if (CurrentChatLine == ChatText.rbegin())
	{
		ChatText.push_back(text);
		CurrentChatLine = ChatText.rbegin();
	}
	else
	{
		ChatText.push_back(text);
	}
}
*/

/*void mtInterface::updateChat()
{
	// Get user input
	string res = C3DTask::instance().kbGetString();
	for (const char *src = res.c_str(); *src != '\0';src++)
	{
		if (*src == 27)
			continue; // Escape

		if (*src == 8)
		{ // Backspace
			if (ChatInput.length() > 0)
			{
				ChatInput.pop_back();
			}
			continue;
		}

		if (*src == '\r')
		{
			if (ChatInput.length() > 0)
			{
				if (ChatInput[0] == '/')
					CNetworkTask::instance().command(ChatInput.substr(1));
				else
					CNetworkTask::instance().chat(ChatInput);
			}
			ChatInput = "";
			continue;
		}

		if (ChatInput.size() >= CHAT_COLUMNS - 10)
			continue;

		if (isprint(*src))
		{
			ChatInput += *src;
		}
	}
	if (C3DTask::instance().kbPressed(KeyPRIOR))
	{
		std::list<std::string>::reverse_iterator it = ChatText.rend();
		sint32 i;
		// can't go back up too much
		for (i = 0; i < CHAT_LINES && it != ChatText.rbegin(); i++, --it)
		{
			// nothing
		}
		// ok, go back up
		for (i = 0; i < CHAT_LINES && CurrentChatLine != it; i++, ++CurrentChatLine)
		{
			// nothing
		}
	}
	if (C3DTask::instance().kbPressed(KeyNEXT))
	{
		// go back down
		for (sint32 i = 0; i < CHAT_LINES && CurrentChatLine != ChatText.rbegin(); i++, --CurrentChatLine)
		{
		}
	}
}*/
/*
void mtInterface::displayChat(bool large)
{
	uint cl = CHAT_LINES;
	if(large) cl *= 4;

	C3DTask::instance().driver().setFrustum(CFrustum(0, (float)C3DTask::instance().screenWidth(), 0, (float)C3DTask::instance().screenHeight(), -1, 1, false));

	// black background
	C3DTask::instance().driver().drawQuad(0.0f, (float)C3DTask::instance().screenHeight() - 16*(cl+1)-8, C3DTask::instance().screenWidth(), C3DTask::instance().screenHeight(), CRGBA (0, 0, 0, 64));

	// text
	std::list<std::string>::reverse_iterator it = CurrentChatLine;
	for (sint32 i = cl - 1; i >= 0 && it != ChatText.rend(); i--, ++it)
	{
		CFontManager::instance().littlePrintf(0.0f, (float)i, "%s", (*it).c_str());
	}
	CFontManager::instance().littlePrintf(0.0f, (float)cl, "> %s", ChatInput.c_str());
}
*/
mtInterface::mtInterface()
{
	displaySelected = eDisplayNone;
	_viewedName = "";
}

//void mtInterface::init()
//{
//	string res;
//	res = CResourceManager::instance().get("intro_background.tga");
//	nelBack   = C3DTask::instance().driver().createTextureFile (res);
//	res = CResourceManager::instance().get("intro_target.tga");
//	nelTarget = C3DTask::instance().driver().createTextureFile (res);
//	res = CResourceManager::instance().get("intro_logo.tga");
//	nelLogo   = C3DTask::instance().driver().createTextureFile (res);
//
//	Login[0] = '\0';
//	Password[0] = '\0';
//	State = 0;
//	altimeterSetRange(CConfigFileTask::instance().configFile().getVar("AltimeterMinValue").asFloat(),CConfigFileTask::instance().configFile().getVar("AltimeterMaxValue").asFloat());
//}

//void mtInterface::altimeterSetRange(float minValue, float maxValue)
//{
//	altimeterMinValue = minValue;
//	altimeterMaxValue = maxValue;
//}

//void mtInterface::altimeterSetValue(float value)
//{
//	altimeterValue = value;
//	if (altimeterValue < altimeterMinValue)
//		altimeterValue = altimeterMinValue;
//	if (altimeterValue > altimeterMaxValue)
//		altimeterValue = altimeterMaxValue;
//}

void mtInterface::setPartTime(float t)
{
	partTime = t;
}

//class cscore
//{
//public:
//	cscore(CEntity *_en, std::string _pseudo, uint32 _currentScore, uint32 _totalScore, uint16 _ping, CRGBA _color) :
//	  en(_en), pseudo(_pseudo), currentScore(_currentScore), totalScore(_totalScore), ping(_ping), color(_color) { }
//
//	CEntity *en;
//	std::string pseudo;
//	uint32 currentScore;
//	uint32 totalScore;
//	uint16 ping;
//	CRGBA color;
//};

void mtInterface::render(uint8 eid)
{
//	altimeterSetValue((eid != 255) ? CEntityManager::instance()[eid].position().z : altimeterMinValue);

	C3DTask::instance().driver().setFrustum(CFrustum(0, (float)C3DTask::instance().screenWidth(), 0, (float)C3DTask::instance().screenHeight(), -1, 1, false));
	
	if (displaySelected != eDisplayNone)
	{
		bool displaySessionInfo = false;
		char str[32];
		*str = '\0';

		float ptdt = 1.0f;
		
		switch (displaySelected)
		{
		case eDisplayWaitReady:
			strcpy(str, "Waiting other players");
			ptdt = 1.0f;
			break;
		case eDisplayBeforeFirstSession:
			strcpy(str, "Waiting for a new session");
			ptdt = 1.0f;
			break;
		case eDisplayStartSession:
			{
				displaySessionInfo = true;
				if (partTime > 5)
					strcpy(str, "Ready ?");
				else if (partTime > 0)
				{
					if (partTime > 4.8f) // don't play ready sound if it's too late
						CSoundManager::instance().playGuiSound(CSoundManager::GuiReady);
					sprintf(str, "%u", ((uint32) partTime)+1);
					ptdt = partTime - (float)(sint)partTime;
				}
				else
					nlstop;
			}
			break;
		case eDisplayGame:
			if (partTime < 2)
			{
				strcpy(str, "Go");
			}
			break;
		}
		uint32 len = strlen(str);

		float scale = 1.0f+3.0f*(1.0f-ptdt);
		CFontManager::instance().printf(CRGBA(255, 255, 255, (uint)(ptdt*255)), (C3DTask::instance().screenWidth() - len * fontWidth*scale) / 2.0f, 7.0f * fontHeight,scale, str);

		if (displaySessionInfo)
		{
			string str;
			uint32 len;
			
			str = "Name: " + LevelName;
			len = str.size();
			CFontManager::instance().printf(CRGBA(255, 255, 255, 255), 1 * fontWidth, 10.0f * fontHeight,1, str.c_str());

			str = "Title: " + FullLevelName;
			len = str.size();
			CFontManager::instance().printf(CRGBA(255, 255, 255, 255), 1 * fontWidth, 11.0f * fontHeight,1, str.c_str());
			
			str = "Author: " + Author;
			len = str.size();
			CFontManager::instance().printf(CRGBA(255, 255, 255, 255), 1 * fontWidth, 12.0f * fontHeight,1, str.c_str());

			CFontManager::instance().printf(CRGBA(245, 238, 141, 255), 1 * fontWidth, 14 * fontHeight,1, "Best score:");
			CFontManager::instance().printf(CRGBA(253, 207, 85, 255), 3 * fontWidth, 15 * fontHeight,1, string1.c_str());
			
			CFontManager::instance().printf(CRGBA(245, 238, 141, 255), 1 * fontWidth, 16 * fontHeight,1, "Best time:");
			CFontManager::instance().printf(CRGBA(253, 207, 85, 255), 3 * fontWidth, 17 * fontHeight,1, string2.c_str());
		}
/*
		float baseY = float(C3DTask::instance().screenHeight()) - 300.0f;
		float height = 244.0f;
		height = (1-(altimeterValue - altimeterMinValue) / (altimeterMaxValue - altimeterMinValue)) * height;
		float x1 = float(C3DTask::instance().screenWidth() - 40), y1 = baseY, tx1 = 15, ty1 = 8, tw1 = 34-15, th1 = 253-8;
		float x2 = float(C3DTask::instance().screenWidth() - 60), y2 = baseY - 18 + height, tx2 = 46, ty2 = 10, tw2 = 78-46, th2 = 43-10;
*/
/*
		C3DTask::instance().driver().setFrustum(CFrustum(0, (float)C3DTask::instance().screenWidth(), 0, (float)C3DTask::instance().screenHeight(), -1, 1, false));
		CFontManager::instance().material().setColor(CRGBA(255, 255, 255, 255));
	
		CQuadUV		quad;
		quad.V0.set(x1,C3DTask::instance().screenHeight()-y1,0);
		quad.V1.set(x1,C3DTask::instance().screenHeight()-y1-th1,0);
		quad.V2.set(x1+tw1,C3DTask::instance().screenHeight()-y1-th1,0);
		quad.V3.set(x1+tw1,C3DTask::instance().screenHeight()-y1,0);

		int rx1 = (int)tx1;
		int ry1 = (int)ty1;
		int rx2 = (int)tx1+(int)tw1;
		int ry2 = (int)ty1+(int)th1;

		quad.Uv0.U= rx1/256.0f;
		quad.Uv0.V= ry1/256.0f;
		quad.Uv1.U= rx1/256.0f;
		quad.Uv1.V= ry2/256.0f;
		quad.Uv2.U= rx2/256.0f;
		quad.Uv2.V= ry2/256.0f;
		quad.Uv3.U= rx2/256.0f;
		quad.Uv3.V= ry1/256.0f;
		
		C3DTask::instance().driver().drawQuad (quad, CFontManager::instance().material());

		quad.V0.set(x2,C3DTask::instance().screenHeight()-y2,0);
		quad.V1.set(x2,C3DTask::instance().screenHeight()-y2-th2,0);
		quad.V2.set(x2+tw2,C3DTask::instance().screenHeight()-y2-th2,0);
		quad.V3.set(x2+tw2,C3DTask::instance().screenHeight()-y2,0);
		
		rx1 = (int)tx2;
		ry1 = (int)ty2;
		rx2 = (int)tx2+(int)tw2;
		ry2 = (int)ty2+(int)th2;
		
		quad.Uv0.U= rx1/256.0f;
		quad.Uv0.V= ry1/256.0f;
		quad.Uv1.U= rx1/256.0f;
		quad.Uv1.V= ry2/256.0f;
		quad.Uv2.U= rx2/256.0f;
		quad.Uv2.V= ry2/256.0f;
		quad.Uv3.U= rx2/256.0f;
		quad.Uv3.V= ry1/256.0f;
		
		C3DTask::instance().driver().drawQuad (quad, CFontManager::instance().material());
*/
	}

	/*for(CEntityList::const_iterator it5=entityList->begin();it5!=entityList->end();++it5)
	{
		CEntity *entity = *it5;
		if(entity != controledEntity)
		{
			CVector position = CMtpTarget::instance().controler().camera.project(entity->getPosition());
	#define MAX_DIST_DISPLAY_NAME 500.0f
			if(0.0f < position.z && position.z < MAX_DIST_DISPLAY_NAME)
			{
				float scale = 0.2f + 0.5f * (MAX_DIST_DISPLAY_NAME - position.z) / MAX_DIST_DISPLAY_NAME;
				float width = (entity->name.length() * fontWidth * scale) * 0.5f;
				CFontManager::instance().printf(CRGBA(255, 253, 255, 255), 640 - position.x - width,480 - position.y ,scale,entity->name.c_str());
			}
		}
	}*/
	

	if (eid != 255)
	{
		// display our score (bottom right)
		string totalScoreStr = toString("score %u",CEntityManager::instance()[eid].totalScore());
		CFontManager::instance().printf(CRGBA(245, 238, 141, 255), (float) (C3DTask::instance().screenWidth() - totalScoreStr.size() * fontWidth - 10), float(C3DTask::instance().screenHeight() - 1 * fontHeight), 1, totalScoreStr.c_str());

		if (CEntityManager::instance()[eid].interpolator.outOfKey())
			CFontManager::instance().printf(CRGBA(255, 0, 0, 255), float(C3DTask::instance().screenWidth() / 2 - 70), 70.0f,1, "NET LAG");
	}
	
	if (mtpTarget::instance().TimeBeforeTimeout < 0)
		mtpTarget::instance().TimeBeforeTimeout = 0;
	string timeBeforeTimeoutStr = toString("Time left %u",(uint)mtpTarget::instance().TimeBeforeTimeout);
	CFontManager::instance().printf(CRGBA(245, 238, 141, 255), 10.0f, (float)(C3DTask::instance().screenHeight() - 20),1,timeBeforeTimeoutStr.c_str() );
	
	CFontManager::instance().printf(CRGBA(255,255,255,255),(C3DTask::instance().screenWidth() - _viewedName.size() * fontWidth) / 2.0f,float(C3DTask::instance().screenHeight() - 2 * fontHeight),1,_viewedName.c_str());

	//updateChat();

	bool displayScoreForced = (displaySelected != eDisplayEndSession) && C3DTask::instance().kbDown(KeyTAB);
	if (displaySelected == eDisplayEndSession || displayScoreForced)
	{
		C3DTask::instance().driver().drawQuad(0.0f, 0.0f, C3DTask::instance().screenWidth() , C3DTask::instance().screenHeight(), CRGBA (0, 0, 0, 200));
		
		// display all players score
		float x1 = 15.0f;
		float x2 = 250.0f;
		float x3 = 500.0f;
		float x4 = 680.0f;
		float y = 100.0f;
		CFontManager::instance().printf(CRGBA(245, 238, 141), x1, y, 1, "name");
		CFontManager::instance().printf(CRGBA(245, 238, 141), x2, y, 1, "score");
		CFontManager::instance().printf(CRGBA(245, 238, 141), x3, y, 1, "total");
		CFontManager::instance().printf(CRGBA(245, 238, 141), x4, y, 1, "ping");
		y += fontHeight+10;

		vector<uint8> eids;
		CEntityManager::instance().getEIdSortedByScore(eids);

		for(uint i = 0; i < eids.size(); i++, y += fontHeight)
		{
			CFontManager::instance().printf(CEntityManager::instance()[i].color(), x1, y, 1, "%s%s", CEntityManager::instance()[i].name().c_str(), (CEntityManager::instance()[i].spectator()?" :spec:":""));
			if (!displayScoreForced)
				CFontManager::instance().printf(CEntityManager::instance()[i].color(), x2, y, 1, "%d", CEntityManager::instance()[i].currentScore());
			CFontManager::instance().printf(CEntityManager::instance()[i].color(), x3, y, 1, "%d", CEntityManager::instance()[i].totalScore());
			CFontManager::instance().printf(CEntityManager::instance()[i].color(), x4, y, 1, "%u", CEntityManager::instance()[i].ping());
		}
	}	

//	displayChat();
}

void mtInterface::display(eDisplay _display)
{
	if (displaySelected != _display)
	{
		displaySelected = _display;
		animTime = 0;
	}
}

//void mtInterface::loginScreen(std::string &OutLogin,std::string &OutPassword)
//{
//	if (OutLogin != "")
//	{
//		strcpy(Login, OutLogin.c_str());
//		strcpy(Password, OutPassword.c_str());
//		if (Password != "")
//			State = 1;
//	}
//
//	while (true)
//	{
////		mtpTarget::instance().updateTime();
//
//		if(C3DTask::instance().kbDown(KeyRETURN))
//			return;
//
//		if (C3DTask::instance().kbPressed(KeyESCAPE))
//			break;
//		
//		C3DTask::instance().clear();
//
//		//mt3dUpdate();
//
//		displayBackground();
//
//		float t = CTimeTask::instance().time();
//		uint8 intensity = (uint8) (((1 + (float)cos(t * 5)) * 0.5f * 64 + 192));
//
//		CFontManager::instance().printf(CRGBA(intensity, intensity, intensity, 0xFF), 150.0f, C3DTask::instance().screenHeight()-160.0f, 1.0f, "press enter to start");
//
////		mt3dRender ();
////		mt3dSwap();
//	}
//	exit(0);
//}

//#define LOGO_SCALE 1.5f
//#define BACK_SIZE 64.0f
//void mtInterface::displayBackground()
//{
//	C3DTask::instance().driver().setMatrixMode2D11 ();
//	C3DTask::instance().driver().setMatrixMode2D (CFrustum(0,640,0,480,-1,1,false));
//	
//	float t = CTimeTask::instance().time();
//	
//	float dx = (float)(70.0f * cos(sin(t*1.2f+0.5f))) - 128.0f;
//	float dy = (float)(60.0f * cos(cos(t*0.9f+3.0f))) - 128.0f;
//	for (uint32 j=0;j<10;j++)
//	{
//		float y = -0.5f + j * BACK_SIZE + dy;
//		for (uint32 i=0;i<15;i++)
//		{
//			float x = -0.5f + i * BACK_SIZE + dx;
//			C3DTask::instance().driver().drawBitmap (x, y, BACK_SIZE, BACK_SIZE, *(UTexture*)nelBack);
//		}
//	}
//
//	dx = (float)(200.0f * cos(t*0.512345f+0.57733584f)*sin(t*0.71874f+0.1248721f)) - 128.0f;
//	dy = (float)(150.0f * cos(t*0.189713f+0.48754118f)*cos(t*0.91871f+3.6873738f)) - 128.0f;
//	C3DTask::instance().driver().drawBitmap (20+250+dx,480-256-50+dy-20, LOGO_SCALE*256.0f, LOGO_SCALE*256.0f, *(UTexture*)nelTarget, true, CRGBA(0,0,0,100));
//	C3DTask::instance().driver().drawBitmap (250+dx,480-256-50+dy, LOGO_SCALE*256.0f, LOGO_SCALE*256.0f, *(UTexture*)nelTarget);
//	C3DTask::instance().driver().drawBitmap (20+150, 480-356-50-20, LOGO_SCALE*256, LOGO_SCALE*256, *(UTexture*)nelLogo, true, CRGBA(0,0,0,100));
//	C3DTask::instance().driver().drawBitmap (150, 480-356-50, LOGO_SCALE*256, LOGO_SCALE*256, *(UTexture*)nelLogo, true, CRGBA(255,255,255,230));
//}

//void mtInterface::network()
//{
//	float oldTime = CTimeTask::instance().time();
//	bool first = true;
//
//	std::string errorMsg1;
//	std::string errorMsg2;
//	while (true)
//	{
////		mtpTarget::instance().updateTime();
//
//		if (C3DTask::instance().kbPressed(KeyESCAPE))
//			exit(0);
//
//		if (C3DTask::instance().kbPressed(KeyRETURN))
//		{
//			errorMsg1 = CNetworkTask::instance().connect();
//			if (errorMsg1.empty())
//				return;
//
//			unsigned int index = errorMsg1.find("Socket error: ");
//			if (index == string::npos)
//				errorMsg2 = "";
//			else
//			{
//				errorMsg2 = errorMsg1.substr(index + strlen("Socket error: "));
//				errorMsg1[index + strlen("Socket error: ")] = '\0';
//			}
//			first = false;
//			oldTime = CTimeTask::instance().time();
//		}
//
//		C3DTask::instance().clear();
////		mt3dClear ();
//
////		mt3dUpdate();
//
//		displayBackground();
//
//		char *text1;
//		char *text2;
//		if (first)
//		{
//			text1 = "press enter to connect";
//			text2 = "to server";
//		}
//		else
//		{
//			text1 = "press enter to retry";
//			text2 = "to connect to server";
//		}
//
//		uint32 len1 = strlen(text1);
//		CFontManager::instance().printf (CRGBA(0xff, 0xff, 0xff, 0xFF), 320.0f - len1/2 * 20, C3DTask::instance().screenHeight() - 200.0f, 1.0f, text1);
//		uint32 len2 = strlen(text2);
//		CFontManager::instance().printf(CRGBA(0xff, 0xff, 0xff, 0xFF), 320.0f - len2/2 * 20, C3DTask::instance().screenHeight() - 200.0f+25.0f, 1.0f, text2);
//
//		if (oldTime - CTimeTask::instance().time() < 5.0f)
//		{
//			CFontManager::instance().littlePrintf (3.0f, 29.0f, (char *) errorMsg1.c_str());
//			CFontManager::instance().littlePrintf (3.0f, 30.0f, (char *) errorMsg2.c_str());
//		}
//
////		mt3dRender ();
////		mt3dSwap();
//	}
//	exit(0);
//}

void mtInterface::setDisplayViewedName(std::string name)
{
	_viewedName = name;
}

#endif