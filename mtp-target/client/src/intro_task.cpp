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

#include <nel/net/callback_client.h>

#include "3d_task.h"
#include "gui_task.h"
#include "time_task.h"
#include "game_task.h"
#include "intro_task.h"
#include "mtp_target.h"
#include "login_client.h"
#include "font_manager.h"
#include "network_task.h"
#include "task_manager.h"
#include "background_task.h"
#include "resource_manager.h"
#include "config_file_task.h"


//
// Namespaces
//

using namespace std;
using namespace NL3D;
using namespace NLMISC;
using namespace NLNET;


//
// Variables
//


//
// Classes
//
class CGuiServerButtonEventBehaviour : public CGuiButtonEventBehaviour
{
public:
	CGuiServerButtonEventBehaviour(uint32 serverId)
	{
		_serverId = serverId;
	}
	virtual ~CGuiServerButtonEventBehaviour()
	{
		
	}
	
	virtual void onPressed()
	{
		CIntroTask::instance().doConnectionOnLine(_serverId);	
	}
private:
	uint32 _serverId;
};

class CGuiExitButtonEventBehaviour : public CGuiButtonEventBehaviour
{
public:
	CGuiExitButtonEventBehaviour() {}
	virtual ~CGuiExitButtonEventBehaviour() {}
	virtual void onPressed()
	{
		CTaskManager::instance().exit();
	}
private:
};

//
// Functions
//


void CIntroTask::reset()
{
	CGuiObjectManager::instance().objects.clear();
	State=eInit;
}

void CIntroTask::init()
{
	CTaskManager::instance().add(CBackgroundTask::instance(), 59);
	_autoLogin = CConfigFileTask::instance().configFile().getVar("AutoLogin").asInt();
	
	reset();
}

void CIntroTask::error(string &reason)
{
	guiSPG<CGuiXml> xml = CGuiXmlManager::instance().Load("error_server.xml");
	_errorServerFrame = (CGuiFrame *)xml->get("errorServerFrame");
	guiSPG<CGuiText>  errorServerReason = (CGuiText *)xml->get("errorServerReason");
	CGuiObjectManager::instance().objects.push_back(_errorServerFrame);

	string::size_type pos = reason.find("failed");
	if(pos == string::npos)
	{
		errorServerReason->text = reason;
	}
	else
	{
		errorServerReason->text = reason.substr(0,pos+6);
		errorServerReason->text += '\n';
		errorServerReason->text += reason.substr(pos+6);
		
	}
}


void CIntroTask::updateInit()
{
	if(State!=eInit) return;

	guiSPG<CGuiXml> xml = NULL;
	xml = CGuiXmlManager::instance().Load("menu.xml");
	menuFrame = (CGuiFrame *)xml->get("menuFrame");
	playOnLineButton = (CGuiButton *)xml->get("bPlayOnline");
	playOnLanButton = (CGuiButton *)xml->get("bPlayOnlan");
	exitButton3 = (CGuiButton *)xml->get("bExit");
	exitButton3->eventBehaviour = new CGuiExitButtonEventBehaviour();
	
	xml = CGuiXmlManager::instance().Load("login.xml");
	loginFrame = (CGuiFrame *)xml->get("loginFrame");
	loginText = (CGuiText*)xml->get("loginEntry");
	loginText->text = CConfigFileTask::instance().configFile().getVar("Login").asString();
	passwordText = (CGuiText*)xml->get("passwordEntry");
	passwordText->text = CConfigFileTask::instance().configFile().getVar("Password").asString();
	loginButton = (CGuiButton *)xml->get("bLogin");
	backButton1 = (CGuiButton *)xml->get("bBack");
	
	xml = CGuiXmlManager::instance().Load("server_list.xml");
	serverListFrame = (CGuiFrame *)xml->get("serverListFrame");
	serverVbox = (CGuiVBox *)xml->get("serverVbox");
	backButton2 = (CGuiButton *)xml->get("bBack");
	
	CGuiObjectManager::instance().objects.push_back(menuFrame);
	State = eMenu;
	
	if(_autoLogin==1)
		State = eLoginOnline;
	else if(_autoLogin==2)
		State = eLoginOnlan;
	
}

void CIntroTask::updateMenu()
{
	if(State!=eMenu) return;

	if(playOnLineButton->pressed())
	{
		CGuiObjectManager::instance().objects.clear();
		CGuiObjectManager::instance().objects.push_back(loginFrame);
		State = eLoginOnline;
	}
	if(playOnLanButton->pressed())
	{
		CGuiObjectManager::instance().objects.clear();
		CGuiObjectManager::instance().objects.push_back(loginFrame);
		State = eLoginOnlan;
	}
	
}

