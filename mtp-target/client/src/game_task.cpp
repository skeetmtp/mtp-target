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
#include "hud_task.h"
#include "time_task.h"
#include "game_task.h"
#include "chat_task.h"
#include "intro_task.h"
#include "score_task.h"
#include "mtp_target.h"
#include "font_manager.h"
#include "task_manager.h"
#include "entity_manager.h"
#include "background_task.h"
#include "level_manager.h"
#include "config_file_task.h"
#include "resource_manager2.h"


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
	
void CGameTask::init()
{
	CTaskManager::instance().add(CLevelManager::instance(), 60);
	CTaskManager::instance().add(CHudTask::instance(), 1020);
	CTaskManager::instance().add(CScoreTask::instance(), 1030);
	CTaskManager::instance().add(CChatTask::instance(), 1040);

	if(CConfigFileTask::instance().configFile().getVar("CaptureMouse").asInt() == 1)
		C3DTask::instance().captureCursor(true);
}

void CGameTask::update()
{
}

void CGameTask::render()
{
	C3DTask::instance().clear();
}

void CGameTask::release()
{
}

void CGameTask::stop()
{
	ITask::stop();
	CLevelManager::instance().stop();
	CHudTask::instance().stop();
	CScoreTask::instance().stop();
	CChatTask::instance().stop();
}

