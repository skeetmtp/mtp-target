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
		CEntityManager::CEntities::CReadAccessor acces(CEntityManager::instance().entities());
		CEntityManager::EntityConstIt it;

		bool end = true;

		if(currentTime > CSessionManager::instance().startTime()+(TTime)TimeTimeout)
		{
			nlinfo("Session time out!!!");
		}
		else
		{
			for(it = acces.value().begin(); it != acces.value().end(); it++)
			{
				CEntity *c = *it;
				if(c->InSession)
				{
					const dReal *vel = dBodyGetLinearVel(c->Body);
					c->LastVel[(*it)->LastVelPos%10]  =(float)fabs(vel[0]);
					c->LastVel[(*it)->LastVelPos%10] +=(float)fabs(vel[1]);
					c->LastVel[(*it)->LastVelPos%10] +=(float)fabs(vel[2]);
					c->LastVelPos++;

					float val = 0.0f;
					for(uint i = 0; i < 10; i++)
						val += c->LastVel[i];
					
					if(val>MinVelBeforeEnd) //si le client bouge
					{
						end = false;
						c->FirstStop = true;
						//c->stopedScore = c->CurrentScore;
					}
					else //si il est arrete
					{
						if(c->FirstStop)
						{
							//nlwarning("%s[%d] stop stopedScore=%d CurrentScore=%d lasttouchedId=%d",c->Name.c_str(),c->Id,c->stopedScore,c->CurrentScore,c->lastTouchedClientId);
							if(c->StopedScore != 0 && c->CurrentScore != c->StopedScore) //mais k il etait arreter avant et ke maintenant il n est plus sur une platforme de point
							{
								nlwarning("%s[%d] has been pushed by [%d]", c->name().c_str(), c->id(), c->LastTouchedClientId);
								for(CEntityManager::EntityConstIt it2 = acces.value().begin(); it2 != acces.value().end(); it2++)
								{
									CEntity *c2 = *it2;
									if(c->LastTouchedClientId == c2->id())
									{
										//on donne ses point a celui ki l a pousse
										c2->PushScore += c->StopedScore - c->CurrentScore;
										nlwarning("%s[%d] has stole %d point from %s[%d]",c2->name().c_str(),c2->id(),c->StopedScore,c->name().c_str(),c->id());
										break;
									}
								}
							}
						}
						c->FirstStop = false;
						c->StopedScore = c->CurrentScore + c->PushScore;
						c->PushScore = 0;
						if(c->Time == 0)
						{
							c->Time =(float)(currentTime - CSessionManager::instance().startTime())/1000.0f;
							//nlinfo("setting time %s %f",(*it)->Name.c_str(),(*it)->Time);
						}
					}
				}
			}
		}

		if(CSessionManager::instance().forceEnding() || end)
		{
			CEntityManager::EntityConstIt  bestit1 = acces.value().end();
			CEntityManager::EntityConstIt  bestit2 = acces.value().end();
			CEntityManager::EntityConstIt  bestit3 = acces.value().end();
			for(it = acces.value().begin(); it != acces.value().end(); it++)
			{
				if((*it)->CurrentScore > 0 && (bestit1 == acces.value().end() || (*bestit1)->Time >(*it)->Time))
				{
					bestit1 = it;
				}
				else if((*it)->CurrentScore > 0 && (bestit2 == acces.value().end() || (*bestit2)->Time >(*it)->Time))
				{
					bestit2 = it;
				}
				else if((*it)->CurrentScore > 0 && (bestit3 == acces.value().end() || (*bestit3)->Time > (*it)->Time))
				{
					bestit3 = it;
				}
			}
/*
			if (bestit1 != acces.value().end())
				nlinfo ("bestit1 = %s %g", (*bestit1)->Name.c_str(), (*bestit1)->Time);
			else
				nlinfo ("bestit1 = NO");
			
			if (bestit2 != acces.value().end())
				nlinfo ("bestit2 = %s %g", (*bestit2)->Name.c_str(), (*bestit2)->Time);
			else
				nlinfo ("bestit2 = NO");
			
			if (bestit3 != acces.value().end())
				nlinfo ("bestit3 = %s %g", (*bestit3)->Name.c_str(), (*bestit3)->Time);
			else
				nlinfo ("bestit3 = NO");
*/
			switch(acces.value().size())
			{
			case 0:
			case 1:
				break;
			case 2:
				if(bestit1 != acces.value().end()) (*bestit1)->CurrentScore +=  50;
				break;
			case 3:
				if(bestit1 != acces.value().end()) (*bestit1)->CurrentScore += 100;
				if(bestit2 != acces.value().end()) (*bestit2)->CurrentScore +=  50;
				break;
			default:
				if(bestit1 != acces.value().end()) (*bestit1)->CurrentScore += 150;
				if(bestit2 != acces.value().end()) (*bestit2)->CurrentScore += 100;
				if(bestit3 != acces.value().end()) (*bestit3)->CurrentScore +=  50;
				break;
			}

//			CMessage msgout("END_SESSION");
			CNetMessage msgout(CNetMessage::EndSession);
//			uint8 nb = acces.value().size();
//			msgout.serial(nb);
			for(it = acces.value().begin(); it != acces.value().end(); it++)
			{
				if((*it)->type() == CEntity::Client)
				{
					bool breakTime = false; // ace set to false
					string res;//ace = mtLevelManager::updateStats((*it)->name(), (*it)->CurrentScore, (*it)->Time, breakTime, true);
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
			/*for(it = acces.value().begin(); it != acces.value().end(); it++)
			{
				if((*it)->From != 0)
					Server->send(msgout, (*it)->From);
			}*/

			CSessionManager::instance().saveAllValidReplay(acces);

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
