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

#include "chat_task.h"
#include "hud_task.h"
#include "mtp_target.h"
#include "network_task.h"
#include "net_callbacks.h"
#include "entity_manager.h"
#include "level_manager.h"
#include "resource_manager.h"
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
	CMtpTarget::instance().error(msg);	
	//nlerror(msg.c_str());
}

static void cbLogin(CNetMessage &msgin)
{
	bool self;
	uint8 eid;
	string name;
	sint32 totalScore;
	CRGBA color;
	bool spec;

	msgin.serial(self, eid, name, totalScore, color, spec);
	nlinfo("Adding player %hu , list.size = %d", (uint16)eid,CEntityManager::instance().size());
	
	CEntityManager::instance().add(eid, name, totalScore, color, spec);

	if(self)
	{
		nlinfo("I'm the player number %hu, my name is '%s' and my score is %d", (uint16)eid, name.c_str(), totalScore);
		CEntityManager::instance()[eid].setIsLocal(true);
		
		try
		{
			string levelName;
			msgin.serial(levelName);
			float timeBeforeTimeout;
			msgin.serial(timeBeforeTimeout);
			if (!levelName.empty())
			{
				CMtpTarget::instance().startSession(0, timeBeforeTimeout, levelName);
			}
		}
		catch(Exception &)
		{
			nlwarning("can't load level because not in message (old server)");
		}
	}
	else
	{
		nlinfo("a new player number %hu name '%s' score %d", (uint16)eid, name.c_str(), totalScore);
	}
}

static void cbLogout(CNetMessage &msgin)
{
	uint8 eid;
	msgin.serial(eid);

	//CEntity *entity = mtpTarget::instance().World.getEntityById(id);
	//nlassert(entity!=0);
	//mtpTarget::instance().World.remove(entity);

	nlinfo("player number %hu leaves", (uint16)eid);

	// if it's my eid, it means that i have to disconnect because i was kicked out from the server
	if(CMtpTarget::instance().controler().getControledEntity()==eid)
	{
		nlerror("You have been kicked");
	}
	CEntityManager::instance().remove(eid);
}

static void cbOpenClose(CNetMessage &msgin)
{
	uint8 eid;
	msgin.serial(eid);
	
	//CEntity *entity = mtpTarget::instance().World.getEntityById(id);
	//nlassert(entity);
	CEntityManager::instance()[eid].swapOpenClose();
	//nlinfo("%d open/close",eid);
	//if(eid == mtpTarget::instance().Controler->getControledEntity())
	//	mtpTarget::instance().Controler->swapOpenClose();
	
	if(SessionFile)
		fprintf(SessionFile, "%hu OC\n", (uint16)eid);
}




static void cbUpdate(CNetMessage &msgin)
{
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
	nlassert(CEntityManager::instance().updateListId.size());
	std::list <uint8 >::iterator it;
	for(it=CEntityManager::instance().updateListId.begin();it!=CEntityManager::instance().updateListId.end();it++)
	{
		eid = *it;
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
		
		pos = CEntityManager::instance()[eid].LastSent2OthersPos + dpos;
		CEntityManager::instance()[eid].LastSent2OthersPos = pos;
		CEntityManager::instance()[eid].LastSent2MePos = pos;
		if(DisplayDebug)
			nlinfo("TCP update client %hu a %g %g %g ping %hu", (uint16)eid, pos.x, pos.y, pos.z);

		if(CEntityManager::instance().exist(eid))
		{
			CEntityManager::instance()[eid].interpolator().addKey(CEntityInterpolatorKey(CEntityState(pos,false),rsxTime));
			//CEntityManager::instance()[eid].ping(ping);
		}
		else
		{
			nlstop;
			nlwarning("Received a position of an unknown entity %hu", (uint16)eid);
		}
	}
}

static void cbUpdateOne(CNetMessage &msgin)
{
	float rsxTime;
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
		
		
		pos = CEntityManager::instance()[eid].LastSent2MePos + dpos;
		CEntityManager::instance()[eid].LastSent2MePos = pos;
		if(DisplayDebug)
			nlinfo("TCP updateOne client %hu a %g %g %g ping %hu", (uint16)eid, pos.x, pos.y, pos.z);
		
		if(CEntityManager::instance().exist(eid))
		{
			CEntityManager::instance()[eid].interpolator().addKey(CEntityInterpolatorKey(CEntityState(pos,false),rsxTime));
			//CEntityManager::instance()[eid].ping(ping);
		}
		else
		{
			nlstop;
			nlwarning("Received a position of an unknown entity %hu", (uint16)eid);
		}
	}
}


static void cbFullUpdate(CNetMessage &msgin)
{
	float rsxTime;
	uint8 eid;
	CVector pos;
	CVector dpos;
	uint16 ping;
	
	// reply to the update first (used for the ping)
	CNetMessage msgout(CNetMessage::Update);
	CNetworkTask::instance().send(msgout);
	
	//msgin.serial (rsxTime);
	

	//while(msgin.getPos() < (sint32)msgin.length())
	nlassert(CEntityManager::instance().updateListId.size());
	std::list <uint8 >::iterator it;
	for(it=CEntityManager::instance().updateListId.begin();it!=CEntityManager::instance().updateListId.end();it++)
	{
		eid = *it;
		//msgin.serial(eid);
		msgin.serial(pos, ping);
		//pos = CEntityManager::instance()[eid].LastSentPos + dpos;
		CEntityManager::instance()[eid].LastSent2MePos = pos;
		CEntityManager::instance()[eid].LastSent2OthersPos = pos;
		if(DisplayDebug)
			nlinfo("TCP updateFull client %hu a %g %g %g ping %hu", (uint16)eid, pos.x, pos.y, pos.z, ping);
		
		if(CEntityManager::instance().exist(eid))
		{
			CEntityManager::instance()[eid].interpolator().addKey(CEntityInterpolatorKey(CEntityState(pos,false),rsxTime));
			CEntityManager::instance()[eid].ping(ping);
		}
		else
		{
			nlstop;
			nlwarning("Received a position of an unknown entity %hu", (uint16)eid);
		}
	}
}


