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

#include <nel/misc/path.h>
#include <nel/misc/debug.h>
#include <nel/misc/report.h>
#include <nel/misc/config_file.h>

#include <nel/3d/u_scene.h>
#include <nel/3d/u_scene.h>
#include <nel/3d/u_driver.h>
#include <nel/3d/u_camera.h>
#include <nel/3d/u_driver.h>
#include <nel/3d/u_texture.h>
#include <nel/3d/u_instance.h>
#include <nel/3d/u_text_context.h>

#include <nel/net/email.h>

#include "main.h"
#include "global.h"
#include "entity.h"
#include "3d_task.h"
#include "gui_task.h"
#include "hud_task.h"
#include "time_task.h"
#include "game_task.h"
#include "mtp_target.h"
#include "intro_task.h"
#include "editor_task.h"
#include "network_task.h"
#include "task_manager.h"
#include "font_manager.h"
#include "swap_3d_task.h"
#include "sound_manager.h"
#include "level_manager.h"
#include "entity_manager.h"
#include "entity_manager.h"
#include "background_task.h"
#include "resource_manager2.h"
#include "config_file_task.h"

//
// Namespaces
//

using namespace std;
using namespace NL3D;
using namespace NLNET;
using namespace NLMISC;


//
// Variables
//

//NLMISC::CConfigFile CConfigFileTask::getInstance().configFile();

extern FILE *SessionFile;

const char *mtpTargetConfigFilename = "mtp_target.cfg";

//
// Functions
//

// Order
//
// 0->100		low level task
// 100->1000	3d render
// 1000->10000	hud render
// 10000		3d swap
//

