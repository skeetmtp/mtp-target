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

#include <nel/misc/file.h>
#include <nel/misc/path.h>

#include <3d/mesh.h>
#include <3d/shape.h>
#include <3d/texture.h>
#include <3d/material.h>
#include <3d/register_3d.h>
#include <3d/texture_file.h>
#include <3d/texture_multi_file.h>

#include "zlib.h"

#include "main.h"
#include "3d_task.h"
#include "interface.h"
#include "time_task.h"
#include "chat_task.h"
#include "gui_task.h"
#include "mtp_target.h"
#include "task_manager.h"
#include "swap_3d_task.h"
#include "network_task.h"
#include "font_manager.h"
#include "background_task.h"
#include "config_file_task.h"
#include "resource_manager.h"


//
// Namespaces
//

using namespace std;
using namespace NL3D;
using namespace NLMISC;


//
// Functions
//


void CResourceManager::init()
{
	CPath::remapExtension("dds", "tga", true);
	CacheDirectory = CPath::standardizePath(CConfigFileTask::instance().configFile().getVar("CacheDirectory").asString());

	if(!CFile::isDirectory(CacheDirectory))
	{
		if(!CFile::createDirectory(CacheDirectory))
		{
			nlerror("Couldn't create \"%s\" directory", CacheDirectory.c_str());
		}
	}

	CConfigFile::CVar &v = CConfigFileTask::instance().configFile().getVar("Path");
	for (int i = 0; i < v.size(); i++)
		CPath::addSearchPath (v.asString(i), true, false);

	CRCCheckTimes.clear();
}

void CResourceManager::receivedCRC(string &fn)
{
	CRCReceived = true;
	CRCUpToDate = fn.empty();
	if(!CRCUpToDate)
	{
		filename2LastCRCCheckTime::iterator it = CRCCheckTimes.find(CFile::getFilename(fn));
		if(it!=CRCCheckTimes.end())
			CRCCheckTimes.erase(it);		
	}
}


void CResourceManager::receivedBlock(const string &res, const vector<uint8> &buf, bool eof, uint32 fileSize, bool receivedError)
{
	Reason = res;
	Buffer = buf;
	Eof = eof;
	Received = true;
	FileSize = fileSize;
	
	ReceivedError = receivedError;

	ReceivedFilename ="none";
	
	if(!ReceivedError)
	{
		nlinfo("Receive an answer of a download request block size %d eof %d", buf.size(), eof);
	}
	else
	{
		nlwarning("Error during download request '%s'", res.substr(strlen("ERROR:")).c_str());
		return;
	}
	ReceivedFilename = res.substr(strlen("FILE:"));
}

void CResourceManager::loadChildren(const std::string &filename)
{
	string ext = CFile::getExtension(filename);
	if(ext == "shape")
	{
		// need to get texture inside the shape
		NL3D::registerSerial3d();

		CShapeStream ss;
		NLMISC::CIFile i(CPath::lookup(filename, false).c_str());
		i.serial(ss);
		i.close();

		CMesh *m = (CMesh*)ss.getShapePointer();
		uint nbm = m->getNbMaterial();
		for(uint i = 0; i < nbm; i++)
		{
			CMaterial &mat = m->getMaterial(i);
			for(uint j = 0; j < IDRV_MAT_MAXTEXTURES; j++)
			{
				ITexture *t = mat.getTexture(j);
				if(t)
				{
					CTextureFile *tf = dynamic_cast<CTextureFile *>(t);
					if(tf)
					{
						get(tf->getFileName());
					}
					else
					{
						CTextureMultiFile *tmf = dynamic_cast<CTextureMultiFile *>(t);
						if(tmf)
						{
							for(uint t = 0; t < tmf->getNumFileName(); t++)
								get(tmf->getFileName(t));
						}
					}
				}
			}
		}
	}
	//TODO else if(ext == "ps")
}


