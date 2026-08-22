#pragma once

#include "stdafx.h"
#include <string.h>

class IAudioProvider
{
public:
	virtual void PlaySound(std::string filename) = 0;
	virtual void PlaySong(std::string filename, bool looping) = 0;
	virtual void StopAllSounds() = 0;
	
	virtual bool IsSoundPlaying() = 0;
	virtual bool IsSongPlaying() = 0;
};