void CMtpTarget::init()
{
	SessionFileName = "";
	Error = false;
	DoError = false;
	// setup default task
	nlinfo("setting up default tasks");
	CTaskManager::getInstance().add(CConfigFileTask::getInstance(), 10);
	CTaskManager::getInstance().add(CTimeTask::getInstance(), 20);
	CTaskManager::getInstance().add(CEntityManager::getInstance(), 24);
	CTaskManager::getInstance().add(CNetworkTask::getInstance(), 25);
	CTaskManager::getInstance().add(CResourceManager::getInstance(), 30);
	CTaskManager::getInstance().add(CResourceManagerLan::getInstance(), 30);
	CTaskManager::getInstance().add(C3DTask::getInstance(), 110);
	CTaskManager::getInstance().add(CGuiTask::getInstance(), 1050);
	CTaskManager::getInstance().add(CEditorTask::getInstance(), 120);
	CTaskManager::getInstance().add(CSwap3DTask::getInstance(), 10000);
	CTaskManager::getInstance().add(CFontManager::getInstance(), 40);

	CConfigFile::CVar &var = CConfigFileTask::getInstance().configFile().getVar("NegFiltersDebug");
	for(sint i = 0; i < var.size(); i++)
		DebugLog->addNegativeFilter(var.asString(i).c_str());

	CConfigFile::CVar &var2 = CConfigFileTask::getInstance().configFile().getVar("NegFiltersInfo");
	for(sint i = 0; i < var2.size(); i++)
		InfoLog->addNegativeFilter(var2.asString(i).c_str());

	if(CConfigFileTask::getInstance().configFile().getVar("Sound").asInt() == 1)
		CTaskManager::getInstance().add(CSoundManager::getInstance(), 50);
	nlinfo("default tasks set");
	
	Controler = new CControler;

	if (!ReplayFile.empty() && 1)
	{
		nlinfo("loading replay file : %s",ReplayFile.c_str());		
		string levelName = "";
		FILE *fp = fopen (ReplayFile.c_str(), "rt");
		if (fp != 0)
		{
			uint nbplayer, eid, self, currentScore, totalScore;
			char name[100];
			char texture[100];
			
			fscanf (fp, "%d", &nbplayer);
			fscanf (fp, "%s", &name);
			levelName = name;
			
			// add players
			for(uint i = 0; i < nbplayer; i++)
			{
				strcpy(texture,"");
				//TODO manage trace and mesh in replay
				fscanf (fp, "%d %s %d %d %d %s", &eid, &name, &self, &currentScore, &totalScore, &texture);
				
				CRGBA col(255,255,255);
				string textureName = texture;
				string traceName = "trace";
				string meshName = "pingoo";
				textureName = textureName.substr(1,textureName.size()-2);
				CEntityManager::getInstance().add(eid, name, totalScore, col, textureName, false,self!=0, traceName, meshName);
				
				//CEntity *entity = new CEntity(CRGBA(255,255,255));
				//entity->setId(eid);
				//mtpTarget::getInstance().World.add(entity);
				//entity->name = name;
				//CEntityManager::getInstance()[eid].setScore(0, score);
				
				/*
				if (self)
				{
					CEntityManager::getInstance()[eid].setIsLocal(true);
				}
				*/
			}
			
			char cmd[10];
			float t =0;
			float x, y, z;
			while (!feof (fp))
			{
				fscanf (fp, "%d %s", &eid, &cmd);
				if (string(cmd) == "PO")
				{
					fscanf (fp, "%f %f %f", &x, &y, &z);
					
					if (eid != 255 && CEntityManager::getInstance().exist(eid))
					{
						CVector v(x, y, z);
						//CEntityManager::getInstance()[eid].interpolator().addKey(CEntityInterpolatorKey(CEntityState(v,false),t));//.position(v,t, false); //put the entity in the good position
						bool oc = false;
						if(CEntityManager::getInstance()[eid].addOpenCloseKey)
						{
							CEntityManager::getInstance()[eid].addOpenCloseKey = false;
							oc = true;
						}
						CCrashEvent ce(false,CVector::Null);
						if(CEntityManager::getInstance()[eid].addCrashEventKey.Crash)
						{
							ce = CEntityManager::getInstance()[eid].addCrashEventKey;
							CEntityManager::getInstance()[eid].addCrashEventKey.Crash = false;
						}
						CEntityManager::getInstance()[eid].interpolator().addKey(CEntityInterpolatorKey(CEntityState(v,false,oc,ce,CEntityManager::getInstance()[eid].addChatLine),t));//.position(v,t, false); //put the entity in the good position
						CEntityManager::getInstance()[eid].addChatLine = "";
					}
					else
						nlwarning ("%d not found", eid);
				}
				else if (string(cmd) == "OC")
				{
					if(CEntityManager::getInstance().exist(eid))
						CEntityManager::getInstance()[eid].addOpenCloseKey = true;
				}
				else if (string(cmd) == "CE")
				{
					fscanf (fp, "%f %f %f", &x, &y, &z);
					CVector v(x, y, z);
					if(CEntityManager::getInstance().exist(eid))
						CEntityManager::getInstance()[eid].addCrashEventKey = CCrashEvent(true,v);
				}
				else if (string(cmd) == "CH")
				{
					char chatLine[1024];
					fgets(chatLine,1024,fp);
					uint l = strlen(chatLine);
					if(l>0 && chatLine[l-1]=='\n')
						chatLine[l-1] = '\0';
					//fscanf (fp, "%s\n",&chatLine);
					uint8 eid = CEntityManager::getInstance().findFirstEId();
					if(eid!=255)
					{
						if(CEntityManager::getInstance().exist(eid))
							CEntityManager::getInstance()[eid].addChatLine = chatLine;
					}
					nlinfo("replay chat line : %s",chatLine);
				}
				else
				{
					nlwarning ("Unknown command %s for user %d", cmd, eid);
				}
			}
			
			fclose (fp);
		}
		else
			nlwarning("connot open replay file");
		
		//CMtpTarget::getInstance().State = CMtpTarget::eReady;
		CTaskManager::getInstance().add(CGameTask::getInstance(), 60);
		CMtpTarget::getInstance().startSession(1, 60, levelName, "", "");
	}
	else
	{
		// start with intro task
		CTaskManager::getInstance().add(CIntroTask::getInstance(), 60);
		reset();
	}
}

void CMtpTarget::error(string reason)
{
	DoError = true;
	ErrorReason = reason;
	Error = true;
	CNetworkTask::getInstance().stop();
}

