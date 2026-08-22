#pragma once
#include "IAudioProvider.h"
#include "fmod/fmod.h"
#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"


class FModSoundProvider : public IAudioProvider
{
public:

	FModSoundProvider();
	virtual ~FModSoundProvider();

	virtual void PlaySound(std::string filename);
	virtual void PlaySong(std::string filename, bool looping);
	virtual void StopAllSounds();
	
	virtual bool IsSoundPlaying();
	virtual bool IsSongPlaying();


private:
	void InitFMOD();
	FMOD::System    * _system;
	FMOD::Sound * _sound;
	FMOD::Channel * _soundChannel;
	FMOD::Sound * _song;
	FMOD::Channel * _musicChannel;
	void ERRCHECK(FMOD_RESULT);
};

