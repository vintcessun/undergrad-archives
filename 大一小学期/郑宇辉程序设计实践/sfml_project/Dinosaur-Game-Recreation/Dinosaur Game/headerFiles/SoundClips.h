#ifndef SOUNDCLIP_H
#define SOUNDCLIP_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <list>

class SoundClips
{
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

#endif /* SOUNDCLIP_H */