void CMtpTarget::reset()
{
	nlinfo("CMtpTarget::reset()");
	CEntityManager::getInstance().sessionReset();
	controler().reset();
	NewSession = false;
	TimeBeforeSessionStart = 0.0f;
	TimeBeforeTimeout = 0.0f;
	State = CMtpTarget::eBeforeFirstSession;
	DisplayTutorialInfo = false;
}

bool CMtpTarget::error()
{
	return Error;
}


void CMtpTarget::_error()
{
	nlinfo("error occured : stop all and reset");
	reset();
	CGameTask::getInstance().stop();
	CEditorTask::getInstance().stop();
	CEditorTask::getInstance().enable(false);
	CBackgroundTask::getInstance().restart();
	//CIntroTask::getInstance().reset();
	CIntroTask::getInstance().error(ErrorReason);
	CIntroTask::getInstance().restart();
	CLevelManager::getInstance().release();
	DoError = false;
}

void CMtpTarget::update()
{
	if(DoError)
		_error();

	if(NewSession)
		loadNewSession();

	controler().update();

	if(FollowEntity)
	{
//		nlinfo("set camera matrix follow");
		C3DTask::getInstance().scene().getCam().setMatrix(*CMtpTarget::getInstance().controler().Camera.getMatrixFollow());
	}

	if(CMtpTarget::getInstance().State == CMtpTarget::eReady)
	{
		TimeBeforeSessionStart -= (float)CTimeTask::getInstance().deltaTime();

		if(TimeBeforeSessionStart > 4.0f)
		{
			CMtpTarget::getInstance().controler().Camera.reset();
			CMtpTarget::getInstance().controler().Camera.ForcedSpeed = 0.0f;
		}
		else
			CMtpTarget::getInstance().controler().Camera.ForcedSpeed = 1.0f;
		
		if(TimeBeforeSessionStart <= 0)
		{
			TimeBeforeSessionStart = 0;
			CMtpTarget::getInstance().State = CMtpTarget::eGame;
			CEntityManager::getInstance().sessionReset();			
		}
	}
	if(CMtpTarget::getInstance().State == CMtpTarget::eGame)
	{
		//CMtpTarget::getInstance().controler().Camera.reset();
		TimeBeforeSessionStart += (float)CTimeTask::getInstance().deltaTime();
		TimeBeforeTimeout -= (float)CTimeTask::getInstance().deltaTime();
	}
	if(CMtpTarget::getInstance().State == CMtpTarget::eBeforeFirstSession)
	{
		//CMtpTarget::getInstance().controler().Camera.reset();
		TimeBeforeSessionStart += (float)CTimeTask::getInstance().deltaTime();
		TimeBeforeTimeout -= (float)CTimeTask::getInstance().deltaTime();
	}
	if(CMtpTarget::getInstance().State == CMtpTarget::eStartSession && IsSpectatorOnly)
	{
		//CMtpTarget::getInstance().controler().Camera.reset();
		TimeBeforeSessionStart += (float)CTimeTask::getInstance().deltaTime();
		TimeBeforeTimeout -= (float)CTimeTask::getInstance().deltaTime();
	}

}

void CMtpTarget::displayTutorialInfo(bool b)
{
	DisplayTutorialInfo = b;
}

bool CMtpTarget::displayTutorialInfo()
{
	return DisplayTutorialInfo;
}

