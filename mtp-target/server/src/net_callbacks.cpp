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

#include <errno.h>

#include <nel/misc/path.h>
#include <nel/misc/sha1.h>

#include "network.h"
#include "welcome.h"
#include "net_callbacks.h"
#include "entity_manager.h"
#include "level_manager.h"
#include "session_manager.h"
#include "zlib.h"


//
// Namespaces
//

using namespace std;
using namespace NLNET;
using namespace NLMISC;


//
// Functions
//

static void cbChat(CClient *c, CNetMessage &msgin)
{
	string msg;
	msgin.serial(msg);
	CNetwork::instance().sendChat("<"+c->name()+"> "+msg);
}

static void cbCommand(CClient *c, CNetMessage &msgin)
{
	string cmd;
	msgin.serial(cmd);

	if(c->isAdmin())
	{
		ICommand::execute(cmd, *InfoLog);
		CNetwork::instance().sendChat(c->name()+" executed: "+cmd);
	}
	else
	{
		CNetwork::instance().sendChat(c->name()+" tried to execute the admin command "+cmd);
	}
}

static void cbForce(CClient *c, CNetMessage &msgin)
{
	// new client update
	CVector force;
	msgin.serial(force);
	c->setForce(force);
}
	
static void cbLogin(CClient *c, CNetMessage &msgin)
{
	string cookie;
	string login, password;
	sint32 score;
	CRGBA color;
	uint32 networkVersion = MTPT_NETWORK_VERSION;
	
	nlinfo("New client login");

	msgin.serial(cookie, login, password, color,networkVersion);

	if(networkVersion!=MTPT_NETWORK_VERSION)
	{
		string reason = toString("'%s' login failed: bad client version(%d)! Get latest one on Mtp Target web site", login.c_str(), networkVersion);
		CNetMessage msgout(CNetMessage::Error);
		msgout.serial(reason);
		CNetwork::instance().send(c->id(), msgout);
		return;
	}
	
	
	string res;
	if(cookie.empty())
	{
		// check in normal way
		res = CEntityManager::instance().check(login, password, false, score);
	}
	else
	{
		// already checks in LS
		nlinfo("Receive a client with cookie %s", cookie.c_str());
		login = getUserFromCookie(cookie);
		if(login.empty())
		{
			res = "Bad cookie identification";
		}
		else
		{
			res = CEntityManager::instance().check(login, password, true, score);
		}
	}
	
	if(res.empty())
	{
		c->name(login);
		c->Cookie = cookie;
		c->Score = score;
		c->Color = color;
		CEntityManager::instance().login(c);
	}
	else
	{
		nlwarning("Login client '%s' failed: %s", login.c_str(), res.c_str());
		CNetMessage msgout(CNetMessage::Error);
		msgout.serial(res);
		CNetwork::instance().send(c->id(), msgout);
	}
}

static void cbOpenClose(CClient *c, CNetMessage &msgin)
{
	TTime currentTime = CTime::getLocalTime();
	
	if(CSessionManager::instance().endTime() != 0 || CSessionManager::instance().startTime() == 0 || currentTime < CSessionManager::instance().startTime())
	{
		// ignoring client update if not in session
		nlinfo("Can't open or close client '%s' because there's no session", c->name().c_str());
		return;
	}
	
	if (c->openClose())
	{
		CNetMessage msgout(CNetMessage::OpenClose);
		uint8 eid = c->id();
		msgout.serial(eid);
		CNetwork::instance().send(msgout);
	}
}

static void cbEditMode(CClient *c, CNetMessage &msgin)
{
	uint8 editMode;
	msgin.serial(editMode);
	if(c->isAdmin())
		CSessionManager::instance().editMode(editMode);
}

