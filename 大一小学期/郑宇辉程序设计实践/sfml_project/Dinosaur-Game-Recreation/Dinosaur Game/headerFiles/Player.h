#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "SoundClips.h"

#include <experimental/fileSystem>

class Player
{
public:
    Player(float gameLevelY);
    sf::Sprite drawPlayer();
    void handlePlayerMovement(sf::Time, SoundClips *audio);
    void handlePlayerInput(sf::Keyboard::Key, bool);

    sf::FloatRect getPaddedGlobalBounds();

    void movementAnimation();

    bool isPlayerAlive();
    void playerStatus(bool status);

    void resetPlayer();

private:
    std::vector<sf::Texture> playerTextures;
    sf::Sprite player;

    bool playerAlive = true;
    bool playerCanJump;
    bool playerRequestedJump;
    float jumpSpeed;

    float playerMinHeightY;

    sf::Vector2f velocity;

    float animWalkTimeFrame = 0.1f;
    sf::Clock walkingTimer;
    int step = 0;
};
