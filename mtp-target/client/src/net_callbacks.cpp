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

#include <nel/misc/types_nl.h>

#ifdef NL_OS_WINDOWS
#	include <windows.h>
#	undef min
#	undef max
#endif

#include <nel/misc/path.h>

#include "3d_task.h"
#include "hud_task.h"
#include "chat_task.h"
#include "mtp_target.h"
#include "network_task.h"
#include "net_callbacks.h"
#include "level_manager.h"
#include "entity_manager.h"
#include "resource_manager.h"
#include "config_file_task.h"
#include "../../common/custom_floating_point.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;


//
// Variables
//

extern FILE *SessionFile;


//
// Functions
//

static void cbChat(CNetMessage &msgin)
{
	nlinfo("NET: cbChat");
	string msg;
	while(msgin.getPos() < (sint32)msgin.length())
	{
		msgin.serial(msg);
		CChatTask::instance().addLine(msg);
	}
}

static void cbError(CNetMessage &msgin)
{
	string msg;
	msgin.serial(msg);
	nlinfo("NET: cbError msg='%s'", msg.c_str());
	
#ifdef NL_OS_WINDOWS
	if(msg.find("bad client version") != string::npos)
	{
		// the client is not up to date, ask the user to update
		if(MessageBox(NULL, "Your client is not up to date. You *must* download a new version from the mtp target web site.\r\nDo you want to automatically quit and go to the download page?", "Warning", MB_ICONWARNING|MB_YESNO) == IDYES)
		{
			openURL("http://mtptarget.free.fr/download.php");
			exit(0);
		}
	}
#endif

	CMtpTarget::instance().error(msg);	
}

static void cbLogin(CNetMessage &msgin)
{
	bool self;
	uint8 eid;
	string name;
	sint32 totalScore;
	CRGBA color;
	string texture;
	bool spec;

	msgin.serial(self, eid, name, totalScore);
	msgin.serial(color, texture, spec);
	nlinfo("NET: cbLogin self=%d eid=%hu name='%s' totalScore='%d' color=(%d,%d,%d) texture='%s' spec=%d", 
		self, (uint16)eid, name.c_str(), totalScore, color.R, color.G, color.B, texture.c_str(), spec);
		
	nldebug("player list.size = %d", CEntityManager::instance().size());

	CEntityManager::instance().add(eid, name, totalScore, color, texture, spec, self);

	if(self)
	{
		CMtpTarget::instance().displayTutorialInfo(totalScore<=CConfigFileTask::instance().configFile().getVar("MinTotalScoreToHideTutorial").asInt());

		string levelName;
		float timeBeforeTimeout;
		msgin.serial(levelName, timeBeforeTimeout);
		nlinfo("levelName='%s' timeBeforeTimeout=%f", levelName.c_str(), timeBeforeTimeout);
		
		if (!levelName.empty())
			CMtpTarget::instance().startSession(0, timeBeforeTimeout/1000.0f, levelName, "", "");
		else
			CMtpTarget::instance().timeBeforeTimeout(timeBeforeTimeout/1000.0f);
	}
	else
	{
		CChatTask::instance().addLine(toString("%s comes in !", name.c_str()));
	}
}

static void cbLogout(CNetMessage &msgin)
{
	uint8 eid;
	msgin.serial(eid);
	nlinfo("NET: cbLogout eid=%hu", (uint16)eid);

	// check if the player exists
	if(!CEntityManager::instance().exist(eid)) { nlwarning("The eid doesn't exist"); return; }

	// if it's my eid, it means that i have to disconnect because i was kicked out from the server
	if(CMtpTarget::instance().controler().getControledEntity()==eid)
	{
		CMtpTarget::instance().error("You have been kicked");
		//nlerror("You have been kicked");
	}
	else
	{
		CChatTask::instance().addLine(toString("%s leaves !", CEntityManager::instance()[eid].name().c_str()));
	}

	CEntityManager::instance().remove(eid);
}

