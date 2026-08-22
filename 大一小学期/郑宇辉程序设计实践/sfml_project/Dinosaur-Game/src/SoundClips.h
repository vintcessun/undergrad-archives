#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class SoundClips {
public:
    SoundClips();
    void playDeathSound();
    void playJumpSound();
    void playPointSound();

private:
    sf::SoundBuffer deathSoundBuff;
    sf::SoundBuffer jumpSoundBuff;
    sf::SoundBuffer pointSoundBuff;

    sf::Sound deathSound;
    sf::Sound jumpSound;
    sf::Sound pointSound;

private:
};