void CMtpTarget::loadNewSession()
{
	bool sendReady = !isSpectatorOnly();
	bool paused = pauseAllThread();
	if(!paused)
	{
		nlwarning("CMtpTarget::loadNewSession() : pauseAllThread() failed");
		return;
	}
	
	nlassert(NewSession);
	CEntityManager::getInstance().everybodyReady(false);
	
	NewSession = false;
	nlinfo("loadNewSession() '%s'",NewLevelName.c_str());
	
	if(NewLevelName.empty())
	{
		nlwarning("CMtpTarget::loadNewSession() : NewLevelName is empty");
		resumeAllThread();
		return;
	}

	resumeAllThread();
	if(TimeBeforeSessionStart != 0.0f)
	{
		// i'm in the new session and all other entities, reset spectator
		CEntityManager::getInstance().resetSpectator();
		if(!CEditorTask::getInstance().enable())
			FollowEntity = true;
	}
	else
	{
		FollowEntity = false;		
	}
	
	CEntityManager::getInstance().load3d();
	
	CLevelManager::getInstance().loadLevel(NewLevelName);
	
	CEntityManager::getInstance().startSession();
	
	CMtpTarget::getInstance().controler().Camera.reset();
	/*
	if (CMtpTarget::getInstance().controler().getControledEntity() != 255)
		CEntityManager::getInstance()[CMtpTarget::getInstance().controler().getControledEntity()].interpolator().reset();
	*/
	resetFollowedEntity();
	controler().Camera.reset();

	if(CLevelManager::getInstance().levelPresent())
		CLevelManager::getInstance().currentLevel().reset();
	else
		nlwarning("there is no level");

	State = eStartSession;
	
	nlinfo ("loaded level");
	
	// say to server that we are ready if other players wait for us
	if(sendReady)
		CNetworkTask::getInstance().ready();
	
	// now we wait the message that say that everybody is ready
	
	// create the session replay
	
	if (SessionFile)
	{
		fclose (SessionFile);
	}
	
	if(!NLMISC::CFile::isDirectory("replay"))
		NLMISC::CFile::createDirectory("replay");
	
	if(ReplayFile.empty() && CConfigFileTask::getInstance().configFile().getVar("GenerateReplay").asInt() == 1)
	{
		int currentReplaySavedCount = CConfigFileTask::getInstance().configFile().getVar("currentReplaySavedCount").asInt();
		int maxReplaySavedCount = CConfigFileTask::getInstance().configFile().getVar("maxReplaySavedCount").asInt();
		if(maxReplaySavedCount<1)
			maxReplaySavedCount=1;
		if(maxReplaySavedCount>999)
			maxReplaySavedCount=999;

		currentReplaySavedCount++;
		currentReplaySavedCount = currentReplaySavedCount % maxReplaySavedCount;
		CConfigFileTask::getInstance().configFile().getVar("currentReplaySavedCount").setAsInt(currentReplaySavedCount);
		CConfigFileTask::getInstance().configFile().save();
		
		SessionFileName = toString("replay/session%03d.mtr",currentReplaySavedCount);
		//nlinfo(">>%s",SessionFileName.c_str());
		//SessionFileName = NLMISC::CFile::findNewFile("replay/session.mtr");
		SessionFile = fopen (SessionFileName.c_str(), "wt");
		nlassert (SessionFile != 0);
	}
	
	if (SessionFile)
	{
		fprintf(SessionFile, "%hu\n", (uint16)CEntityManager::getInstance().size());
		fprintf(SessionFile, "%s\n",NewLevelName.c_str() );
	}
	
	int self;
	
	uint8 eid = CEntityManager::getInstance().findFirstEId();
	if (eid != 255 && SessionFile)
	{
		do
		{
			self = (eid == CMtpTarget::getInstance().controler().getControledEntity())?1:0;
			fprintf(SessionFile, "%hu %s %d %d %d '%s'\n", (uint16)eid, CEntityManager::getInstance()[eid].name().c_str(), self, CEntityManager::getInstance()[eid].currentScore(), CEntityManager::getInstance()[eid].totalScore(),CEntityManager::getInstance()[eid].texture().c_str());
			eid = CEntityManager::getInstance().findNextEId(eid);
		}
		while(eid != CEntityManager::getInstance().findFirstEId());
	}

	if(!ReplayFile.empty())
		mtpTarget::getInstance().everybodyReady();
		
}

void CMtpTarget::startSession(float timeBeforeSessionStart, float timeBeforeTimeout, const string &levelName, const std::string &str1, const std::string &str2,bool isSpectatorOnly)
{
	nlinfo("level '%s' loaded, it timeouts in %g seconds", levelName.c_str(), timeBeforeTimeout);
	
	C3DTask::getInstance().mouseListener().reset();
	IsSpectatorOnly = isSpectatorOnly;
	NewSession = true;
	TimeBeforeSessionStart = timeBeforeSessionStart;
	TimeBeforeTimeout = timeBeforeTimeout;
	NewLevelName = levelName;
	String1 = str1;
	String2 = str2;
}

