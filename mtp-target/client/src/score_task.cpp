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
#include "time_task.h"
#include "game_task.h"
#include "chat_task.h"
#include "score_task.h"
#include "network_task.h"
#include "font_manager.h"
#include "task_manager.h"
#include "entity_manager.h"
#include "background_task.h"
#include "resource_manager.h"
#include "mtp_target.h"


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
	
void CScoreTask::init()
{
}

void CScoreTask::update()
{
}

void CScoreTask::render()
{
	if(!C3DTask::instance().kbDown(KeyTAB) && CMtpTarget::instance().State != CMtpTarget::eEndSession )
		return;

	C3DTask::instance().driver().setFrustum(CFrustum(0, (float)C3DTask::instance().screenWidth(), 0, (float)C3DTask::instance().screenHeight(), -1, 1, false));
	C3DTask::instance().driver().clearZBuffer();

	C3DTask::instance().driver().drawQuad(0.0f, 0.0f, C3DTask::instance().screenWidth() , C3DTask::instance().screenHeight(), CRGBA (0, 0, 0, 100));
	
	// display all players score
	float x1 = 15.0f;
	float x2 = 250.0f;
	float x3 = 500.0f;
	float x4 = 680.0f;
	float y = 100.0f;
	CFontManager::instance().printf(CRGBA(245, 238, 141), x1, y, 1, "name");
	CFontManager::instance().printf(CRGBA(245, 238, 141), x2, y, 1, "score");
	CFontManager::instance().printf(CRGBA(245, 238, 141), x3, y, 1, "total");
	CFontManager::instance().printf(CRGBA(245, 238, 141), x4, y, 1, "ping");
	y += CFontManager::instance().fontHeight()+10;
	
	vector<uint8> eids;
	CEntityManager::instance().getEIdSortedByScore(eids);
	
	for(uint i = 0; i < eids.size(); i++, y += CFontManager::instance().fontHeight())
	{
		sint32 curScore = CEntityManager::instance()[eids[i]].currentScore();
		if(curScore<0)
			printf("ok");
		CFontManager::instance().printf(CEntityManager::instance()[eids[i]].color(), x1, y, 1, "%s%s", CEntityManager::instance()[eids[i]].name().c_str(), (CEntityManager::instance()[eids[i]].spectator()?" :spec:":""));
		CFontManager::instance().printf(CEntityManager::instance()[eids[i]].color(), x2, y, 1, "%d", CEntityManager::instance()[eids[i]].currentScore());
		CFontManager::instance().printf(CEntityManager::instance()[eids[i]].color(), x3, y, 1, "%d", CEntityManager::instance()[eids[i]].totalScore());
		CFontManager::instance().printf(CEntityManager::instance()[eids[i]].color(), x4, y, 1, "%u", CEntityManager::instance()[eids[i]].ping());
	}
}

void CScoreTask::release()
{
}
