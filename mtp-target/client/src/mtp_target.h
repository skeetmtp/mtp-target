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
// This is the main class that manages all other classes
//

#ifndef MTP_TARGET_H
#define MTP_TARGET_H

#define MTPT_RELEASE_VERSION_NUMBER "1.0.3"
#define MTPT_RELEASE_VERSION_NAME ""


//
// Includes
//

#include <nel/misc/config_file.h>
#include <nel/misc/vector.h>
#include <nel/misc/matrix.h>
#include <nel/misc/reader_writer.h>

#include "global.h"
#include "controler.h"
#include "interface.h"


//
// Variables
//

extern bool DisplayDebug;
extern bool FollowEntity;
extern CVector ControlerFreeLookPos;
extern CMatrix ControlerCamMatrix;
extern std::string Cookie, FSAddr, ReplayFile;


//
// Classes
//


class CMtpTarget : public CSingleton<CMtpTarget>, public ITask
{
public:

	virtual void init();
	virtual void update();
	virtual void render() { }
	virtual void release() { }

	void reset();
	void error(std::string reason);
	
	virtual std::string name() { return "CMtpTarget"; }

	CControler		&controler() const { nlassert(Controler); return *Controler; }

	// called by main thread
	void	loadNewSession();

	// called by the net thread
	void	startSession(float timeBeforeSessionStart, float timeBeforeTimeout, const std::string &levelName);
	void	resetFollowedEntity();

	float	timeBeforeSessionStart() const { return TimeBeforeSessionStart; }
	float	timeBeforeTimeout() const { return TimeBeforeTimeout; }
	void	timeBeforeTimeout(float t) { TimeBeforeTimeout = t; }
	
	enum TState { eLoginPassword, eConnect, eBeforeFirstSession, eStartSession, eReady, eGame, eEndSession };
	
	TState State;

	void displayTutorialInfo(bool b);
	bool displayTutorialInfo();
	
private:

	CControler		*Controler;
	
	bool			NewSession;
	float			TimeBeforeSessionStart;
	float			TimeBeforeTimeout;
	std::string		NewLevelName;
	bool			DoError;
	
	void _error();
	std::string		ErrorReason;
	bool			DisplayTutorialInfo;
};







//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////


class mtpTarget
{
public:

	static mtpTarget &instance();

	void	init();
	void	update();
	void	render();
	//void	renderInterface ();
	void	release ();

	void	everybodyReady();
	void	endSession();
	
//	void	resetFollowedEntity();
	void	toggleQuakeControl();

//	float	getTime () { return Time; }
//	float	getDeltaTime () { return DeltaTime; }

	//mtInterface		 Interface2d;
	
//	static NLMISC::CConfigFile ConfigFile;

		
private:

	static mtpTarget *Instance;

//	bool loadPhysicLevel(const char *filename);
//	bool loadLevel(const char *filename);
	
	bool	FirstResetCamera;

	float TimeBeforeSessionStart, TimeBeforeTimeout;

	// use accessor to access these values
//	float Time, DeltaTime, OldTime;
//	NLMISC::CValueSmoother DeltaTimeSmooth;
//	double FirstTime;
//	void updateTime ();
//	void updateSound ();

	friend class mtInterface;
	friend class CResourceManager;
};

#endif