void CMtpTarget::resetFollowedEntity()
{
	CMtpTarget::getInstance().controler().Camera.setFollowedEntity(CMtpTarget::getInstance().controler().getControledEntity());
	CMtpTarget::getInstance().controler().ViewedEId = CMtpTarget::getInstance().controler().getControledEntity();
	
	//	for(uint32 entityPos = 0; entityPos < mtpTarget::getInstance().World.count(); entityPos++)
	//	{
	//		if(mtpTarget::getInstance().World.get(entityPos) == CMtpTarget::getInstance().controler().getControledEntity())
	//		{
	//			CMtpTarget::getInstance().controler().PlayerViewedPos = entityPos;
	//			break;
	//		}
	//	}
}





void CMtpTarget::moveReplay(bool b)
{
	MoveReplay = b;
}

bool CMtpTarget::moveReplay(void)
{
	return MoveReplay;
}

//sint32 SleepTime;

/*void mtpTarget::updateTime ()
{
	// use double for precision, and use FirstTime to translate time to
	// avoid precision lost with very big time

	OldTime = Time;

	double newTime = NLMISC::CTime::ticksToSecond(NLMISC::CTime::getPerformanceTime());

	if (FirstTime == 0)
		FirstTime = newTime;

	Time = (float)(newTime - FirstTime);

	if(OldTime == 0) DeltaTime = 0.0f;
	else DeltaTime = Time - OldTime;

	DeltaTimeSmooth.addValue(DeltaTime);
}*/

