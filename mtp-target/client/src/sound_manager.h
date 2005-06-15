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

#ifndef MTPT_SOUND_MANAGER_H
#define MTPT_SOUND_MANAGER_H


//
// Includes
//
#include <fstream>
#include <iostream>
#include <vector>

#ifdef USE_FMOD
#	include <fmod.h>
#	include <fmod_errors.h>
#endif

#include <nel/misc/singleton.h>


//
// Classes
//

class CSoundManager : public NLMISC::CSingleton<CSoundManager>, public ITask
{
public:

	virtual void init();
	virtual void update();
	virtual void render();
	virtual void release();

	void updateListener(const NLMISC::CVector &position, const NLMISC::CVector &velocity, const NLMISC::CVector &atVector, const NLMISC::CVector &upVector);
	
	virtual std::string name() const { return "CSoundManager"; }

	class CEntitySoundsDescriptor
	{
	public:

		CEntitySoundsDescriptor();
		~CEntitySoundsDescriptor();
		
		enum TSoundId { BallOpen, BallClose, Splash, Impact, SoundCount };
		void play(TSoundId sound, bool loop = false);
		void stop(TSoundId sound);
		void update3d(const NLMISC::CVector &position, const NLMISC::CVector &velocity);
			
	protected:

		void reset();

		enum { CommandNone, CommandPlay, CommandPlayLoop, CommandStop };
		
		struct TSound
		{
			int Channel;
			uint Command;
		};

		TSound Sounds[SoundCount];
		float Position[3];
		float Velocity[3];

		friend class CSoundManager;
	};

	// call this method once when an entity is created : entity's sounds will be updated auomatically
	void registerEntity(CEntitySoundsDescriptor &esd);
	
	// call this method once when an entity is destroyed : destroy some datas and the entity's sounds will not be updated anymore
	void unregisterEntity(CEntitySoundsDescriptor &esd);
	
	// gui sounds
	enum TGuiSound { GuiReady, GuiSoundCount };

	void playGuiSound(TGuiSound sound, bool loop = false);
	void stopGuiSound(TGuiSound sound);
	
	// streams
	// you can only play one stream at a time (the old stream will be automaticaly stopped)
	//void playStream(const std::string &filename, bool loop = false); // you can only play one stream at a time (the old stream will be automaticaly stopped)
	void playStream(bool useM3U = false); // for m3u list support
	void stopStream();

#ifdef USE_FMOD
	
	// sample
	FSOUND_SAMPLE *entitySoundSamples[CSoundManager::CEntitySoundsDescriptor::SoundCount];
	typedef std::list<CEntitySoundsDescriptor *> TEntitySoundsDescriptorList;
	TEntitySoundsDescriptorList entitySoundsDescriptorList;
	
	// gui
	FSOUND_SAMPLE *guiSoundSamples[CSoundManager::GuiSoundCount];
	int guiChannels[CSoundManager::GuiSoundCount];
	
	// for the stream
	FSOUND_STREAM *streamStream;
	
	// listener
	float ListenerPosition[3];
	float ListenerVelocity[3];
	NLMISC::CVector ListenerAtVector;
	NLMISC::CVector ListenerUpVector;

#endif // USE_FMOD

	friend class NLMISC::CSingleton<CSoundManager>;
	CSoundManager();

	std::string getNextMusicRes(); // get next file url from m3u playlist
	std::ifstream m3uFile; // stream for playlist file
	bool useM3U; // classic music or m3u playlist
	bool useM3Ushuffle; // shuffle playlist or not
	std::vector<std::string> m3uVector; // for holding playlist at runtime
	int m3uNowPlaying; // number of now playing file
	
private:
	bool isInit;

};

#endif
