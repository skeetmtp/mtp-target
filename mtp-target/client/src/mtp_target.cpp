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

#include "global.h"
#include "entity.h"
#include "gui_task.h"
#include "3d_task.h"
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
#include "lens_flare_task.h"
#include "background_task.h"
#include "resource_manager.h"
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

//NLMISC::CConfigFile CConfigFileTask::instance().configFile();

mtpTarget *mtpTarget::Instance = 0;

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
	DoError = false;
	// setup default task
	CTaskManager::instance().add(CConfigFileTask::instance(), 10);
	CTaskManager::instance().add(CTimeTask::instance(), 20);
	CTaskManager::instance().add(CEntityManager::instance(), 24);
	CTaskManager::instance().add(CNetworkTask::instance(), 25);
	CTaskManager::instance().add(CResourceManager::instance(), 30);
	CTaskManager::instance().add(C3DTask::instance(), 110);
	CTaskManager::instance().add(CGuiTask::instance(), 130);
	CTaskManager::instance().add(CEditorTask::instance(), 120);
	CTaskManager::instance().add(CLensFlareTask::instance(), 140);
	CTaskManager::instance().add(CSwap3DTask::instance(), 10000);
	CTaskManager::instance().add(CFontManager::instance(), 40);

	if(CConfigFileTask::instance().configFile().getVar("Sound").asInt() == 1)
		CTaskManager::instance().add(CSoundManager::instance(), 50);
	
	Controler = new CControler;

	// start with intro task
	CTaskManager::instance().add(CIntroTask::instance(), 60);

	reset();
}

void CMtpTarget::error()
{
	DoError = true;
}

void CMtpTarget::reset()
{
	CEntityManager::instance().reset();
	controler().reset();
	NewSession = false;
	TimeBeforeSessionStart = 0.0f;
	TimeBeforeTimeout = 0.0f;
	State = CMtpTarget::eBeforeFirstSession;
}

void CMtpTarget::_error()
{
	reset();
	CGameTask::instance().stop();
	CEditorTask::instance().stop();
	CEditorTask::instance().enable(false);
	CBackgroundTask::instance().restart();
	//CIntroTask::instance().reset();
	CIntroTask::instance().error(string("Server lost !"));
	CIntroTask::instance().restart();
	CLevelManager::instance().release();
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
		C3DTask::instance().scene().getCam()->setMatrix(*CMtpTarget::instance().controler().Camera.getMatrixFollow());
	}

	if(CMtpTarget::instance().State == CMtpTarget::eReady)
	{
		TimeBeforeSessionStart -= (float)CTimeTask::instance().deltaTime();

		if(TimeBeforeSessionStart > 4.0f)
		{
			CMtpTarget::instance().controler().Camera.reset();
			CMtpTarget::instance().controler().Camera.ForcedSpeed = 0.0f;
		}
		else
			CMtpTarget::instance().controler().Camera.ForcedSpeed = 1.0f;
		
		if(TimeBeforeSessionStart <= 0)
		{
			TimeBeforeSessionStart = 0;
			CMtpTarget::instance().State = CMtpTarget::eGame;
			CEntityManager::instance().reset();			
		}
	}
	if(CMtpTarget::instance().State == CMtpTarget::eGame)
	{
		//CMtpTarget::instance().controler().Camera.reset();
		TimeBeforeSessionStart += (float)CTimeTask::instance().deltaTime();
		TimeBeforeTimeout -= (float)CTimeTask::instance().deltaTime();
	}
}