void mtpTarget::init()
{
	nlinfo("Init Mtp Target...");

	try
	{
		if(!NLMISC::CFile::fileExists(mtpTargetConfigFilename))
		{
			FILE *fp = fopen(mtpTargetConfigFilename,"wt");
			nlassert(fp!=0);
			fprintf(fp,"RootConfigFilename   = \"mtp_target_default.cfg\";\n");
			fclose(fp);
		}
		CConfigFileTask::getInstance().configFile().load (mtpTargetConfigFilename);
	}
	catch (NLMISC::EConfigFile &e)
	{
		nlerror(e.what());
	}
	

	CResourceManager::getInstance().init();

	CNetworkTask::getInstance().init();
	
//	string err;
//	if ( !mt3dInit(err) )
//	{
//		nlerror("Can't init 3D...");
//	}
	
//	if ( !mtFontInit(err) )
//	{
//		nlerror("Can't init Font...");
//	}
//	if ( !mtSound::init())
//	{
//		nlwarning("Can't init Sound...");
//	}
	
//	SleepTime = CConfigFileTask::getInstance().configFile().getVar("Sleep").asInt();

	setReportEmailFunction ((void*)sendEmail);

	CMtpTarget::getInstance().State = CMtpTarget::eLoginPassword;
	
	DisplayDebug = (CConfigFileTask::getInstance().configFile().getVar("DisplayDebug").asInt() == 1);

	CEntityManager::getInstance().init();

//	World.init();
	CLevelManager::getInstance().init();

//	Controler = new CControler;
	
//	Interface2d.init();

	if (CConfigFileTask::getInstance().configFile().getVar("Music").asInt() == 1 || CConfigFileTask::getInstance().configFile().getVar("Sound").asInt() == 1)
	{
//		soundState = eNone;
	}

	FirstResetCamera = true;

	//

	if (CMtpTarget::getInstance().State == CMtpTarget::eLoginPassword)
	{
		std::string login = CConfigFileTask::getInstance().configFile().getVar("Login").asString();
		std::string password = CConfigFileTask::getInstance().configFile().getVar("Password").asString();
		login = NLMISC::strlwr(login);
		password = NLMISC::strlwr(password);
		CConfigFileTask::getInstance().configFile().getVar("Login").setAsString(login);
		CConfigFileTask::getInstance().configFile().getVar("Password").setAsString(password);
		CConfigFileTask::getInstance().configFile().save();
		CMtpTarget::getInstance().State = CMtpTarget::eConnect;
	}
	
	if (CMtpTarget::getInstance().State == CMtpTarget::eConnect)
	{
//		Interface2d.network();
		CMtpTarget::getInstance().State = CMtpTarget::eBeforeFirstSession;
//		if(nelLogoParticle!=0)
//		{
//			C3DTask::getInstance().scene().deleteInstance(nelLogoParticle);
//			nelLogoParticle = 0;
//		}
//		if(C3DTask::getInstance().levelParticle()!=0)
//			C3DTask::getInstance().levelParticle()->show();

//		if(CConfigFileTask::getInstance().configFile().getVar("CaptureMouse").asInt() == 1)
//		{
//			C3DTask::getInstance().driver().showCursor(false);
//			C3DTask::getInstance().driver().setCapture(true);
//		}
//		C3DTask::getInstance().mouseListener().init();
	}
}
/*
void mtpTarget::updateSound ()
{
}
*/
/*void mtpTarget::update()
{
//	mt3dUpdate();

//	updateTime ();

	if(GNewSession)
		loadNewSession();

	CLevelManager::getInstance().update();
	CEntityManager::getInstance().update();

	if (State == eBeforeFirstSession)
	{
		Interface2d.display(mtInterface::eDisplayBeforeFirstSession);
		if(FirstResetCamera)
		{
			FirstResetCamera = false;
			CMtpTarget::getInstance().controler().Camera.reset();
		}
	}
	
	CMtpTarget::getInstance().controler().update();

	if (FollowEntity)
		C3DTask::getInstance().scene().getCam()->setMatrix(*CMtpTarget::getInstance().controler().Camera.getMatrixFollow());

//	if(nelLevelParticle!=0)
//		nelLevelParticle->setPos(C3DTask::getInstance().scene().getCam()->getMatrix().getPos());
	
	updateSound ();

	// todo : set the listener velocity !
	CMatrix *cameraMatrix = CMtpTarget::getInstance().controler().Camera.getMatrix();
	CSoundManager::getInstance().updateListener(cameraMatrix->getPos(), CVector::Null, cameraMatrix->getJ(), cameraMatrix->getK());

	NLMISC::CConfigFile::checkConfigFiles();
//	mtLevelManager::update ();

	C3DTask::getInstance().clear();
	//	mt3dClear ();

	mtpTarget::getInstance().render();
//	mt3dRender ();
	//mtpTarget::getInstance().World.render();
	CEntityManager::getInstance().render();
	mtpTarget::getInstance().renderInterface();

//	mt3dSwap ();

//	CSoundManager::getInstance().update();
	
	// Give some cpu if user wanted in cfg
	if (SleepTime >= 0)
		nlSleep (SleepTime);
}*/

/*void mtpTarget::render()
{
}

void mtpTarget::release()
{
}
*/

