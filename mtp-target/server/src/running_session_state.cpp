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

#include "bot.h"
#include "main.h"
#include "network.h"
#include "variables.h"
#include "lua_engine.h"
#include "level_manager.h"
#include "entity_manager.h"
#include "session_manager.h"
#include "ending_session_state.h"
#include "running_session_state.h"

//
// Namespaces
//

using namespace std;
using namespace NLNET;
using namespace NLMISC;


//
// Functions
//

void CRunningSessionState::update()
{
	TTime currentTime = CTime::getLocalTime();
	if(CSessionManager::instance().forceEnding() || ( currentTime > CSessionManager::instance().startTime()+(TTime)TimeBeforeCheck && CSessionManager::instance().editMode()==0 ) )
	{
	vector<string> chat;

	{
		CEntityManager::EntityConstIt it;

		bool everybodyStopped = true;

		if(currentTime > CSessionManager::instance().startTime()+(TTime)CLevelManager::instance().timeTimeout())
		{
			nlinfo("Session time out!!!");
		}
		else
		{
			for(it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
			{
				CEntity *c = *it;
				if(c->InGame)
				{
					float val = c->meanVelocity();
					
					if(val>MinVelBeforeEnd) //si le client bouge
					{
						everybodyStopped = false;
					}
					else //si il est arrete
					{
						if(c->ArrivalTime == 0)
						{
							c->ArrivalTime =(float)(currentTime - CSessionManager::instance().startTime())/1000.0f;
							nlinfo("*** ARRIVAL TIME '%s' %f",c->name().c_str(),c->ArrivalTime);

							CNetMessage msgout(CNetMessage::TimeArrival);
							uint8 eid = c->id();
							msgout.serial(eid);
							msgout.serial(c->ArrivalTime);
							CNetwork::instance().send(msgout);
						}
					}
				}
			}
		}

		if(everybodyStopped)
			nlinfo("everybodyStopped");

		if(CSessionManager::instance().forceEnding() || everybodyStopped)
		{

			CLuaEngine::instance().levelEndSession();
			CEntityManager::instance().checkAfkClient();//kick away client

			CEntityManager::EntityConstIt  bestit1 = CEntityManager::instance().entities().end();
			CEntityManager::EntityConstIt  bestit2 = CEntityManager::instance().entities().end();
			CEntityManager::EntityConstIt  bestit3 = CEntityManager::instance().entities().end();
			for(it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
			{
				if((*it)->CurrentScore > 0 && (bestit1 == CEntityManager::instance().entities().end() || (*bestit1)->ArrivalTime >(*it)->ArrivalTime))
				{
					bestit1 = it;
				}
				else if((*it)->CurrentScore > 0 && (bestit2 == CEntityManager::instance().entities().end() || (*bestit2)->ArrivalTime >(*it)->ArrivalTime))
				{
					bestit2 = it;
				}
				else if((*it)->CurrentScore > 0 && (bestit3 == CEntityManager::instance().entities().end() || (*bestit3)->ArrivalTime > (*it)->ArrivalTime))
				{
					bestit3 = it;
				}
			}
			if(CLevelManager::instance().bonusTime())
			{
				switch(CEntityManager::instance().entities().size())
				{
				case 0:
				case 1:
					break;
				case 2:
					if(bestit1 != CEntityManager::instance().entities().end()) (*bestit1)->CurrentScore +=  50;
					break;
				case 3:
					if(bestit1 != CEntityManager::instance().entities().end()) (*bestit1)->CurrentScore += 100;
					if(bestit2 != CEntityManager::instance().entities().end()) (*bestit2)->CurrentScore +=  50;
					break;
				default:
					if(bestit1 != CEntityManager::instance().entities().end()) (*bestit1)->CurrentScore += 150;
					if(bestit2 != CEntityManager::instance().entities().end()) (*bestit2)->CurrentScore += 100;
					if(bestit3 != CEntityManager::instance().entities().end()) (*bestit3)->CurrentScore +=  50;
					break;
				}
			}

//			CMessage msgout("END_SESSION");
			CNetMessage msgout(CNetMessage::EndSession);
//			uint8 nb = CEntityManager::instance().entities().size();
//			msgout.serial(nb);
			for(it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
			{
				if((*it)->type() == CEntity::Client)
				{
					bool breakTime = false; // ace set to false
//					string res;//ace = mtLevelManager::updateStats((*it)->name(), (*it)->CurrentScore, (*it)->Time, breakTime, true);
					string res = CLevelManager::instance().updateStats((*it)->name(), (*it)->CurrentScore, (*it)->ArrivalTime, breakTime);
					if(!res.empty())
					{
						chat.push_back(res);
						if(breakTime)
						{
							(*it)->CurrentScore += 1000;
						}
					}
				}

				(*it)->Score +=(*it)->CurrentScore;

				CConfigFile::CVar &accounts = IService::getInstance()->ConfigFile.getVar("Accounts");
				for(sint i = 0; i < accounts.size(); i+=3)
				{
					if(accounts.asString(i) == (*it)->name())
					{
						accounts.setAsString(toString((*it)->Score), i+2);
						break;
					}
				}

				uint8 eid = (*it)->id();
				msgout.serial(eid, (*it)->CurrentScore, (*it)->Score);

				dBodySetLinearVel((*it)->Body, 0.0f, 0.0f, 0.0f);
				dBodySetAngularVel((*it)->Body, 0.0f, 0.0f, 0.0f);
				(*it)->Force = CVector::Null;
			}
			
			// now save scores
			IService::getInstance()->ConfigFile.save();
			
			// send the message to all entities
			CNetwork::instance().send(msgout);
			/*for(it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
			{
				if((*it)->From != 0)
					Server->send(msgout, (*it)->From);
			}*/


			CEntityManager::instance().saveAllValidReplay();
			changeState(CEndingSessionState::instance());
			CSessionManager::instance().endTime(currentTime);
			CSessionManager::instance().forceEnding(false);
		}
	}
	
	// chat must be call outside the client accessor
	for(uint i = 0; i < chat.size(); i++)
		CNetwork::instance().sendChat(chat[i]);
	}
}