bool CResourceManager::waitNetworkMessage(bool stopFlag,bool &received, bool displayBackground)
{
		C3DTask::instance().update();
		CTimeTask::instance().update();
		//CBackgroundTask::instance().update();
		CChatTask::instance().update();
		CGuiTask::instance().update();
		
		if (C3DTask::instance().kbPressed(KeyESCAPE) || !C3DTask::instance().driver().isActive() || !CNetworkTask::instance().connected())
		{
			exit(1);
			received = false;
			return false;
		}
		
		//			mt3dUpdate();
		//			mtpTarget::instance().updateTime ();
		//			mtpTarget::instance().Interface2d.updateChat();
		
		C3DTask::instance().clear();
		//			mt3dClear ();
		
		// ace todo put this in task	
		
		//			mtpTarget::instance().Interface2d.displayBackground();
		//			mtpTarget::instance().Interface2d.displayChat(true);
		
		C3DTask::instance().render();
		if(displayBackground)
			CBackgroundTask::instance().render();
		CChatTask::instance().render();
		CGuiTask::instance().render();
		
		
		//CFontManager::instance().littlePrintf(3.0f, 29.0f, str.c_str());
		
		CSwap3DTask::instance().render();
		
		//			mt3dSwap ();
		
		nlSleep(1);

		if(!stopFlag)
			received = true;
		return !stopFlag;
}


void CResourceManager::refresh(const string &filename)
{
	string fn = CFile::getFilename(filename);
	filename2LastCRCCheckTime::iterator it = CRCCheckTimes.find(fn);
	while(it!=CRCCheckTimes.end())
	{
		CRCCheckTimes.erase(it);		
		it = CRCCheckTimes.find(fn);
	}
}

string CResourceManager::get(const string &filename)
{
	bool ok;
	return get(filename, ok);
}