static void cbOpenClose(CNetMessage &msgin)
{
	uint8 eid;
	msgin.serial(eid);
	nlinfo("NET: cbOpenClose eid=%hu", (uint16)eid);

	// check if the player exists
	if(!CEntityManager::instance().exist(eid)) { nlwarning("The eid doesn't exist"); return; }
	
	//CEntityManager::instance()[eid].addOpenCloseKey = true;
	CEntityManager::instance()[eid].swapOpenClose();

	if(SessionFile) fprintf(SessionFile, "%hu OC\n", (uint16)eid);
}

static void cbUpdate(CNetMessage &msgin)
{
	//nlinfo("cbUpdate %f",CTimeTask::instance().time());
	float rsxTime;
	uint8 eid;
	CVector pos;
	CVector dpos;
	CVector ddpos;
	//uint16 ping;

	// reply to the update first (used for the ping)
	CNetMessage msgout(CNetMessage::Update);
	CNetworkTask::instance().send(msgout);
	
	//msgin.serial (rsxTime);

	//while(msgin.getPos() < (sint32)msgin.length())
	//nlassert(CEntityManager::instance().updateListId.size());
	std::list <uint8 >::iterator it;
	//for(it=CEntityManager::instance().updateListId.begin();it!=CEntityManager::instance().updateListId.end();it++)
	for(eid=0;eid<255;eid++)
	{
		if(CEntityManager::instance().exist(eid))
		{
			if(msgin.getPos() >= (sint32)msgin.length())
			{
				nlwarning("cbUpdate not enough data in message (stopped at eid = %d)",eid);
				break;
			}
			//eid = *it;
			float deltaCoef = 100;

			/*
			uint8 dxx,sxx;
			msgin.serial(sxx,dxx);
			dpos.x = convert8_8fp(sxx,dxx);
			uint8 dxy,sxy;
			msgin.serial(sxy,dxy);
			dpos.y = convert8_8fp(sxy,dxy);
			uint8 dxz,sxz;
			msgin.serial(sxz,dxz);
			dpos.z = convert8_8fp(sxz,dxz);
			*/

			uint32 bits;
			msgin.serial(bits);
			packBit32 pb32(bits);
			uint32 ndx,nsx;
			pb32.unpackBits(nsx,6);
			pb32.unpackBits(ndx,4);//z
			//		nlinfo(">>>>z dx/ndx = %d/%d ",dxz,ndx);
			//		nlinfo(">>>>z sx/nsx = %d/%d ",sxz,nsx);
			//nlassert(dxz==ndx);
			//nlassert(sxz==nsx);
			dpos.z = convert8_8fp(nsx,ndx);
			pb32.unpackBits(nsx,6);
			pb32.unpackBits(ndx,4);//y
			//		nlinfo(">>>>y dx/ndx = %d/%d ",dxy,ndx);
			//		nlinfo(">>>>y sx/nsx = %d/%d ",sxy,nsx);
			//nlassert(dxy==ndx);
			//nlassert(sxy==nsx);
			dpos.y = convert8_8fp(nsx,ndx);
			pb32.unpackBits(nsx,6);
			pb32.unpackBits(ndx,4);//x
			//		nlinfo(">>>>x dx/ndx = %d/%d ",dxx,ndx);
			//		nlinfo(">>>>x sx/nsx = %d/%d ",sxx,nsx);
			//nlassert(dxx==ndx);
			//nlassert(sxx==nsx);
			dpos.x = convert8_8fp(nsx,ndx);
			

			if(CEntityManager::instance().exist(eid))
			{
				pos = CEntityManager::instance()[eid].LastSent2OthersPos + dpos;
				CEntityManager::instance()[eid].LastSent2OthersPos = pos;
				CEntityManager::instance()[eid].LastSent2MePos = pos;
				if(DisplayDebug)
					nlinfo("TCP update client %hu a %g %g %g ping %hu", (uint16)eid, pos.x, pos.y, pos.z);
				if(SessionFile) 
					fprintf(SessionFile, "%hu PO %f %f %f\n", (uint16)eid, pos.x, pos.y, pos.z);
				//TODO remove rsxTime param
				bool oc = false;
				if(CEntityManager::instance()[eid].addOpenCloseKey)
				{
					CEntityManager::instance()[eid].addOpenCloseKey = false;
					oc = true;
				}
				CCrashEvent ce(false,CVector::Null);
				if(CEntityManager::instance()[eid].addCrashEventKey.crash)
				{
					ce = CEntityManager::instance()[eid].addCrashEventKey;
					CEntityManager::instance()[eid].addCrashEventKey.crash = false;
				}
				CEntityManager::instance()[eid].interpolator().addKey(CEntityInterpolatorKey(CEntityState(pos,false,oc,ce),rsxTime));
				//CEntityManager::instance()[eid].ping(ping);
			}
			else
			{
				nlstop;
				nlwarning("Received a position of an unknown entity %hu", (uint16)eid);
			}
		}
	}
}