static void cbUpdateList(CNetMessage &msgin)
{
	uint8 eid;
	
	CEntityManager::instance().updateListId.clear();
	while(msgin.getPos() < (sint32)msgin.length())
	{
		msgin.serial(eid);
		CEntityManager::instance().updateListId.push_back(eid);
	}
}


static void cbEditMode(CNetMessage &msgin)
{
	uint8 editMode;
	msgin.serial(editMode);
	
}

static void cbUpdateElement(CNetMessage &msgin)
{
	uint8 elementType;
	msgin.serial(elementType);
	uint8 elementId;
	msgin.serial(elementId);
	uint8 selectedBy;
	msgin.serial(selectedBy);
	CVector pos;
	msgin.serial(pos);
	CVector eulerRot;
	msgin.serial(eulerRot);

	if(elementType==CEditableElementCommon::Module)
		CLevelManager::instance().currentLevel().updateModule(elementId,pos,eulerRot,selectedBy);
}
	
static void cbReady(CNetMessage &msgin)
{
	// called when everybody is ready, can start the countdown
	nlinfo ("everybody is ready");
	mtpTarget::instance().everybodyReady();
}

static void cbShowElement(CNetMessage &msgin)
{
	uint8 elementId;
	msgin.serial(elementId);
	bool show;
	msgin.serial(show);
}

static void cbRequestCRCKey(CNetMessage &msgin)
{
	string fn;
	msgin.serial(fn);
	CHashKey hashKey;
	msgin.serial(hashKey);
	CResourceManager::instance().receivedCRC(fn);
}

static void cbRequestDownload(CNetMessage &msgin)
{
	string res;
	vector<uint8> buf;
	bool eof;
	uint32 fileSize;
	
	msgin.serial(res);
	nlinfo("cbRequestDownload res = %s",res.c_str());
	
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
	float x,y;
	float s;
	CRGBA col;
	double duration;
	string message;
	msgin.serial(x);
	msgin.serial(y);
	msgin.serial(s);
	msgin.serial(message);
	msgin.serial(col);
	msgin.serial(duration);

	CHudMessage m = CHudMessage(x,y,s,message,col,duration);
	CHudTask::instance().messages.push_back(m);
	nlinfo("display text(during %f sec) : %s",duration,message.c_str());
	
}

static void cbStartSession(CNetMessage &msgin)
{
	float timebeforestart, timeout;
	msgin.serial(timebeforestart, timeout);

	string levelName, str1, str2;
	msgin.serial(levelName, str1, str2);

	vector<uint8> ranks;
	vector<uint8> eids;
	msgin.serialCont(ranks);
	msgin.serialCont(eids);

	nlassert(ranks.size()==eids.size());

	for(uint32 i=0;i<eids.size();i++)
		CEntityManager::instance()[eids[i]].rank(ranks[i]);
	
	nlinfo ("cbStartSession");
	CMtpTarget::instance().startSession(timebeforestart / 1000.0f, timeout / 1000.0f, levelName);

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
	nlinfo("Server session state is now '%s'", sn.c_str());
}

static void cbEndSession(CNetMessage &msgin)
{
	while(msgin.getPos() < (sint32)msgin.length())
	{
		uint8 eid;
		sint32 currentScore, totalScore;
		
		msgin.serial(eid, currentScore, totalScore);
		
		//CEntity *entity = mtpTarget::instance().World.getEntityById(id);
		//nlassert(entity!=0);
		//entity->setScore(currentScore, totalScore);
		CEntityManager::instance()[eid].currentScore(currentScore);
		CEntityManager::instance()[eid].totalScore(totalScore);
	}
	mtpTarget::instance().endSession();
	
	if(SessionFile)
	{
		fclose (SessionFile);
		SessionFile = 0;
	}
}
	
//
// Callback handler
//

#define SWITCH_CASE(_n) case CNetMessage::_n: cb##_n(msgin); break

void netCallbacksHandler(CNetMessage &msgin)
{
//	nlinfo ("Received message type %hu", (uint16)msgin.type());

	switch(msgin.type())
	{
	SWITCH_CASE(Chat);
	SWITCH_CASE(Error);
	SWITCH_CASE(Login);
	SWITCH_CASE(Logout);
	SWITCH_CASE(OpenClose);
	SWITCH_CASE(Update);
	SWITCH_CASE(UpdateOne);
	SWITCH_CASE(UpdateList);
	SWITCH_CASE(FullUpdate);
	SWITCH_CASE(UpdateElement);
	SWITCH_CASE(Ready);
	SWITCH_CASE(RequestDownload);
	SWITCH_CASE(RequestCRCKey);
	SWITCH_CASE(DisplayText);
	SWITCH_CASE(SessionState);
	SWITCH_CASE(StartSession);
	SWITCH_CASE(EndSession);
	SWITCH_CASE(EditMode);
	SWITCH_CASE(ShowElement);
	
	default: nlinfo("Received an unknown message type %hu", (uint16)msgin.type()); break;
	}
	
}
