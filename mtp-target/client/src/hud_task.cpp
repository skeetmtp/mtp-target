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
#include "hud_task.h"
#include "time_task.h"
#include "game_task.h"
#include "chat_task.h"
#include "mtp_target.h"
#include "network_task.h"
#include "font_manager.h"
#include "task_manager.h"
#include "entity_manager.h"
#include "background_task.h"
#include "config_file_task.h"
#include "resource_manager.h"
#include "level_manager.h"


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
	
void CHudTask::init()
{
	AltimeterMinValue = CConfigFileTask::instance().configFile().getVar("AltimeterMinValue").asFloat();
	AltimeterMaxValue = CConfigFileTask::instance().configFile().getVar("AltimeterMaxValue").asFloat();
}

void CHudTask::update()
{
	// update altimeter value
	uint8 eid = CMtpTarget::instance().controler().getControledEntity();
	AltimeterValue = ((eid != 255) ? CEntityManager::instance()[eid].interpolator().position().z : AltimeterMinValue);
	AltimeterValue = min(AltimeterValue, AltimeterMaxValue);
	AltimeterValue = max(AltimeterValue, AltimeterMinValue);
}

void CHudTask::render()
{
	C3DTask::instance().driver().setFrustum(CFrustum(0, (float)C3DTask::instance().screenWidth(), 0, (float)C3DTask::instance().screenHeight(), -1, 1, false));
	
	string str;

	float ptdt = 1.0f;
	bool displaySessionInfo = false;
	float partTime = CMtpTarget::instance().timeBeforeSessionStart();
	
	if(CMtpTarget::instance().State == CMtpTarget::eStartSession)
	{		
		str = "Waiting other players";
		ptdt = 1.0f;
	}
	if(CMtpTarget::instance().State == CMtpTarget::eBeforeFirstSession)
	{
		str = "Waiting for a new session";
		ptdt = 1.0f;
	}
	if(CMtpTarget::instance().State == CMtpTarget::eReady)
	{
		displaySessionInfo = true;
		if (partTime > 5.0f)
			str = "Ready ?";
		else if (partTime > 0.0f)
		{
			if (partTime > 4.8f) // don't play ready sound if it's too late
				CSoundManager::instance().playGuiSound(CSoundManager::GuiReady);
			str = toString("%u", ((uint32) partTime)+1);
			ptdt = partTime - (float)(sint)partTime;
		}
		else
			nlstop;
	}
	if(CMtpTarget::instance().State == CMtpTarget::eGame)
	{
		if (partTime < 2.0f)
		{
			str = "Go";
		}
	}
	float scale = 1.0f+3.0f*(1.0f-ptdt);
	CFontManager::instance().printf(CRGBA(255, 255, 255, (uint)(ptdt*255)), (C3DTask::instance().screenWidth() - str.size() * CFontManager::instance().fontWidth()*scale) / 2.0f, 7.0f * CFontManager::instance().fontHeight(),scale, str.c_str());

	list<CHudMessage>::iterator it;
	list<CHudMessage>::iterator it2delete;
	for(it=messages.begin();it!=messages.end();)
	{
		CHudMessage m = *it;
		double time = CTimeTask::instance().time();
		if(it->endTime!=0.0 && time>it->endTime)
		{
			it2delete = it;
			it++;
			messages.erase(it2delete);
		}
		else
		{
			CFontManager::instance().printf(it->col,it->scale * it->x * CFontManager::instance().fontWidth(),it->scale * it->y * CFontManager::instance().fontHeight(),it->scale, it->message.c_str());			
			it++;
		}
	}
	
/*
	if (CMtpTarget::instance().State == CMtpTarget::eStartSession)
	{
		displaySelected = mtInterface::eDisplayWaitReady;
	}
	if (CMtpTarget::instance().State == CMtpTarget::eReady)
	{
		CMtpTarget::instance().timeBeforeSessionStart -= CTimeTask::instance().deltaTime();
		displaySelected = mtInterface::eDisplayStartSession;
		partTime = CMtpTarget::instance().timeBeforeSessionStart;
		
		if (CMtpTarget::instance().timeBeforeSessionStart > 4.0f)
		{
			CMtpTarget::instance().controler().Camera.reset();
			CMtpTarget::instance().controler().Camera.ForcedSpeed = 0.0f;
		}
		else
			CMtpTarget::instance().controler().Camera.ForcedSpeed = 1.0f;
		
		if (CMtpTarget::instance().timeBeforeSessionStart <= 0)
		{
			CMtpTarget::instance().timeBeforeSessionStart = 0;
			CMtpTarget::instance().State = CMtpTarget::eGame;
		}
	}
	if (CMtpTarget::instance().State == CMtpTarget::eGame)
	{
		//CMtpTarget::instance().controler().Camera.reset();
		CMtpTarget::instance().timeBeforeSessionStart += CTimeTask::instance().deltaTime();
		TimeBeforeTimeout -= CTimeTask::instance().deltaTime();
		Interface2d.setPartTime(CMtpTarget::instance().timeBeforeSessionStart);
		Interface2d.display(mtInterface::eDisplayGame);
	}
	if (CMtpTarget::instance().State == CMtpTarget::eEndSession)
	{
		Interface2d.display(mtInterface::eDisplayEndSession);
	}
	

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
		*/

	
		if (displaySessionInfo)
		{
			float fontWidth  = (float )CFontManager::instance().fontWidth();
			float fontHeight = (float )CFontManager::instance().fontHeight();
			string str;
			uint32 len;
			
			str = "Title: " + CLevelManager::instance().currentLevel().name();
			len = str.size();
			CFontManager::instance().printf(CRGBA(255, 255, 255, 255), 1 * fontWidth, 10.0f * fontHeight,1, str.c_str());
			/*
			str = "File : " + CLevelManager::instance().currentLevel().filename();
			len = str.size();
			CFontManager::instance().printf(CRGBA(255, 255, 255, 255), 1 * fontWidth, 11.0f * fontHeight,1, str.c_str());
			*/
			str = "Author: " + CLevelManager::instance().currentLevel().author();
			len = str.size();
			CFontManager::instance().printf(CRGBA(255, 255, 255, 255), 1 * fontWidth, 12.0f * fontHeight,1, str.c_str());
			/*
			CFontManager::instance().printf(CRGBA(245, 238, 141, 255), 1 * fontWidth, 14 * fontHeight,1, "Best score:");
			CFontManager::instance().printf(CRGBA(253, 207, 85, 255), 3 * fontWidth, 15 * fontHeight,1, string1.c_str());
			
			CFontManager::instance().printf(CRGBA(245, 238, 141, 255), 1 * fontWidth, 16 * fontHeight,1, "Best time:");
			CFontManager::instance().printf(CRGBA(253, 207, 85, 255), 3 * fontWidth, 17 * fontHeight,1, string2.c_str());
			*/
		}

	// ace todo put HUD in a task
	
	float baseY = float(C3DTask::instance().screenHeight()) - 300.0f;
	float height = 244.0f;
	height = (1-(AltimeterValue - AltimeterMinValue) / (AltimeterMaxValue - AltimeterMinValue)) * height;
	float x1 = float(C3DTask::instance().screenWidth() - 40), y1 = baseY, tx1 = 15, ty1 = 8, tw1 = 34-15, th1 = 253-8;
	float x2 = float(C3DTask::instance().screenWidth() - 60), y2 = baseY - 18 + height, tx2 = 46, ty2 = 10, tw2 = 78-46, th2 = 43-10;	

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
	
	uint8 eid = CMtpTarget::instance().controler().Camera.getFollowedEntity();
	if (eid != 255)
	{
		// display our score (bottom right)
		string totalScoreStr = toString("score %u",CEntityManager::instance()[eid].totalScore());
		CFontManager::instance().printf(CRGBA(245, 238, 141, 255), (float) (C3DTask::instance().screenWidth() - totalScoreStr.size() * CFontManager::instance().fontWidth() - 10), float(C3DTask::instance().screenHeight() - 1 * CFontManager::instance().fontHeight()), 1, totalScoreStr.c_str());
		
		if (CEntityManager::instance()[eid].interpolator().outOfKey())
			CFontManager::instance().printf(CRGBA(255, 0, 0, 255), float(C3DTask::instance().screenWidth() / 2 - 70), 70.0f,1, "NET LAG");
	}
	

	double TimeBeforeTimeout = CMtpTarget::instance().timeBeforeTimeout();
	if (TimeBeforeTimeout < 0)
		TimeBeforeTimeout = 0;
	string timeBeforeTimeoutStr = toString("Time left %u",(uint)TimeBeforeTimeout);
	CFontManager::instance().printf(CRGBA(245, 238, 141, 255), 10.0f, (float)(C3DTask::instance().screenHeight() - 20),1,timeBeforeTimeoutStr.c_str() );
	
	CFontManager::instance().printf(CRGBA(255,255,255,255),(C3DTask::instance().screenWidth() - _viewedName.size() * CFontManager::instance().fontWidth()) / 2.0f,float(C3DTask::instance().screenHeight() - 2 * CFontManager::instance().fontHeight()),1,_viewedName.c_str());


	/*
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
				CFontManager::instance().printf(CEntityManager::instance()[i].color(), x2, y, 1, "%u", CEntityManager::instance()[i].currentScore());
			CFontManager::instance().printf(CEntityManager::instance()[i].color(), x3, y, 1, "%u", CEntityManager::instance()[i].totalScore());
			CFontManager::instance().printf(CEntityManager::instance()[i].color(), x4, y, 1, "%u", CEntityManager::instance()[i].ping());
		}
	}	
	*/
}

void CHudTask::release()
{
}

void CHudTask::setDisplayViewedName(const string &name)
{
	_viewedName = name;
}