static void cbUpdateElement(CClient *c, CNetMessage &msgin)
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

	if(c->isAdmin() && CSessionManager::instance().currentStateName()=="Running")
	{
		CNetMessage msgout(CNetMessage::UpdateElement);
		msgout.serial(elementType);
		msgout.serial(elementId);
		msgout.serial(selectedBy);
		msgout.serial(pos);
		msgout.serial(eulerRot);
		CNetwork::instance().sendAllExcept(c->id(),msgout);

		if(CSessionManager::instance().editMode()==0)
			CSessionManager::instance().editMode(1);
		if(elementType==CEditableElementCommon::Module)
			CLevelManager::instance().currentLevel().updateModule(elementId,pos,eulerRot);
		else if(elementType==CEditableElementCommon::StartPosition)
			CLevelManager::instance().currentLevel().updateStartPoint(elementId,pos,eulerRot);
	}
	
}

static void cbReady(CClient *c, CNetMessage &msgin)
{
	c->Ready = true;
	nlinfo("Client '%s' is ready to play", c->name().c_str());
	CNetMessage msgout(CNetMessage::Ready);
	uint8 eid = c->id();
	msgout.serial(eid);
	CNetwork::instance().send(msgout);
}

static void cbRequestCRCKey(CClient *c, CNetMessage &msgin)
{
	CNetMessage msgout(CNetMessage::RequestCRCKey);

	string fn;
	msgin.serial(fn);
	CHashKey hashKey;
	msgin.serial(hashKey);

	string fns = CFile::getFilename(fn);
	string path = CPath::lookup(fns, false, false);

	CHashKey serverHashKey;
	string serverfn;
	if(path.empty())
		serverfn = "";
	else
	{
		serverHashKey = getSHA1(path);
		if(serverHashKey==hashKey)
			serverfn = "";
		else
			serverfn = CFile::getFilename(path);
	}

	msgout.serial(serverfn);
	msgout.serial(serverHashKey);
	CNetwork::instance().send(c->id(),msgout);	
}
	
static void cbRequestDownload(CClient *c, CNetMessage &msgin)
{
	CNetMessage msgout(CNetMessage::RequestDownload);

	string fn;
	msgin.serial(fn);
	string fns = CFile::getFilename(fn);
	string path = CPath::lookup(fns, false, false);
	string packedPath = CPath::lookup(CFile::getFilename(path)+".gz", false, false);
	if(path.empty())
	{
		nlwarning("Client '%s' wants an unknown file '%s'", c->name().c_str(), fn.c_str());
		string res = toString("ERROR:File '%s' is not found", fn.c_str());
		msgout.serial(res);
		CNetwork::instance().send(c->id(),msgout);
		return;
	}

	vector<uint8> buf;
	buf.resize(8000);
	uint8 *ptr = &(*(buf.begin()));
	
	bool packedFileUpToDate = CFile::getFileModificationDate(packedPath) >= CFile::getFileModificationDate(path);


	if(!packedFileUpToDate)
	{
		CHashKey serverHashKey = getSHA1(path);
		CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
		for(CEntityManager::EntityConstIt it = acces.value().begin(); it != acces.value().end(); it++)
		{
			nlassert(*it);
			if((*it)->type() != CEntity::Client || (*it)==c)
				continue;
			
			CClient *e = (CClient *)(*it);
			uint8 nid = e->id();
			CNetMessage msgout(CNetMessage::RequestCRCKey);
			msgout.serial(fn);
			msgout.serial(serverHashKey);
			CNetwork::instance().send(nid, msgout);
		}
	}

	if(packedPath.empty() || !packedFileUpToDate )
	{
		if(packedPath.empty())
			packedPath = path + ".gz";
		FILE *fp = fopen(path.c_str(), "rb");
		gzFile gzfp = gzopen(packedPath.c_str(), "wb");
		while (!feof(fp)) 
		{
			uint32 res = fread(ptr, 1, 8000, fp);
			gzwrite(gzfp,ptr,res);
		}
		fclose(fp);
		gzclose(gzfp);
		
	}

	uint32 part = 0;
	msgin.serial(part);
	nlinfo("Client '%s' wants file '%s' part %d", c->name().c_str(), fns.c_str(), part);

	FILE *fp = fopen(packedPath.c_str(), "rb");
	if(!fp)
	{
		nlwarning("Can't open file '%s'", packedPath.c_str());
		string res = toString("ERROR:Can't open file '%s'", packedPath.c_str());
		msgout.serial(res);
		CNetwork::instance().send(c->id(),msgout);
		return;
	}

	if (fseek(fp, part, SEEK_SET) != 0)
	{
		nlwarning("Can't seek file '%s' to part %d", packedPath.c_str(), part);
		string res = toString("ERROR:Can't seek file '%s' part %d", packedPath.c_str(), part);
		msgout.serial(res);
		CNetwork::instance().send(c->id(),msgout);
		fclose(fp);
		return;
	}

	uint32 fileSize = CFile::getFileSize(fp);

	uint32 res = fread(ptr, 1, 8000, fp);
	if(res != 8000)
	{
		if(feof(fp))
		{
			// we are at the end of the file, resize to the good size
			buf.resize(res);
		}
		else
		{
			// problem during reading
			nlwarning("Failed to read file '%s' to part %d: %s", packedPath.c_str(), part, strerror(errno));
			string res = toString("ERROR:Failed to read file '%s' part %d: %s", packedPath.c_str(), part, strerror(errno));
			msgout.serial(res);
			CNetwork::instance().send(c->id(),msgout);
			fclose(fp);
			return;
		}
	}

	// empty string means no error
	string str = toString("FILE:%s",CFile::getFilename(path).c_str());
	msgout.serial(str);
	msgout.serial(fileSize);
	msgout.serialCont(buf);

	bool eof = feof(fp)!=0;
	nlinfo("the file eof is : %d size = %d", eof, res);
	msgout.serial(eof);

	CNetwork::instance().send(c->id(),msgout);
	fclose(fp);
}