static void cbUpdateOne(CNetMessage &msgin)
{
	//nlinfo("cbUpdateOne %f",CTimeTask::instance().time());
	float rsxTime = 0.0f;
	uint8 eid;
	CVector pos;
	CVector dpos;
	CVector ddpos;
	//uint16 ping;
	
	// reply to the update first (used for the ping)
	//CNetMessage msgout(CNetMessage::Update);
	//CNetworkTask::instance().send(msgout);
	
	//msgin.serial (rsxTime);
	
	//while(msgin.getPos() < (sint32)msgin.length())
	{
		float deltaCoef = 100;
		//msgin.serial(eid);
		eid = CMtpTarget::instance().controler().getControledEntity();
		/*
		uint8 dxx,sxx;
		msgin.serial(sxx,dxx);
		dpos.x = convert8_8fp(sxx,dxx);
		uint8 dxy,sxy;
		msgin.serial(sxy,dxy);
		dpos.y = convert8_8fp(sxy,dxy);
		uint8 dxz,sxz;
		msgin.serial(sxz,dxz);
		dpos.z = convert8_8fp(sxz,dxz);
		*/

		uint32 bits;
		msgin.serial(bits);
		packBit32 pb32(bits);
		uint32 ndx,nsx;
		pb32.unpackBits(nsx,6);
		pb32.unpackBits(ndx,4);//z
		//		nlinfo(">>>>z dx/ndx = %d/%d ",dxz,ndx);
		//		nlinfo(">>>>z sx/nsx = %d/%d ",sxz,nsx);
		//nlassert(dxz==ndx);
		//nlassert(sxz==nsx);
		dpos.z = convert8_8fp(nsx,ndx);
		pb32.unpackBits(nsx,6);
		pb32.unpackBits(ndx,4);//y
		//		nlinfo(">>>>y dx/ndx = %d/%d ",dxy,ndx);
		//		nlinfo(">>>>y sx/nsx = %d/%d ",sxy,nsx);
		//nlassert(dxy==ndx);
		//nlassert(sxy==nsx);
		dpos.y = convert8_8fp(nsx,ndx);
		pb32.unpackBits(nsx,6);
		pb32.unpackBits(ndx,4);//x
		//		nlinfo(">>>>x dx/ndx = %d/%d ",dxx,ndx);
		//		nlinfo(">>>>x sx/nsx = %d/%d ",sxx,nsx);
		//nlassert(dxx==ndx);
		//nlassert(sxx==nsx);
		dpos.x = convert8_8fp(nsx,ndx);
		
		//nlinfo(">>>>dx =  %f ",dpos.x - mdx);

		if(CEntityManager::instance().exist(eid))
		{
			pos = CEntityManager::instance()[eid].LastSent2MePos + dpos;
			CEntityManager::instance()[eid].LastSent2MePos = pos;
			if(DisplayDebug)
				nlinfo("TCP updateOne client %hu a %g %g %g ping %hu", (uint16)eid, pos.x, pos.y, pos.z);
			if(SessionFile) 
				fprintf(SessionFile, "%hu PO %f %f %f\n", (uint16)eid, pos.x, pos.y, pos.z);
			
			bool oc = false;
			if(CEntityManager::instance()[eid].addOpenCloseKey)
			{
				CEntityManager::instance()[eid].addOpenCloseKey = false;
				oc = true;
			}
			CCrashEvent ce(false,CVector::Null);
			if(CEntityManager::instance()[eid].addCrashEventKey.crash)
			{
				ce = CEntityManager::instance()[eid].addCrashEventKey;
				CEntityManager::instance()[eid].addCrashEventKey.crash = false;
			}
			CEntityManager::instance()[eid].interpolator().addKey(CEntityInterpolatorKey(CEntityState(pos,false,oc,ce),rsxTime));
			//CEntityManager::instance()[eid].ping(ping);
		}
		/*
		else
		{
			nlwarning("Received a position of an unknown entity %hu", (uint16)eid);
			nlstop;
		}
		*/
	}
}


