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

#define SHARD_LIST_FILE "shard_list.xml"

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

	errorServerReason->text = reason;
}


void CIntroTask::updateInit()
{
	if(State!=eInit) return;

	guiSPG<CGuiXml> xml = 0;
	xml = CGuiXmlManager::instance().Load("menu.xml");
	menuFrame = (CGuiFrame *)xml->get("menuFrame");
	howToPlay = (CGuiButton *)xml->get("bHowToPlay");
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

	xml = CGuiXmlManager::instance().Load("login_lan.xml");
	loginLanFrame = (CGuiFrame *)xml->get("loginFrame");
	loginLanText = (CGuiText*)xml->get("loginEntry");
	loginLanText->text = CConfigFileTask::instance().configFile().getVar("Login").asString();
	passwordLanText = (CGuiText*)xml->get("passwordEntry");
	passwordLanText->text = CConfigFileTask::instance().configFile().getVar("Password").asString();
	serverLanText = (CGuiText*)xml->get("serverEntry");
	serverLanText->text = CConfigFileTask::instance().configFile().getVar("ServerHost").asString();
	loginLanButton = (CGuiButton *)xml->get("bLogin");
	backLanButton1 = (CGuiButton *)xml->get("bBack");
	
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

	if(howToPlay->pressed())
	{
		openURL("http://www.mtp-target.org/index.php?page=./lang/en/howto-play.php");
	}
	if(playOnLineButton->pressed())
	{
		CGuiObjectManager::instance().objects.clear();
		CGuiObjectManager::instance().objects.push_back(loginFrame);
		State = eLoginOnline;
	}
	if(playOnLanButton->pressed())
	{
		CGuiObjectManager::instance().objects.clear();
		CGuiObjectManager::instance().objects.push_back(loginLanFrame);
		State = eLoginOnlan;
	}
	
}


void CIntroTask::getShardListFromFile(const string &filename)
{
	xmlNodePtr snode;
	CIFile file;
	NLMISC::CIXml doc;
	string fn = CPath::lookup(filename, false);
	if (file.open(fn.c_str()))
	{
		if (doc.init(file))
		{
			snode = doc.getRootNode();
		}		
		else
		{
			nlwarning("cannot init xml file : %s",filename.c_str());
			nlassert(false);
			return;
		}
	}
	else
	{
		nlwarning("cannot open file : %s",filename.c_str());
		nlassert(false);
		return;
	}
	CLoginClientMtp::ShardList.clear ();
	for( snode = doc.getFirstChildNode(snode,"server");snode;snode = doc.getNextChildNode(snode,"server") )
	{
		xmlNodePtr node;
		bool isok;
		string name;
		isok = doc.getPropertyString(name,snode,"name");
		string address;
		isok = doc.getPropertyString(address,snode,"address");
		uint16 port = CConfigFileTask::instance().configFile().getVar("TcpPort").asInt();

		node = doc.getFirstChildNode(snode,"port");
		if(node)
		{
			string s;
			doc.getContentString(s,node);
			char tmpstr[1024];
			strcpy(tmpstr,s.c_str());
			char *cstr = tmpstr;
			char *endstr = 0;
			port = (uint16)strtod(cstr,&endstr);
		}			
		nlinfo("adding server : %s(%s) : %d",name.c_str(),address.c_str(),port);
		CLoginClientMtp::CShardEntry se;
		se.ShardName = name;
		se.WSAddr = address;
		se.port = port;
		CLoginClientMtp::ShardList.push_back (se);
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
		loginText->text = NLMISC::strlwr(loginText->text);
		passwordText->text = NLMISC::strlwr(passwordText->text);
		CConfigFileTask::instance().configFile().getVar("Login").setAsString(loginText->text);
		CConfigFileTask::instance().configFile().getVar("Password").setAsString(passwordText->text);
		CConfigFileTask::instance().configFile().save();
		_errorServerFrame = 0;
		/*
		string loginServer = CConfigFileTask::instance().configFile().getVar("LSHost").asString();
		string reason = CLoginClientMtp::authenticate(loginServer, loginText->text, passwordText->text, 0);
		if(!reason.empty())
		{
			_autoLogin = 0;
			error(reason);
			CGuiObjectManager::instance().objects.push_back(loginFrame);
		}
		else
		*/
		{
			getShardListFromFile(SHARD_LIST_FILE);
			ServerId = 0;
			
			serverVbox->elements.clear();
			for(uint i = 0; i < CLoginClientMtp::ShardList.size(); i++)
			{
				guiSPG<CGuiXml> xml = 0;
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
	
	if(backLanButton1->pressed())
	{
		State = eMenu;
		CGuiObjectManager::instance().objects.clear();
		CGuiObjectManager::instance().objects.push_back(menuFrame);	
		return;
	}
	
	if(loginLanButton->pressed() || _autoLogin)
	{
		loginLanText->text = NLMISC::strlwr(loginLanText->text);
		passwordLanText->text = NLMISC::strlwr(passwordLanText->text);
		serverLanText->text = NLMISC::strlwr(serverLanText->text);
		CConfigFileTask::instance().configFile().getVar("Login").setAsString(loginText->text);
		CConfigFileTask::instance().configFile().getVar("Password").setAsString(passwordText->text);
		CConfigFileTask::instance().configFile().getVar("ServerHost").setAsString(serverLanText->text);
		CConfigFileTask::instance().configFile().save();
		
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

/* old login system
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
	
	// try to connect
	string res = CNetworkTask::instance().connect(&ip);
	if(res.empty())
	{
		_autoLogin = 0;//autologin only once
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
*/

void CIntroTask::updateConnectionOnLine()
{
	if(State!=eConnectionOnline) return;
	
	CGuiObjectManager::instance().objects.clear();
	
	CInetAddress addr;
	
//	addr.setNameAndPort(CLoginClientMtp::ShardList[ServerId].WSAddr+":"+toString(CConfigFileTask::instance().configFile().getVar("TcpPort").asInt()));
	addr.setNameAndPort(CLoginClientMtp::ShardList[ServerId].WSAddr+":"+toString(CLoginClientMtp::ShardList[ServerId].port));
	
	string res = CNetworkTask::instance().connect(&addr);
	if(res.empty())
	{
		_autoLogin = 0;//autologin only once
		stop();
		// stop the background
		CBackgroundTask::instance().stop();
		// go to the game task
		CResourceManager::instance().get(string(SHARD_LIST_FILE));		
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
}


void CIntroTask::updateConnectionOnLan()
{
	if(State!=eConnectionOnlan) return;
	
	CGuiObjectManager::instance().objects.clear();
	
	string res = CNetworkTask::instance().connect();
	if(res.empty())
	{
		_autoLogin = 0;//autologin only once
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
		CGuiObjectManager::instance().objects.push_back(loginLanFrame);
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
	CFontManager::instance().littlePrintf(0.0f, 0.0f, toString("v%s %s",MTPT_RELEASE_VERSION_NUMBER,MTPT_RELEASE_VERSION_NAME).c_str());
}

void CIntroTask::release()
{
}
