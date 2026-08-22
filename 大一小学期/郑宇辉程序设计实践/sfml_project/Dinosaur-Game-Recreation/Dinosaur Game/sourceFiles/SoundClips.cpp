#include "..\headerFiles\SoundClips.h"

SoundClips::SoundClips()
{
    if (!deathSoundBuff.loadFromFile("gameAudio\\die.wav"))
    {
        // Handle loading error
    }
    deathSound.setBuffer(deathSoundBuff);

    if (!jumpSoundBuff.loadFromFile("gameAudio\\jump.wav"))
    {
        // Handle loading error
    }
    jumpSound.setBuffer(jumpSoundBuff);

    if (!pointSoundBuff.loadFromFile("gameAudio\\point.wav"))
    {
        // Handle loading error
    }
    pointSound.setBuffer(pointSoundBuff);
    pointSound.setVolume(90.0f);
}

void SoundClips::playDeathSound()
{
    deathSound.play();
}

void SoundClips::playJumpSound()
{
    jumpSound.play();
}

void SoundClips::playPointSound()
{
    pointSound.play();
}