void CMtpTarget::loadNewSession()
{
	nlassert(NewSession);
	
	NewSession = false;
	
	if(NewLevelName.empty())
		return;

	if(TimeBeforeSessionStart != 0.0f)
	{
		// i'm in the new session and all other entities, reset spectator
		CEntityManager::instance().resetSpectator();
		if(!CEditorTask::instance().enable())
			FollowEntity = true;
	}
	else
	{
		FollowEntity = false;		
	}
	
	CEntityManager::instance().load3d();
	
	CLevelManager::instance().loadLevel(NewLevelName);
	
	CEntityManager::instance().startSession();
	
	CMtpTarget::instance().controler().Camera.reset();
	/*
	if (CMtpTarget::instance().controler().getControledEntity() != 255)
		CEntityManager::instance()[CMtpTarget::instance().controler().getControledEntity()].interpolator.reset();
	*/
	resetFollowedEntity();
	controler().Camera.reset();

	State = eStartSession;
	
	nlinfo ("loaded level");
	
	// say to server that we are ready
	CNetworkTask::instance().ready();
	
	// now we wait the message that say that everybody is ready
	
	// create the session replay
	
	if (SessionFile)
		fclose (SessionFile);
	
	if(!NLMISC::CFile::isDirectory("replay"))
		NLMISC::CFile::createDirectory("replay");
	
	if(CConfigFileTask::instance().configFile().getVar("GenerateReplay").asInt() == 1)
	{
		SessionFile = fopen (NLMISC::CFile::findNewFile("replay/session.mtr").c_str(), "wt");
		nlassert (SessionFile != 0);
	}
	
	if (SessionFile)
		fprintf(SessionFile, "%hu\n", (uint16)CEntityManager::instance().size());
	
	int self;
	
	uint8 eid = CEntityManager::instance().findFirstEId();
	if (eid != 255 && SessionFile)
	{
		do
		{
			self = (eid == CMtpTarget::instance().controler().getControledEntity())?1:0;
			fprintf(SessionFile, "%hu %s %d %d\n", (uint16)eid, CEntityManager::instance()[eid].name().c_str(), self, CEntityManager::instance()[eid].currentScore());
			eid = CEntityManager::instance().findNextEId(eid);
		}
		while(eid != CEntityManager::instance().findFirstEId());
	}
}

void CMtpTarget::startSession(float timeBeforeSessionStart, float timeBeforeTimeout, const string &levelName)
{
	nlinfo("level '%s' loaded, it timeouts in %g seconds", levelName.c_str(), timeBeforeTimeout);
	
	NewSession = true;
	TimeBeforeSessionStart = timeBeforeSessionStart;
	TimeBeforeTimeout = timeBeforeTimeout;
	NewLevelName = levelName;
}

void CMtpTarget::resetFollowedEntity()
{
	CMtpTarget::instance().controler().Camera.setFollowedEntity(CMtpTarget::instance().controler().getControledEntity());
	CMtpTarget::instance().controler().ViewedEId = CMtpTarget::instance().controler().getControledEntity();
	
	//	for(uint32 entityPos = 0; entityPos < mtpTarget::instance().World.count(); entityPos++)
	//	{
	//		if(mtpTarget::instance().World.get(entityPos) == CMtpTarget::instance().controler().getControledEntity())
	//		{
	//			CMtpTarget::instance().controler().PlayerViewedPos = entityPos;
	//			break;
	//		}
	//	}
}






















mtpTarget &mtpTarget::instance()
{
	if (!Instance)
		Instance = new mtpTarget;

	return *Instance;
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
		CConfigFileTask::instance().configFile().load (mtpTargetConfigFilename);
	}
	catch (NLMISC::EConfigFile &e)
	{
		nlerror(e.what());
	}
	

	CResourceManager::instance().init();

	CNetworkTask::instance().init();
	
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
	
//	SleepTime = CConfigFileTask::instance().configFile().getVar("Sleep").asInt();
	

	if(NLMISC::DebugLog!=0)
		NLMISC::DebugLog->addNegativeFilter("LNET");

	setReportEmailFunction ((void*)sendEmail);

	if (Cookie.empty())
		CMtpTarget::instance().State = CMtpTarget::eLoginPassword;
	else
		CMtpTarget::instance().State = CMtpTarget::eConnect;
	
	DisplayDebug = (CConfigFileTask::instance().configFile().getVar("DisplayDebug").asInt() == 1);

	CEntityManager::instance().init();

//	World.init();
	CLevelManager::instance().init();

//	Controler = new CControler;
	
