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
// This class manages all entities using a 256 entries static array.
// Using that static array to be able to be multithread safe.
// The entry 255 is always empty because 255 is the code for "unknown" entity.
//

#ifndef MTPT_TASK_MANAGER
#define MTPT_TASK_MANAGER


//
// Includes
//



//
// Classes
//

class CTaskManager : public CSingleton<CTaskManager>
{
public:

	// do all the init and execution
	void execute();

	// add a task into the manager
	void add(ITask &task, sint32 order = -1, bool startNow = true);

	// remove the task from the manager
	void remove(ITask &task);

	// call this function if you want to exit the game
	void exit();

	CTaskManager();

private:

	std::list<ITask*>	Tasks;
	std::list<ITask*>	OrderSortedTasks;

	bool Exit;

	friend struct displayTasksClass;
};


bool pauseTaskManager(bool waitAck=true);
bool isTaskManagerPaused();
void resumeTaskManager();

#endif