static void cbFullUpdate(CNetMessage &msgin)
{
	//nlinfo("cbFullUpdate %f",CTimeTask::instance().time());
	float rsxTime = 0.0f;

	// reply to the update first (used for the ping)
	CNetMessage msgout(CNetMessage::Update);
	CNetworkTask::instance().send(msgout);
	
	//msgin.serial (rsxTime);

	//nlassert(CEntityManager::instance().updateListId.size());
	//std::list <uint8 >::iterator it;
	//for(it=CEntityManager::instance().updateListId.begin();it!=CEntityManager::instance().updateListId.end();it++)
	while(msgin.getPos() < (sint32)msgin.length())
	{
		//eid = *it;
		//CVector dpos;
		uint8 eid;
		CVector pos;
		uint16 ping;
		msgin.serial(eid, pos ,ping);
		//pos = CEntityManager::instance()[eid].LastSentPos + dpos;
		
		if(CEntityManager::instance().exist(eid))
		{
			CEntityManager::instance()[eid].LastSent2MePos = pos;
			CEntityManager::instance()[eid].LastSent2OthersPos = pos;
			if(DisplayDebug)
				nlinfo("TCP updateFull client %hu a %g %g %g ping %hu", (uint16)eid, pos.x, pos.y, pos.z, ping);

			if(SessionFile) 
				fprintf(SessionFile, "%hu PO %f %f %f\n", (uint16)eid, pos.x, pos.y, pos.z);

			bool oc = false;
			if(CEntityManager::instance()[eid].addOpenCloseKey)
			{
				CEntityManager::instance()[eid].addOpenCloseKey = false;
				oc = true;
			}
			CCrashEvent ce(false,CVector::Null);
			if(CEntityManager::instance()[eid].addCrashEventKey.crash)
			{
				ce = CEntityManager::instance()[eid].addCrashEventKey;
				CEntityManager::instance()[eid].addCrashEventKey.crash = false;
			}
			CEntityManager::instance()[eid].interpolator().addKey(CEntityInterpolatorKey(CEntityState(pos,false,oc,ce),rsxTime));
			CEntityManager::instance()[eid].ping(ping);
		}
		/*
		else
		{
			nlwarning("Received a position of an unknown entity %hu", (uint16)eid);
			nlstop;
		}
		*/
	}
}


static void cbUpdateList(CNetMessage &msgin)
{
	CEntityManager::instance().updateListId.clear();
	while(msgin.getPos() < (sint32)msgin.length())
	{
		uint8 eid;
		msgin.serial(eid);
		CEntityManager::instance().updateListId.push_back(eid);
	}
}


static void cbEditMode(CNetMessage &msgin)
{
	uint8 editMode;
	msgin.serial(editMode);
	nlinfo("NET: cbEditMode editMode=%hu", (uint16)editMode);

	// TODO: what is the goal of this function???
}

static void cbUpdateElement(CNetMessage &msgin)
{
	uint8 elementType, elementId, selectedBy;
	CVector pos, eulerRot;
	msgin.serial(elementType, elementId, selectedBy, pos, eulerRot);
	nlinfo("NET: cbUpdateElement");
	
	switch(elementType)
	{
	case CEditableElementCommon::Module:
		if(CLevelManager::instance().levelPresent())
			CLevelManager::instance().currentLevel().updateModule(elementId,pos,eulerRot,selectedBy);
		break;
	case CEditableElementCommon::StartPosition:
		if(CLevelManager::instance().levelPresent())
			CLevelManager::instance().currentLevel().updateStartPoint(elementId,pos,eulerRot,selectedBy);
		break;
	default:
		nlwarning("Unknown elemen type %hu", (uint16)elementType);
		break;
	}
}
	
