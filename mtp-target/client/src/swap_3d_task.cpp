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
#include "config_file_task.h"
#include "swap_3d_task.h"


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

void CSwap3DTask::init()
{
	SleepTime = CConfigFileTask::instance().configFile().getVar("Sleep").asInt();
}

void CSwap3DTask::render()
{
	C3DTask::instance().driver().swapBuffers ();
	
	if(C3DTask::instance().kbPressed(KeyF2))
	{
		CBitmap btm;
		C3DTask::instance().driver().getBuffer (btm);
		btm.flipV();
		string filename = CFile::findNewFile ("screenshot.jpg");
		COFile fs (filename);
		btm.writeJPG(fs);
		nlinfo("Screenshot '%s' saved", filename.c_str());
	}
	
	if (SleepTime >= 0)
		nlSleep (SleepTime);
}
