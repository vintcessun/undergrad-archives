#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <iostream>

#include <SFML/Graphics.hpp>

class Obstacle
{
public:
    Obstacle(std::string);
    sf::Sprite getObstacle();

    void updatePosition(float xPos, float yPos);
    void moveObstacle(sf::Vector2f trackVelocity, sf::Time deltaTime);

    bool checkPosition();

    sf::FloatRect getPaddedGlobalBounds();

private:
private:
    sf::Texture obstacleTexture;
    sf::Sprite obstacle;

    float obstacleEndRange = -100.0f;

    float obstacleY;
};

#endif /* OBSTACLE_H */