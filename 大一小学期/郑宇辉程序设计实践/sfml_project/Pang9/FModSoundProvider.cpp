#include "StdAfx.h"
#include "FModSoundProvider.h"


FModSoundProvider::FModSoundProvider() :
_system(NULL)
{
	InitFMOD();
}

FModSoundProvider::~FModSoundProvider()
{
	if(_sound != NULL)
	{
		_sound->release();
	}
	if(_song != NULL)
	{
		_song->release();
	}
	
	if(_system != NULL)
	{
		_system->release();
	}
}

void FModSoundProvider::PlaySound(std::string filename)
{
	_system->createSound(filename.c_str(),FMOD_DEFAULT,0, &_sound);
	_system->playSound(FMOD_CHANNEL_REUSE,_sound,false,&_soundChannel);
}

void FModSoundProvider::PlaySong(std::string filename, bool looping)
{
	if(looping)
		_system->createSound(filename.c_str(),FMOD_LOOP_NORMAL ,0, &_song);
	else
		_system->createSound(filename.c_str(),FMOD_DEFAULT,0, &_song);

	_system->playSound(FMOD_CHANNEL_REUSE,_song,false,&_musicChannel);
}

void FModSoundProvider::StopAllSounds()
{
	{
   int            channelIndex;
   FMOD_RESULT    result;
   FMOD::Channel *nextChannel;
 
   for (channelIndex = 0; channelIndex < 100; channelIndex++)
      {
      result = _system->getChannel(channelIndex, &nextChannel);
      if ((result == FMOD_OK) && (nextChannel != NULL))
         nextChannel->stop();
      }
   }
}
	
bool FModSoundProvider::IsSoundPlaying()
{
	bool result;
	_soundChannel->isPlaying(&result);
	return result;
}

bool FModSoundProvider::IsSongPlaying()
{
	bool result;
	_musicChannel->isPlaying(&result);
	return result;
}

void FModSoundProvider::ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
	    exit(-1);
	}
}


void FModSoundProvider::InitFMOD()
{
	FMOD_RESULT      result; 
	unsigned int     version; 
	int              numdrivers; 
	FMOD_SPEAKERMODE speakermode; 
	FMOD_CAPS        caps; 
	char             name[256]; 
 
	/* 
		Create a System object and initialize. 
	*/ 
	result = FMOD::System_Create(&_system); 
	ERRCHECK(result); 
 
	result = _system->getVersion(&version); 
	ERRCHECK(result); 
 
	if (version < FMOD_VERSION) 
	{ 
		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", 
	version, FMOD_VERSION); 
		return; 
	} 
 
	result = _system->getNumDrivers(&numdrivers); 
	ERRCHECK(result); 
 
	if (numdrivers == 0) 
	{ 
		result = _system->setOutput(FMOD_OUTPUTTYPE_NOSOUND); 
		ERRCHECK(result); 
	} 
	else 
	{
		result = _system->getDriverCaps(0, &caps, 0, &speakermode); 
		ERRCHECK(result); 
 
		/* 
			Set the user selected speaker mode. 
		*/ 
		result = _system->setSpeakerMode(speakermode); 
		ERRCHECK(result); 
 
		if (caps & FMOD_CAPS_HARDWARE_EMULATED) 
		{ 
			/* 
				The user has the 'Acceleration' slider set to off!  This is really bad  
				for latency! You might want to warn the user about this. 
			*/ 
			result = _system->setDSPBufferSize(1024, 10); 
			ERRCHECK(result); 
		} 
 
		result = _system->getDriverInfo(0, name, 256, 0); 
		ERRCHECK(result); 
 
		if (strstr(name, "SigmaTel")) 
		{ 
			/* 
				Sigmatel sound devices crackle for some reason if the format is PCM 16bit. 
				PCM floating point output seems to solve it. 
			*/ 
			result = _system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, 
	FMOD_DSP_RESAMPLER_LINEAR); 
			ERRCHECK(result); 
		} 
	} 
 
	result = _system->init(100, FMOD_INIT_NORMAL, 0); 
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER) 
	{ 
		/* 
			Ok, the speaker mode selected isn't supported by this soundcard.  Switch it  
			back to stereo... 
		*/ 
		result = _system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO); 
		ERRCHECK(result); 
 
		/* 
			... and re-init. 
		*/ 
		result = _system->init(100, FMOD_INIT_NORMAL, 0); 
	} 
	ERRCHECK(result);
}