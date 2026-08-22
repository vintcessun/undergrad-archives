#include "Obstacle.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

Obstacle::Obstacle(std::string obstaclePathName)
    : obstacleTexture(obstaclePathName),
      obstacle(obstacleTexture),
      obstacleY() {
    obstacle.setScale({0.5f, 0.5f});
    obstacleY = obstacle.getTextureRect().size.y / 2.0f;
}

sf::Sprite Obstacle::getObstacle() { return obstacle; }

void Obstacle::updatePosition(float xPos, float yPos) {
    obstacle.setPosition({xPos, yPos - obstacleY});
}

void Obstacle::moveObstacle(sf::Vector2f trackVelocity, sf::Time deltaTime) {
    obstacle.move(trackVelocity * deltaTime.asSeconds());
}

bool Obstacle::checkPosition() {
    if (obstacle.getPosition().x < obstacleEndRange) {
        obstacle.move(sf::Vector2f(0.0f, 0.0f));
        obstacle.setPosition({-100.0f, -100.0f});
        return true;
    }
    return false;
}

sf::FloatRect Obstacle::getPaddedGlobalBounds() {
    sf::FloatRect paddedRect = obstacle.getGlobalBounds();

    paddedRect.position.y += paddedRect.size.x * 0.1f;
    paddedRect.position.x += paddedRect.size.y * 0.1f;
    paddedRect.size.x *= 0.8f;
    paddedRect.size.y *= 0.8f;

    return paddedRect;
}