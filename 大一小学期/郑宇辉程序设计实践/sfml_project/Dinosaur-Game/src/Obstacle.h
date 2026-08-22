#pragma once

#include <SFML/Graphics.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>

class Obstacle {
private:
    friend class boost::serialization::access;

    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & obstacleEndRange;
        ar & obstacleY;
    }

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