void CIntroTask::updateLoginOnline()
{
	if(State!=eLoginOnline) return;

	if(backButton1->pressed())
	{
		State = eMenu;
		CGuiObjectManager::instance().objects.clear();
		CGuiObjectManager::instance().objects.push_back(menuFrame);	
		return;
	}

	if(loginButton->pressed() || _autoLogin==1)
	{
		_errorServerFrame = NULL;
		
		string loginServer = CConfigFileTask::instance().configFile().getVar("LSHost").asString();
		
		string reason = CLoginClientMtp::authenticate(loginServer, loginText->text, passwordText->text, 0);
		
		if(!reason.empty())
		{
			_autoLogin = 0;
			error(reason);
			CGuiObjectManager::instance().objects.push_back(loginFrame);
		}
		else
		{
			ServerId = 0;
			
			serverVbox->elements.clear();
			for(uint i = 0; i < CLoginClientMtp::ShardList.size(); i++)
			{
				guiSPG<CGuiXml> xml = NULL;
				xml = CGuiXmlManager::instance().Load("server_item.xml");
				{
					guiSPG<CGuiHBox> serverItemHBox = (CGuiHBox *)xml->get("serverItemHBox");
					guiSPG<CGuiText> btextServer = (CGuiText *)xml->get("btextServer");
					guiSPG<CGuiButton> serverButton = (CGuiButton *)xml->get("bServer");
					serverButton->eventBehaviour = new CGuiServerButtonEventBehaviour(i);
					btextServer->text = CLoginClientMtp::ShardList[i].ShardName;
					serverVbox->elements.push_back(serverItemHBox);
				}			
			}
			CGuiObjectManager::instance().objects.clear();
			CGuiObjectManager::instance().objects.push_back(serverListFrame);
			State = eServerList;
		}

	}
		
}

void CIntroTask::updateLoginOnlan()
{
	if(State!=eLoginOnlan) return;
	
	if(backButton1->pressed())
	{
		State = eMenu;
		CGuiObjectManager::instance().objects.clear();
		CGuiObjectManager::instance().objects.push_back(menuFrame);	
		return;
	}
	
	if(loginButton->pressed() || _autoLogin)
	{
		CGuiObjectManager::instance().objects.clear();
		State = eConnectionOnlan;
	}
	
}

void CIntroTask::updateServerList()
{
	if(State!=eServerList) return;

	if(_autoLogin==1)
		doConnectionOnLine(0);

	if(backButton2->pressed())
	{
		State = eLoginOnlan;
		CGuiObjectManager::instance().objects.clear();
		CGuiObjectManager::instance().objects.push_back(menuFrame);	
		return;
	}
	
	
}

void CIntroTask::updateConnectionOnLine()
{
	if(State!=eConnectionOnline) return;
	
	CGuiObjectManager::instance().objects.clear();
	
	CInetAddress ip;
	CLoginCookie cookie;
	string reason = CLoginClientMtp::connectToShard(ServerId,ip,cookie); // i = index dans le vector de server a parti de 0
	
	if(!reason.empty())
	{
		_autoLogin = 0;
		error(reason);
		CGuiObjectManager::instance().objects.push_back(loginFrame);
		State = eLoginOnline;
		return;
	}
	
	CConfigFileTask::instance().configFile().getVar("Login").setAsString(loginText->text);
	CConfigFileTask::instance().configFile().getVar("Password").setAsString(passwordText->text);
	CConfigFileTask::instance().configFile().save();
	
	// try to connect
	string res = CNetworkTask::instance().connect(&ip);
	if(res.empty())
	{
		_autoLogin = 0;//autologin only once
		// no connection problem
		// remove this intro task
		//remove();
		stop();
		// stop the background
		CBackgroundTask::instance().stop();
		// go to the game task
		CTaskManager::instance().add(CGameTask::instance(), 60);
	
		CGuiObjectManager::instance().objects.clear();
		State = eNone;
		return;		
	}
	else
	{
		_autoLogin = 0;
		error(res);
		CGuiObjectManager::instance().objects.push_back(loginFrame);
		State = eLoginOnline;
		return;
	}

	nlassert(false);
}

void CIntroTask::updateConnectionOnLan()
{
	if(State!=eConnectionOnlan) return;
	
	CGuiObjectManager::instance().objects.clear();
	
	string res = CNetworkTask::instance().connect();
	if(res.empty())
	{
		_autoLogin = 0;//autologin only once
		// no connection problem
		// remove this intro task
//		remove();
		stop();
		// stop the background
		CBackgroundTask::instance().stop();
		// go to the game task
		CTaskManager::instance().add(CGameTask::instance(), 60);
		
		CGuiObjectManager::instance().objects.clear();
		State = eNone;
		return;		
	}
	else
	{
		_autoLogin = 0;
		error(res);
		CGuiObjectManager::instance().objects.push_back(loginFrame);
		State = eLoginOnlan;
		return;
	}
	
	
}	

void CIntroTask::doConnectionOnLine(uint32 serverId)
{
	ServerId = serverId;
	State = eConnectionOnline;
}

void CIntroTask::doConnectionOnLan()
{
	State = eConnectionOnlan;
}

void CIntroTask::update()
{
	updateMenu();
	updateInit();
	updateLoginOnline();
	updateLoginOnlan();
	updateServerList();
	updateConnectionOnLine();
	updateConnectionOnLan();
	
	if(C3DTask::instance().kbPressed(KeyESCAPE))
	{
		// want to quit
		CTaskManager::instance().exit();
	}
	
}

void CIntroTask::render()
{
	float x = 0;
	float y = 0;
	string versionStr = toString("v%s %s",MTPT_RELEASE_VERSION_NUMBER,MTPT_RELEASE_VERSION_NAME);
	
	C3DTask::instance().textContext().setHotSpot (UTextContext::BottomLeft);
	C3DTask::instance().textContext().setColor (CRGBA(0,0,0,255));
	C3DTask::instance().textContext().setFontSize (12);
	C3DTask::instance().textContext().setShaded(false);
	C3DTask::instance().textContext().setKeep800x600Ratio (true);
	C3DTask::instance().textContext().printAt (CGuiObject::ToProportionalX(x), CGuiObject::ToProportionalY(y), ucstring(versionStr.c_str()));	
}

void CIntroTask::release()
{
}
