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

#ifndef MTPT_LEVEL_MANAGER
#define MTPT_LEVEL_MANAGER


//
// Includes
//

#include "level.h"


//
// Classes
//

class CLevelManager : public CSingleton<CLevelManager>
{
public:

	CLevel &currentLevel() { nlassert(CurrentLevel); return *CurrentLevel;}
	void init();
	void update();
	void release();

	void display(NLMISC::CLog *log = NLMISC::InfoLog) const;

	// find and load a new level
	bool newLevel(std::string &str1, std::string &str2);

	void nextLevel();
	
	std::string levelName() const;
	std::string levelFilename() const;

	// return true if a current level exists
	bool haveCurrentLevel() const { return CurrentLevel != 0; }
	uint32 levelSessionCount();
	void maxLevelSessionCount(uint32 levelCount);
	
	bool bonusTime();
	void bonusTime(bool b);
	
	bool recordBest();
	void recordBest(bool b);

	double timeTimeout();
	void timeTimeout(double timeout);

	std::string updateStats(const std::string &name, sint32 score, float time, bool &breakTime);
	
private:

	uint8						NextLevelId;
	uint32						MaxLevelSessionCount;
	uint32						LevelSessionCount;
	bool						BonusTime;
	bool						RecordBest;
	
	CLevel						*CurrentLevel;
	double						TimeTimeout;

	// private ctor due to singleton pattern
	CLevelManager();

	friend class CSingleton<CLevelManager>;
};

#endif
