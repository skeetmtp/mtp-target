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

#ifdef NL_OS_WINDOWS
// these defines is for IsDebuggerPresent(). it'll not compile on windows 95
// just comment this and the IsDebuggerPresent to compile on windows 95
#	define _WIN32_WINDOWS	0x0410
#	define WINVER			0x0400
#	include <windows.h>
#	undef min
#	undef max
HINSTANCE ghInstance = 0;
#endif

#include <string>

#include "mtp_target.h"
#include "main.h"
#include "task_manager.h"
#include "network_task.h"
	

//
// Namespaces
//
	
using namespace std;
using namespace NLMISC;
	

//
// Variables
//

bool DisplayDebug = false;
bool FollowEntity = false;
string ReplayFile;

uint TaskManagerThreadId = 0;
uint NetworkThreadId = 0;


#ifdef NL_OS_WINDOWS

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	ghInstance = hInstance;
	// Look the command line to see if we have a cookie and a addr

	nlinfo ("args: '%s'", lpCmdLine);

	string cmd = lpCmdLine;

	if (cmd.find ("\"") != string::npos)
	{
		// it s a replay, remove ""
		ReplayFile = cmd.substr(1, cmd.size()-2);
	}
	else
	{
		ReplayFile = cmd;
	}

	if(!IsDebuggerPresent() && !ReplayFile.empty())
	{
		char exePath  [512] = "";
		DWORD success = GetModuleFileName (NULL, exePath, 512);
		bool found = false;
		if(success)
		{
			int l = strlen(exePath);
			for(int i=l;i>=0;i--)
			{
				if(exePath[i]=='\\')
				{
					exePath[i] = '\0';
					found = true;
					break;
				}
			}
		}
		if(found)
			SetCurrentDirectory(exePath);
	}
	
#else

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		ReplayFile = argv[1];
	}

#endif

	TaskManagerThreadId = getThreadId();
	// add the main task
	CTaskManager::instance().add(CMtpTarget::instance(), 70);

	// start the execution
	CTaskManager::instance().execute();

	// return
	return EXIT_SUCCESS;
}


CSynchronized<PauseFlags> pauseAllFlags("pauseAll");

bool pauseAllThread()
{
	{
		CSynchronized<PauseFlags>::CAccessor acces(&pauseAllFlags);
		if(acces.value().pauseCount>0)
			return false;
		acces.value().pauseCount = 1;
	}
	bool allOk = true;
	allOk = allOk && pauseTaskManager(false);
	if(!allOk)
	{
		CSynchronized<PauseFlags>::CAccessor acces(&pauseAllFlags);
		acces.value().pauseCount = 0;
		return false;		
	}
	allOk = allOk && true;
	if(!allOk)
	{
		resumeTaskManager();
		CSynchronized<PauseFlags>::CAccessor acces(&pauseAllFlags);
		acces.value().pauseCount = 0;
		return false;
	}

	while(true)
	{
		int threadPausedCount = 0;
		if(isTaskManagerPaused()) threadPausedCount++;
		if(true) threadPausedCount++;
		if(threadPausedCount==1) break;
		nlSleep(10);
	}

	return true;
}

void resumeAllThread()
{
	resumeTaskManager();
	{
		CSynchronized<PauseFlags>::CAccessor acces(&pauseAllFlags);
		acces.value().pauseCount = 0;
	}
}