//TODO skeet no more used ?
#if 0
void mtpTarget::renderInterface()
{
	/*
	if (CMtpTarget::getInstance().State == CMtpTarget::eStartSession)
	{
		Interface2d.display(mtInterface::eDisplayWaitReady);
	}
	*/
	if (CMtpTarget::getInstance().State == CMtpTarget::eReady)
	{
		TimeBeforeSessionStart -= (float)CTimeTask::getInstance().deltaTime();
		/*
		Interface2d.display(mtInterface::eDisplayStartSession);
		Interface2d.setPartTime(TimeBeforeSessionStart);
		*/
		
		if (TimeBeforeSessionStart > 4.0f)
		{
			CMtpTarget::getInstance().controler().Camera.reset();
			CMtpTarget::getInstance().controler().Camera.ForcedSpeed = 0.0f;
		}
		else
			CMtpTarget::getInstance().controler().Camera.ForcedSpeed = 1.0f;
		
		if (TimeBeforeSessionStart <= 0)
		{
			TimeBeforeSessionStart = 0;
			CMtpTarget::getInstance().State = CMtpTarget::eGame;
		}
	}
	if (CMtpTarget::getInstance().State == CMtpTarget::eGame)
	{
		//CMtpTarget::getInstance().controler().Camera.reset();
		TimeBeforeSessionStart += (float)CTimeTask::getInstance().deltaTime();
		TimeBeforeTimeout -= (float)CTimeTask::getInstance().deltaTime();
		/*
		Interface2d.setPartTime(TimeBeforeSessionStart);
		Interface2d.display(mtInterface::eDisplayGame);
		*/
	}
	/*
	if (CMtpTarget::getInstance().State == CMtpTarget::eEndSession)
	{
		Interface2d.display(mtInterface::eDisplayEndSession);
	}
	Interface2d.render(CMtpTarget::getInstance().controler().getControledEntity());
	*/
	
	
	if (DisplayDebug)
	{
		CFontManager::getInstance().littlePrintf(0, 6, "pos %.2f %.2f %.2f", C3DTask::getInstance().scene().getCam().getMatrix().getPos().x, C3DTask::getInstance().scene().getCam().getMatrix().getPos().y, C3DTask::getInstance().scene().getCam().getMatrix().getPos().z);
		CQuat q = C3DTask::getInstance().scene().getCam().getMatrix().getRot();
		CAngleAxis aa = q.getAngleAxis();
		CFontManager::getInstance().littlePrintf(0, 7, "rot %.2f %.2f %.2f %.2f", aa.Axis.x, aa.Axis.y, aa.Axis.z, aa.Angle);

		CFontManager::getInstance().littlePrintf(0, 8, "%.2ffps %.2fms", CTimeTask::getInstance().fps(), CTimeTask::getInstance().spf()*1000.0f);

		CFontManager::getInstance().littlePrintf(0, 9, "FxNbFaceAsked %.2f", C3DTask::getInstance().scene().getGroupNbFaceAsked("Fx"));
		CFontManager::getInstance().littlePrintf(0, 10, "FxNbFaceMax %d", C3DTask::getInstance().scene().getGroupLoadMaxPolygon("Fx"));
		CFontManager::getInstance().littlePrintf(0, 11, "TotalNbFacesAsked %.2f", C3DTask::getInstance().scene().getNbFaceAsked());

		CPrimitiveProfile in, out;
		C3DTask::getInstance().driver().profileRenderedPrimitives(in, out);
		CFontManager::getInstance().littlePrintf(0, 12, "in:  %d %d %d %d %d", in.NPoints, in.NLines, in.NTriangles, in.NQuads, in.NTriangleStrips);
		CFontManager::getInstance().littlePrintf(0, 13, "out: %d %d %d %d %d", out.NPoints, out.NLines, out.NTriangles, out.NQuads, out.NTriangleStrips);

		uint8 eid = CMtpTarget::getInstance().controler().getControledEntity();
		CFontManager::getInstance().littlePrintf(0, 16, "Cam (%.2f %.2f %.2f)", CMtpTarget::getInstance().controler().Camera.getMatrixFollow()->getPos().x, CMtpTarget::getInstance().controler().Camera.getMatrixFollow()->getPos().y, CMtpTarget::getInstance().controler().Camera.getMatrixFollow()->getPos().z);
	}
}
#endif

void mtpTarget::everybodyReady()
{
	// everybody is ok, let s count down
	CMtpTarget::getInstance().State = CMtpTarget::eReady;
	CEntityManager::getInstance().everybodyReady(true);
}
	
void mtpTarget::endSession()
{
	// end of a session
	CMtpTarget::getInstance().State = CMtpTarget::eEndSession;
	C3DTask::getInstance().EnableExternalCamera = false;
	CHudTask::getInstance().messages.clear();
}

/*
void CMtpTarget::resetFollowedEntity()
{
	CMtpTarget::getInstance().controler().Camera.setFollowedEntity(CMtpTarget::getInstance().controler().getControledEntity());
	CMtpTarget::getInstance().controler().ViewedEId = CMtpTarget::getInstance().controler().getControledEntity();

//	for(uint32 entityPos = 0; entityPos < mtpTarget::getInstance().World.count(); entityPos++)
//	{
//		if(mtpTarget::getInstance().World.get(entityPos) == CMtpTarget::getInstance().controler().getControledEntity())
//		{
//			CMtpTarget::getInstance().controler().PlayerViewedPos = entityPos;
//			break;
//		}
//	}
}
*/