//	Interface2d.init();

	if (CConfigFileTask::instance().configFile().getVar("Music").asInt() == 1 || CConfigFileTask::instance().configFile().getVar("Sound").asInt() == 1)
	{
//		soundState = eNone;
	}

	FirstResetCamera = true;

	if (!ReplayFile.empty())
	{
		FILE *fp = fopen (ReplayFile.c_str(), "rt");
		if (fp != 0)
		{
			uint nbplayer, eid, self, totalScore;
			char name[100];
			
			fscanf (fp, "%d", &nbplayer);

			// add players
			for(uint i = 0; i < nbplayer; i++)
			{
				fscanf (fp, "%d %s %d %d", &eid, &name, &self, &totalScore);

				CRGBA col(255,255,255);
				CEntityManager::instance().add(eid, name, totalScore, col, false);
			
				//CEntity *entity = new CEntity(CRGBA(255,255,255));
				//entity->setId(eid);
				//mtpTarget::instance().World.add(entity);
				//entity->name = name;
				//CEntityManager::instance()[eid].setScore(0, score);

				if (self)
				{
					CMtpTarget::instance().controler().setControledEntity(eid);
				}
			}

			char cmd[10];
			float t, x, y, z;
			while (!feof (fp))
			{
				fscanf (fp, "%d %s", &eid, &cmd);
				if (string(cmd) == "PO")
				{
					fscanf (fp, "%f %f %f %f", &t, &x, &y, &z);

					if (eid != 255)
					  {
					    CVector v(x, y, z);
						CEntityManager::instance()[eid].interpolator.addKey(CEntityInterpolatorKey(CEntityState(v,false),t));//.position(v,t, false); //put the entity in the good position
					  }
					else
						nlwarning ("%d introuvable", eid);
				}
				else if (string(cmd) == "OC")
				{
				}
				else
				{
					nlwarning ("Unknown command %s for user %d", cmd, eid);
				}
			}

			fclose (fp);
		}

		CMtpTarget::instance().State = CMtpTarget::eReady;
	}

	//

	if (CMtpTarget::instance().State == CMtpTarget::eLoginPassword)
	{
		std::string login = CConfigFileTask::instance().configFile().getVar("Login").asString();
		std::string password = CConfigFileTask::instance().configFile().getVar("Password").asString();
		login = NLMISC::strlwr(login);
		password = NLMISC::strlwr(password);
//		Interface2d.loginScreen(login, password);
		login = NLMISC::strlwr(login);
		password = NLMISC::strlwr(password);
		CConfigFileTask::instance().configFile().getVar("Login").setAsString(login);
		CConfigFileTask::instance().configFile().getVar("Password").setAsString(password);
		CConfigFileTask::instance().configFile().save();
		CMtpTarget::instance().State = CMtpTarget::eConnect;
	}
	
	if (CMtpTarget::instance().State == CMtpTarget::eConnect)
	{
//		Interface2d.network();
		CMtpTarget::instance().State = CMtpTarget::eBeforeFirstSession;
//		if(nelLogoParticle!=0)
//		{
//			C3DTask::instance().scene().deleteInstance(nelLogoParticle);
//			nelLogoParticle = 0;
//		}
//		if(C3DTask::instance().levelParticle()!=0)
//			C3DTask::instance().levelParticle()->show();

//		if(CConfigFileTask::instance().configFile().getVar("CaptureMouse").asInt() == 1)
//		{
//			C3DTask::instance().driver().showCursor(false);
//			C3DTask::instance().driver().setCapture(true);
//		}
//		C3DTask::instance().mouseListener().init();
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

	CLevelManager::instance().update();
	CEntityManager::instance().update();

	if (State == eBeforeFirstSession)
	{
		Interface2d.display(mtInterface::eDisplayBeforeFirstSession);
		if(FirstResetCamera)
		{
			FirstResetCamera = false;
			CMtpTarget::instance().controler().Camera.reset();
		}
	}
	
	CMtpTarget::instance().controler().update();

	if (FollowEntity)
		C3DTask::instance().scene().getCam()->setMatrix(*CMtpTarget::instance().controler().Camera.getMatrixFollow());

//	if(nelLevelParticle!=0)
//		nelLevelParticle->setPos(C3DTask::instance().scene().getCam()->getMatrix().getPos());
	
	updateSound ();

	// todo : set the listener velocity !
	CMatrix *cameraMatrix = CMtpTarget::instance().controler().Camera.getMatrix();
	CSoundManager::instance().updateListener(cameraMatrix->getPos(), CVector::Null, cameraMatrix->getJ(), cameraMatrix->getK());

	NLMISC::CConfigFile::checkConfigFiles();
//	mtLevelManager::update ();

	C3DTask::instance().clear();
	//	mt3dClear ();

	mtpTarget::instance().render();
//	mt3dRender ();
	//mtpTarget::instance().World.render();
	CEntityManager::instance().render();
	mtpTarget::instance().renderInterface();

//	mt3dSwap ();

//	CSoundManager::instance().update();
	
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

void mtpTarget::renderInterface()
{
	/*
	if (CMtpTarget::instance().State == CMtpTarget::eStartSession)
	{
		Interface2d.display(mtInterface::eDisplayWaitReady);
	}
	*/
	if (CMtpTarget::instance().State == CMtpTarget::eReady)
	{
		TimeBeforeSessionStart -= (float)CTimeTask::instance().deltaTime();
		/*
		Interface2d.display(mtInterface::eDisplayStartSession);
		Interface2d.setPartTime(TimeBeforeSessionStart);
		*/
		
		if (TimeBeforeSessionStart > 4.0f)
		{
			CMtpTarget::instance().controler().Camera.reset();
			CMtpTarget::instance().controler().Camera.ForcedSpeed = 0.0f;
		}
		else
			CMtpTarget::instance().controler().Camera.ForcedSpeed = 1.0f;
		
		if (TimeBeforeSessionStart <= 0)
		{
			TimeBeforeSessionStart = 0;
			CMtpTarget::instance().State = CMtpTarget::eGame;
		}
	}
	if (CMtpTarget::instance().State == CMtpTarget::eGame)
	{
		//CMtpTarget::instance().controler().Camera.reset();
		TimeBeforeSessionStart += (float)CTimeTask::instance().deltaTime();
		TimeBeforeTimeout -= (float)CTimeTask::instance().deltaTime();
		/*
		Interface2d.setPartTime(TimeBeforeSessionStart);
		Interface2d.display(mtInterface::eDisplayGame);
		*/
	}
	/*
	if (CMtpTarget::instance().State == CMtpTarget::eEndSession)
	{
		Interface2d.display(mtInterface::eDisplayEndSession);
	}
	Interface2d.render(CMtpTarget::instance().controler().getControledEntity());
	*/
	
	
	if (DisplayDebug)
	{
		CFontManager::instance().littlePrintf(0, 6, "pos %.2f %.2f %.2f", C3DTask::instance().scene().getCam()->getMatrix().getPos().x, C3DTask::instance().scene().getCam()->getMatrix().getPos().y, C3DTask::instance().scene().getCam()->getMatrix().getPos().z);
		CQuat q = C3DTask::instance().scene().getCam()->getMatrix().getRot();
		CAngleAxis aa = q.getAngleAxis();
		CFontManager::instance().littlePrintf(0, 7, "rot %.2f %.2f %.2f %.2f", aa.Axis.x, aa.Axis.y, aa.Axis.z, aa.Angle);

		CFontManager::instance().littlePrintf(0, 8, "%.2ffps %.2fms", CTimeTask::instance().fps(), CTimeTask::instance().spf()*1000.0f);

		CFontManager::instance().littlePrintf(0, 9, "FxNbFaceAsked %.2f", C3DTask::instance().scene().getGroupNbFaceAsked("Fx"));
		CFontManager::instance().littlePrintf(0, 10, "FxNbFaceMax %d", C3DTask::instance().scene().getGroupLoadMaxPolygon("Fx"));
		CFontManager::instance().littlePrintf(0, 11, "TotalNbFacesAsked %.2f", C3DTask::instance().scene().getNbFaceAsked());

		CPrimitiveProfile in, out;
		C3DTask::instance().driver().profileRenderedPrimitives(in, out);
		CFontManager::instance().littlePrintf(0, 12, "in:  %d %d %d %d %d", in.NPoints, in.NLines, in.NTriangles, in.NQuads, in.NTriangleStrips);
		CFontManager::instance().littlePrintf(0, 13, "out: %d %d %d %d %d", out.NPoints, out.NLines, out.NTriangles, out.NQuads, out.NTriangleStrips);

		uint8 eid = CMtpTarget::instance().controler().getControledEntity();
		CFontManager::instance().littlePrintf(0, 16, "Cam (%.2f %.2f %.2f)", CMtpTarget::instance().controler().Camera.getMatrixFollow()->getPos().x, CMtpTarget::instance().controler().Camera.getMatrixFollow()->getPos().y, CMtpTarget::instance().controler().Camera.getMatrixFollow()->getPos().z);
	}
}


void mtpTarget::everybodyReady()
{
	// everybody is ok, let s count down
	CMtpTarget::instance().State = CMtpTarget::eReady;
	CLevelManager::instance().currentLevel().reset();
}
	
void mtpTarget::endSession()
{
	// end of a session
	CMtpTarget::instance().State = CMtpTarget::eEndSession;
}

/*
void CMtpTarget::resetFollowedEntity()
{
	CMtpTarget::instance().controler().Camera.setFollowedEntity(CMtpTarget::instance().controler().getControledEntity());
	CMtpTarget::instance().controler().ViewedEId = CMtpTarget::instance().controler().getControledEntity();

//	for(uint32 entityPos = 0; entityPos < mtpTarget::instance().World.count(); entityPos++)
//	{
//		if(mtpTarget::instance().World.get(entityPos) == CMtpTarget::instance().controler().getControledEntity())
//		{
//			CMtpTarget::instance().controler().PlayerViewedPos = entityPos;
//			break;
//		}
//	}
}
*/
