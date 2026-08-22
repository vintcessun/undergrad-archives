#include "..\headerFiles\Obstacle.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

Obstacle::Obstacle(std::string obstaclePathName)
    : obstacleTexture(), obstacle(), obstacleY()
{
    // std::string filename = "gameSprites\\ObstacleSprites\\" + obstacleName + ".png";
    if (!obstacleTexture.loadFromFile(obstaclePathName))
    {
        // Handle loading error
    }
    obstacle.setTexture(obstacleTexture);
    obstacle.setScale(0.5f, 0.5f);
    obstacleY = obstacle.getTextureRect().height / 2.0f;
}

sf::Sprite Obstacle::getObstacle()
{
    return obstacle;
}

void Obstacle::updatePosition(float xPos, float yPos)
{
    obstacle.setPosition(xPos, yPos - obstacleY);
}

void Obstacle::moveObstacle(sf::Vector2f trackVelocity, sf::Time deltaTime)
{
    obstacle.move(trackVelocity * deltaTime.asSeconds());
}

// Check to see if obstacle has passed screen
bool Obstacle::checkPosition()
{
    if (obstacle.getPosition().x < obstacleEndRange)
    {
        obstacle.move(sf::Vector2f(0.0f, 0.0f));
        obstacle.setPosition(-100.0f, -100.0f);
        return true;
    }
    return false;
}

sf::FloatRect Obstacle::getPaddedGlobalBounds()
{
    sf::FloatRect paddedRect = obstacle.getGlobalBounds();

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