static void cbUpdate(CClient *c, CNetMessage &msgin)
{
	TTime CurrentTime = CTime::getLocalTime();

	if(c->LastSentPing.empty())
	{
		nlwarning ("Received an ack of an update without info sending from %hu '%s'", (uint16)c->id(), c->name().c_str());
		return;
	}

	TTime OldTime = c->LastSentPing.front();
	if(CurrentTime < OldTime)
	{
		nlwarning ("Received an ack of an update with bad sync from %hu '%s'", (uint16)c->id(), c->name().c_str());
		return;
	}

	c->Ping.addValue((uint16)(CurrentTime - OldTime));
//	nlinfo("Ping for '%s' is %d (%"NL_I64"d)", c->name().c_str(), c->Ping.getSmoothValue(), CurrentTime - OldTime);
	c->LastSentPing.pop();
}




//
// Callback handler
//

#define SWITCH_CASE(_n) case CNetMessage::_n: cb##_n(c, msgin); break

void netCallbacksHandler(CClient *c, CNetMessage &msgin)
{
//	nlinfo ("Received message type %hu from %hu '%s'", (uint16)msgin.type(), c->id(), c->name().c_str());

	nlassert(c);
	nlassert(c->type() == CEntity::Client);

	switch(msgin.type())
	{
	SWITCH_CASE(Chat);
	SWITCH_CASE(Command);
	SWITCH_CASE(EditMode);
	SWITCH_CASE(Force);
	SWITCH_CASE(Login);
	SWITCH_CASE(OpenClose);
	SWITCH_CASE(Ready);
	SWITCH_CASE(RequestDownload);
	SWITCH_CASE(RequestCRCKey);
	SWITCH_CASE(Update);
	SWITCH_CASE(UpdateElement);
	
	default: nlinfo("Received an unknown message type %hu", (uint16)msgin.type()); break;
	}

}


//
// Commands
//