static void cbReady(CNetMessage &msgin)
{
	// called when one player is ready
	uint8 eid;
	msgin.serial(eid);
	nlinfo("NET: cbReady eid=%hu", (uint16)eid);

	// check if the player exists
	if(!CEntityManager::instance().exist(eid)) { nlwarning("The eid doesn't exist"); return; }
	
	CEntityManager::instance()[eid].ready(true);
}

static void cbEverybodyReady(CNetMessage &msgin)
{
	nlinfo("NET: cbEverybodyReady");

	// called when everybody is ready, can start the countdown
	mtpTarget::instance().everybodyReady();
}

static void cbEnableElement(CNetMessage &msgin)
{
	uint8 elementId;
	bool enabled;
	msgin.serial(elementId, enabled);
	nlinfo("NET: cbEnableElement");
	
	if(CLevelManager::instance().levelPresent())
		CLevelManager::instance().currentLevel().getModule(elementId)->enabled(enabled);
}

static void cbRequestCRCKey(CNetMessage &msgin)
{
	string fn;
	CHashKey hashKey;
	msgin.serial(fn, hashKey);
	nlinfo("NET: cbRequestCRCKey fn='%s' hashKey='%s'", fn.c_str(), hashKey.toString().c_str());

	CResourceManager::instance().receivedCRC(fn);
}

static void cbRequestDownload(CNetMessage &msgin)
{
	string res;
	vector<uint8> buf;
	bool eof = false;
	uint32 fileSize = 0xFFFFFFFF;

	msgin.serial(res);
	nlinfo("NET: cbRequestDownload res = '%s'", res.c_str());

	bool receivedError = res.find("FILE:")!=0;

	if(!receivedError)
	{
		msgin.serial(fileSize);
		msgin.serialCont(buf);
		msgin.serial(eof);
	}
	CResourceManager::instance().receivedBlock(res, buf, eof, fileSize, receivedError);
}

static void cbDisplayText(CNetMessage &msgin)
{
	float x, y, s;
	CRGBA col;
	double duration;
	string message;
	msgin.serial(x, y, s, message, col, duration);
	nlinfo("NET: cbDisplayText x=%f y=%f s=%f col=(%d,%d,%d), duration=%f message='%s'",
		x, y, s, col.R, col.G, col.B, duration, message.c_str());

	CHudTask::instance().addMessage(CHudMessage(x,y,s,message,col,duration));
}

static void cbStartSession(CNetMessage &msgin)
{
	float timebeforestart, timeout;
	string levelName, str1, str2;
	vector<uint8> ranks;
	vector<uint8> eids;

	msgin.serial(timebeforestart, timeout, levelName, str1, str2);
	msgin.serialCont(ranks);
	msgin.serialCont(eids);
	
	nlinfo("NET: cbStartSession timebeforestart=%f timeout=%f levelName='%s' str1='%s' str2='%s'",
		timebeforestart, timeout, levelName.c_str(), str1.c_str(), str2.c_str());
	
	nlassert(ranks.size()==eids.size());

	for(uint32 i=0;i<eids.size();i++)
	{
		if(CEntityManager::instance().exist(eids[i]))
			CEntityManager::instance()[eids[i]].rank(ranks[i]);
	}

	CMtpTarget::instance().startSession(timebeforestart / 1000.0f, timeout / 1000.0f, levelName, str1, str2);

	/*
	msgin.serial (mtpTarget::instance().Interface2d.LevelName);
	msgin.serial (mtpTarget::instance().Interface2d.FullLevelName);
	msgin.serial (mtpTarget::instance().Interface2d.string1);
	msgin.serial (mtpTarget::instance().Interface2d.string2);
	msgin.serial (mtpTarget::instance().Interface2d.Author);

	mtpTarget::instance().startSession(timebeforestart / 1000, timeout / 1000, mtpTarget::instance().Interface2d.LevelName);
*/
  
}

