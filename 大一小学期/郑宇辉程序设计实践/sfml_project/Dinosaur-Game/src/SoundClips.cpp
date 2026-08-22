#include "SoundClips.h"

SoundClips::SoundClips()
    : deathSoundBuff("resources/die.wav"),
      deathSound(deathSoundBuff),
      jumpSoundBuff("resources/jump.wav"),
      jumpSound(jumpSoundBuff),
      pointSoundBuff("resources/point.wav"),
      pointSound(pointSoundBuff) {
    pointSound.setVolume(90.0f);
}

void SoundClips::playDeathSound() { deathSound.play(); }

void SoundClips::playJumpSound() { jumpSound.play(); }

void SoundClips::playPointSound() { pointSound.play(); }