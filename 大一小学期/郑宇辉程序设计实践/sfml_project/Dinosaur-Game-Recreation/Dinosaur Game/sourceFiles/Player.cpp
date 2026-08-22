#include "../headerFiles/Player.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <math.h>

namespace fs = std::experimental::filesystem;

Player::Player(float gameLevelY)
    : player(),
      playerCanJump(true), playerRequestedJump(false), jumpSpeed(100.0f),
      playerMinHeightY(), velocity(0.0, 0.0)
{
    const fs::directory_iterator end{};

    for (fs::directory_iterator iter{"gameSprites\\DinoSprites"}; iter != end; ++iter)
    {
        sf::Texture textureTemp = *new sf::Texture;
        if (fs::is_regular_file(*iter))
        {
            if (!textureTemp.loadFromFile(iter->path().string()))
            {
                // Handle loading error
            }
            else
            {
                playerTextures.push_back(textureTemp);
            }
        }
    }
    player.setTexture(playerTextures[1]);

    player.setScale(0.5f, 0.5f);

    playerMinHeightY = gameLevelY - (player.getTextureRect().height / 2.0f);
    player.setPosition(100.f, playerMinHeightY);
}

sf::Sprite Player::drawPlayer()
{
    return player;
}

void Player::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    if (key == sf::Keyboard::Space || key == sf::Keyboard::Up && playerCanJump)
    {
        playerRequestedJump = true;
    }
}

void Player::handlePlayerMovement(sf::Time deltaTime, SoundClips *audio)
{
    if (playerRequestedJump && playerCanJump)
    {
        audio->playJumpSound();
        playerCanJump = false;
        velocity.y = -sqrtf(8.0f * 981.0f * jumpSpeed);
    }
    else if (player.getPosition().y < playerMinHeightY)
    {
        velocity.y += 4.0f * 981.0f * deltaTime.asSeconds();
    }
    else if (player.getPosition().y > playerMinHeightY - 40)
    {
        // Bind back to minimum height position
        float interpolatedY = player.getPosition().y - playerMinHeightY;
        velocity.y = 0 * deltaTime.asSeconds();
        player.move(0.0f, -interpolatedY);

        playerCanJump = true;
    }

    playerRequestedJump = false;
    player.move(velocity * deltaTime.asSeconds());
}

sf::FloatRect Player::getPaddedGlobalBounds()
{
    sf::FloatRect paddedRect = player.getGlobalBounds();

    // Create padding for sprite so collision is not so strict.
    // Adds a 10% padding from the width/height of the texture
    // and shrinks the Rect to 80%, so that it is still
    // remains in the center of the sprite
    paddedRect.top = paddedRect.width * 0.1f + paddedRect.top;
    paddedRect.left = paddedRect.height * 0.1f + paddedRect.left;
    paddedRect.width = paddedRect.width * 0.8f;
    paddedRect.height = paddedRect.height * 0.8f;

    return paddedRect;
}

void Player::movementAnimation()
{
    if (!playerCanJump)
    {
        // player is jumping
        player.setTexture(playerTextures[1]);
    }
    else if (playerAlive)
    {
        // player is running
        if (walkingTimer.getElapsedTime().asSeconds() > animWalkTimeFrame)
        {
            walkingTimer.restart();
            if (step == 0)
            {
                step = 1;
                player.setTexture(playerTextures[3]);
            }
            else
            {
                step = 0;
                player.setTexture(playerTextures[4]);
            }
        }
    }
    else if (!playerAlive)
    {
        // player has lost
        player.setTexture(playerTextures[5]);
    }
}

bool Player::isPlayerAlive()
{
    return playerAlive;
}

void Player::playerStatus(bool status)
{
    playerAlive = status;
}

void Player::resetPlayer()
{
    // Reset player position
    player.setPosition(100.f, playerMinHeightY);
    playerCanJump = true;
    playerRequestedJump = false;
    playerAlive = true;

    // Reset player animation
    player.setTexture(playerTextures[1]);
}