static void cbSessionState(CNetMessage &msgin)
{
	string sn;
	msgin.serial(sn);
	nlinfo("NET: cbSessionState sn='%s'", sn.c_str());

	// TODO: what is the goal of this function???
}

static void cbEndSession(CNetMessage &msgin)
{
	nlinfo("NET: cbEndSession");

	while(msgin.getPos() < (sint32)msgin.length())
	{
		uint8 eid;
		sint32 currentScore, totalScore;
		
		msgin.serial(eid, currentScore, totalScore);
		
		//CEntity *entity = mtpTarget::instance().World.getEntityById(id);
		//nlassert(entity!=0);
		//entity->setScore(currentScore, totalScore);
		if(CEntityManager::instance().exist(eid))
		{
			CEntityManager::instance()[eid].currentScore(currentScore);
			CEntityManager::instance()[eid].totalScore(totalScore);
		}
	}
	mtpTarget::instance().endSession();
	
	if(SessionFile)
	{
		fclose (SessionFile);
		SessionFile = 0;
	}
}

static void cbExecLua(CNetMessage &msgin)
{
	string luaCode;
	msgin.serial(luaCode);
	nlinfo("NET: cbExecLua luaCode='%s'", luaCode.c_str());

	if(CLevelManager::instance().levelPresent())
		CLevelManager::instance().currentLevel().execLuaCode(luaCode);
}

static void cbCollideWhenFly(CNetMessage &msgin)
{
	uint8 eid;
	CVector pos;
	msgin.serial(eid, pos);
	nlinfo("NET: cbCollideWhenFly eid=%hu, pos=(%f,%f,%f)", (uint16)eid, pos.x, pos.y, pos.z);

	// check if the player exists
	if(!CEntityManager::instance().exist(eid)) { nlwarning("The eid doesn't exist"); return; }

	if(CMtpTarget::instance().controler().getControledEntity()==eid)
	{
		// it's my collide when fly
		C3DTask::instance().EnableExternalCamera = true;
	}

	CEntityManager::instance()[eid].addCrashEventKey = CCrashEvent(true,pos);
	if(SessionFile) 
		fprintf(SessionFile, "%hu CE %f %f %f\n", (uint16)eid,pos.x,pos.y,pos.z);
}

static void cbTimeArrival(CNetMessage &msgin)
{
	uint8 eid;
	float time;
	msgin.serial(eid, time);
	nlinfo("NET: cbTimeArrival eid=%hu, time=%f", (uint16)eid, time);
	
	// check if the player exists
	if(!CEntityManager::instance().exist(eid)) { nlwarning("The eid doesn't exist"); return; }
	
	if(CMtpTarget::instance().controler().getControledEntity()==eid)
	{
		// it's my arrival
		C3DTask::instance().EnableExternalCamera = true;
	}
}

//
// Callback handler
//

#define SWITCH_CASE(_n) case CNetMessage::_n: cb##_n(msgin); break

void netCallbacksHandler(CNetMessage &msgin)
{
//	nldebug("Received message type %hu", (uint16)msgin.type());

	switch(msgin.type())
	{
	SWITCH_CASE(Chat);
	SWITCH_CASE(Error);
	SWITCH_CASE(Login);
	SWITCH_CASE(Logout);
	SWITCH_CASE(OpenClose);
	SWITCH_CASE(Update);
	SWITCH_CASE(UpdateOne);
	SWITCH_CASE(FullUpdate);
	SWITCH_CASE(UpdateElement);
	SWITCH_CASE(Ready);
	SWITCH_CASE(EverybodyReady);
	SWITCH_CASE(RequestDownload);
	SWITCH_CASE(RequestCRCKey);
	SWITCH_CASE(DisplayText);
	SWITCH_CASE(SessionState);
	SWITCH_CASE(StartSession);
	SWITCH_CASE(EndSession);
	SWITCH_CASE(EditMode);
	SWITCH_CASE(EnableElement);
	SWITCH_CASE(ExecLua);
	SWITCH_CASE(CollideWhenFly);
	SWITCH_CASE(TimeArrival);
	
	default: nlwarning("Received an unknown message type %hu", (uint16)msgin.type()); break;
	}
	
}