string CResourceManager::get(const string &filename, bool &ok)
{
	string unk;
	ok = false;

	if(filename.empty())
		return unk;

	string ext = CFile::getExtension(filename);
	if(ext == "shape") unk = CPath::lookup("unknown.shape", false);
	else if(ext == "lua") unk = CPath::lookup("unknown.lua", false);
	else if(ext == "tga") unk = CPath::lookup("unknown.tga", false);
	else if(ext == "dds") unk = CPath::lookup("unknown.tga", false);
	else if(ext == "png") unk = CPath::lookup("unknown.tga", false);
	else if(ext == "ps") unk = CPath::lookup("unknown.ps", false);
	else if(ext == "wav") unk = CPath::lookup("unkown.wav", false);
	else 
	{
		nlwarning("Extension not managed : %s(%s)",ext.c_str(),filename.c_str());
	}

	
	string fn = CFile::getFilename(filename);

	string path = CPath::lookup(fn, false, false);
	string fns = CFile::getFilename(path);

	double currentTime = CTimeTask::instance().time();
	float updatePercent = 0;
	guiSPG<CGuiFrame> mainFrame = NULL;

	//nlinfo("CResourceManager get(%s)",filename.c_str());

	if(!path.empty())
	{
		//nlinfo("!path.empty()");
		CRCUpToDate = true;
		if(CNetworkTask::instance().connected())
		{
			//nlinfo("connected");
			double lastCheckTime = 0;
			filename2LastCRCCheckTime::iterator it = CRCCheckTimes.find(fn);
			if(it!=CRCCheckTimes.end())
				lastCheckTime = (*it).second;
			if( it==CRCCheckTimes.end() && CConfigFileTask::instance().configFile().getVar("CRCCheck").asInt())
			{
				guiSPG<CGuiXml> xml = CGuiXmlManager::instance().Load("checking.xml");
				mainFrame = (CGuiFrame *)xml->get("checkingFrame");
				guiSPG<CGuiText>  checkingMessage = (CGuiText *)xml->get("checkingMessage");
				checkingMessage->text = "Please wait while checking : ";
				guiSPG<CGuiText>  checkingFilename = (CGuiText *)xml->get("checkingFilename");
				checkingFilename->text = fn;
				CGuiObjectManager::instance().objects.push_back(mainFrame);

				CHashKey hashKey = getSHA1(path);
				CNetMessage msgout(CNetMessage::RequestCRCKey);
				msgout.serial(fns);
				msgout.serial(hashKey);
				CNetworkTask::instance().send(msgout);
				
				CRCReceived = false;
				bool messageReceived;

				uint tid = getThreadId();
				nlassert(tid==TaskManagerThreadId || tid==NetworkThreadId);
				if(tid==TaskManagerThreadId)
				{
					while(waitNetworkMessage(CRCReceived,messageReceived))
						checkTaskManagerPaused();
				}
				else
					while(waitNetworkMessage(CRCReceived,messageReceived));
					
				if(!messageReceived)
					return unk;
				if(it!=CRCCheckTimes.end())
					CRCCheckTimes.erase(it);
				CRCCheckTimes.insert(filename2LastCRCCheckTime::value_type(fn,currentTime));
			}
		}

		if(CRCUpToDate)
		{
			//nlinfo("CRCUpToDate");
			loadChildren(path);
			// we already have the file on local
			ok = true;
			return path;
		}
	}

	if(!CNetworkTask::instance().connected())
	{
		//nlinfo("!connected");
		// we can't download the file
		return unk;
	}
	
	guiSPG<CGuiXml> xml = CGuiXmlManager::instance().Load("updating.xml");
	mainFrame = (CGuiFrame *)xml->get("updatingFrame");
	guiSPG<CGuiText>  updatingMessage = (CGuiText *)xml->get("updatingMessage");
	updatingMessage->text = "Please wait while dowloading : ";
	guiSPG<CGuiText>  updatingFilename = (CGuiText *)xml->get("updatingFilename");
	updatingFilename->text = fn;
	guiSPG<CGuiProgressBar>  updatingProgressBar = (CGuiProgressBar *)xml->get("updatingProgressBar");
	updatingProgressBar->ptrValue(&updatePercent);
	CGuiObjectManager::instance().objects.push_back(mainFrame);		
	
	
	Eof = false;
	uint32 part = 0;
	FileSize = 0;
	
	string dlfn = CacheDirectory + fn;
	string packedfn = dlfn + ".gz";

	if(updatingMessage)
		updatingMessage->text = "Please wait while dowloading : ";
	
	while(!Eof)
	{
		Received = false;
		CNetMessage msgout(CNetMessage::RequestDownload);
		msgout.serial(fn);
		msgout.serial(part);
		CNetworkTask::instance().send(msgout);

		string str = toString("Please wait while downloading '%s' part %d", fn.c_str(), part);

		bool messageReceived;
		while(waitNetworkMessage(Received,messageReceived));
		if(!messageReceived)
		{
			CFile::deleteFile(packedfn);
			exit(1);
		}

		if(ReceivedError)
		{
			CFile::deleteFile(packedfn);
			return unk;
		}

		nlinfo("Receive an answer of a download request block size %s %d eof %d", Reason.c_str(), Buffer.size(), Eof);

		FILE *fp = fopen(packedfn.c_str(), "ab");
		if(!fp)
		{
			nlwarning("Couldn't open file '%s'", packedfn.c_str());
			return unk;
		}
		if(fwrite(&*Buffer.begin(), 1, Buffer.size(), fp) != Buffer.size())
		{
			nlwarning("Couldn't write file '%s'", packedfn.c_str());
			fclose(fp);
			return unk;
		}
		fclose(fp);

		part += Buffer.size();
		if(FileSize)
			updatePercent = ((float)part) / FileSize;
	}

	string destfn = CacheDirectory + ReceivedFilename;
	nlinfo("Received the whole file '%s'", destfn.c_str());

	vector<uint8> buf;
	buf.resize(8000);
	uint8 *ptr = &(*(buf.begin()));
	
	{
		FILE *fp = fopen(destfn.c_str(), "wb");
		gzFile gzfp = gzopen(packedfn.c_str(), "rb");
		while (!gzeof(gzfp)) 
		{
			uint32 res = gzread(gzfp, ptr, 8000);
			fwrite(ptr,1,res,fp);
		}
		fclose(fp);
		gzclose(gzfp);
		CFile::deleteFile(packedfn);		
	}
	
	CPath::addSearchFile(destfn);

	loadChildren(destfn);
	CRCCheckTimes.insert(filename2LastCRCCheckTime::value_type(fn,currentTime));
				
	// need to download the file
	ok = true;
	return destfn;
}
