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

#include <nel/misc/command.h>

#include "main.h"
#include "task_manager.h"
#include "entity_manager.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;


//
// Variables
//

CSynchronized<PauseFlags> taskManagerPauseFlags("taskManagerPauseFlags");


//
// Functions
//

void CTaskManager::execute()
{
	CHTimer::startBench();

	Exit = false;

	while(!Exit)
	{
//		nlinfo("***** new frame, update");
		for(list<ITask*>::iterator it = OrderSortedTasks.begin(); it != OrderSortedTasks.end(); it++)
		{
			ITask *task = *it;
			if((*it)->Execute)
			{
//				nlinfo("Update %s", (*it)->name().c_str());
				(*it)->HTimerUpdate.before();
				(*it)->update();
				(*it)->HTimerUpdate.after();
			}
		}

//		nlinfo("----- render");
		for(list<ITask*>::iterator it = OrderSortedTasks.begin(); it != OrderSortedTasks.end(); it++)
		{
			if((*it)->Execute)
			{
//				nlinfo("Render %s", (*it)->name().c_str());
				(*it)->HTimerRender.before();
				(*it)->render();
				(*it)->HTimerRender.after();
			}
		}
//		nlinfo("----- end frame");

		// release and remove task from Task
		for(list<ITask*>::iterator it2 = Tasks.begin(); it2 != Tasks.end(); )
		{
			if((*it2)->NeedRemove)
			{
				remove(*(*it2));
				it2 = Tasks.begin();
			}
			else
			{
				it2++;
			}
		}
		checkTaskManagerPaused();
		CEntityManager::instance().flushAddRemoveList();
	}

	for(std::list<ITask*>::reverse_iterator it = Tasks.rbegin(); it != Tasks.rend(); it++)
	{
//		nlinfo("Release %s", (*it)->name().c_str());
		(*it)->release();
	}

	CHTimer::endBench();
}

void CTaskManager::remove(ITask &task)
{
	nlinfo("Removing task %s", task.name().c_str());
	// release and remove task from Task
	for(std::list<ITask*>::iterator it = Tasks.begin(); it != Tasks.end();)
	{
		if(&task == (*it))
		{
			std::list<ITask*>::iterator itr = it;
			it++;
			Tasks.remove(*itr);
		}
		else
		{
			it++;
		}
	}
	// remove task from OrderSortedTasks
	for(std::list<ITask*>::iterator it = OrderSortedTasks.begin(); it != OrderSortedTasks.end();)
	{
		if(&task == (*it))
		{
			std::list<ITask*>::iterator itr = it;
			it++;
			OrderSortedTasks.remove(*itr);
		}
		else
		{
			it++;
		}
	}
	nlinfo("Removed task %s from list", task.name().c_str());
	task.release();
	nlinfo("Removed task %s and release called", task.name().c_str());
}

void CTaskManager::add(ITask &task, sint32 order, bool startNow)
{
	task.Order = order;

	std::string n = task.name() + "Update";
	strcpy(task.NameUpdate, n.c_str());
	n = task.name() + "Render";
	strcpy(task.NameRender, n.c_str());
	task.HTimerUpdate.setName(task.NameUpdate);
	task.HTimerRender.setName(task.NameRender);

	task.NeedRemove = false;
	task.Execute = startNow;

	Tasks.push_back(&task);

	if(order == -1)
	{
		OrderSortedTasks.push_back(&task);
	}
	else
	{
		list<ITask*>::iterator it;
		for(it = OrderSortedTasks.begin(); it != OrderSortedTasks.end(); it++)
		{
			if((*it)->Order > order)
			{
				break;
			}
		}
		OrderSortedTasks.insert(it, &task);
	}

	nlinfo("Init during exec %s", task.name().c_str());
	task.init();
}

CTaskManager::CTaskManager()
{
}

void CTaskManager::exit()
{
	Exit = true;
}


//
// Commands
//

NLMISC_COMMAND(displayTasks, "display all task", "")
{
	if(args.size() != 0) return false;

	log.displayNL("There's %d tasks:", CTaskManager::instance().Tasks.size());
	log.displayNL("Init order call:");
	for(std::list<ITask*>::iterator it = CTaskManager::instance().Tasks.begin(); it != CTaskManager::instance().Tasks.end(); it++)
	{
		log.displayNL("  %s %s", (*it)->name().c_str(), ((*it)->Execute?"Running":"Stop"));
	}
	log.displayNL("Update order call:");
	for(std::list<ITask*>::iterator it = CTaskManager::instance().OrderSortedTasks.begin(); it != CTaskManager::instance().OrderSortedTasks.end(); it++)
	{
		log.displayNL("  %s", (*it)->name().c_str());
		(*it)->render();
	}
	log.displayNL("Release order call:");
	for(std::list<ITask*>::reverse_iterator it = CTaskManager::instance().Tasks.rbegin(); it != CTaskManager::instance().Tasks.rend(); it++)
	{
		log.displayNL("  %s", (*it)->name().c_str());
		(*it)->release();
	}

	return true;
}


void checkTaskManagerPaused()
{
	{
		bool pause;
		{
			CSynchronized<PauseFlags>::CAccessor acces(&taskManagerPauseFlags);
			pause = acces.value().pauseCount>0;
			if(pause)
				acces.value().ackPaused = true;
		}
		while (pause) 
		{
			nlSleep(10);
			{
				CSynchronized<PauseFlags>::CAccessor acces(&taskManagerPauseFlags);
				pause = acces.value().pauseCount>0;
				if(pause)
					acces.value().ackPaused = true;
			}
		}
	}
	{
		CSynchronized<PauseFlags>::CAccessor acces(&taskManagerPauseFlags);
		acces.value().ackPaused = false;
	}	
}

bool pauseTaskManager(bool waitAck)
{
	bool pause;
	{
		CSynchronized<PauseFlags>::CAccessor acces(&taskManagerPauseFlags);
		pause = acces.value().pauseCount>0;
	}
	if(!pause) 
	{
		bool ackPaused;
		{
			CSynchronized<PauseFlags>::CAccessor acces(&taskManagerPauseFlags);
			ackPaused = false;
			acces.value().pauseCount++;
		}
		if(!waitAck) return true;
		while(!ackPaused)
		{
			nlSleep(10);
			{
				CSynchronized<PauseFlags>::CAccessor acces(&taskManagerPauseFlags);
				ackPaused = acces.value().ackPaused;
			}
		}
	}
	else
		return false;
	return true;
}

bool isTaskManagerPaused()
{
	bool ackPaused;
	{
		CSynchronized<PauseFlags>::CAccessor acces(&taskManagerPauseFlags);
		ackPaused = acces.value().ackPaused;
	}
	return ackPaused;
}

void resumeTaskManager()
{
	CSynchronized<PauseFlags>::CAccessor acces(&taskManagerPauseFlags);
	if(acces.value().pauseCount>0) 
	{
		acces.value().pauseCount--;
		nlassert(acces.value().pauseCount>=0);
	}	
}
