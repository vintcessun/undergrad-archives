#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>

#include "CloudPool.h"
#include "ObstaclePool.h"
#include "sfml_serialization.hpp"

class Scene {
private:
    friend class boost::serialization::access;

    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & trackLength;
        ar & createTrackOverlap;

        ar & windowSizeX;

        ar & backgroundPosY;

        ar & trackVelocity;
        ar & cloudVelocity;

        ar & obstaclePool;
        ar & cloudPool;
        ar & cloudEndRange;
    }

public:
    Scene(float windowSizeX, float gameLevelY);

    sf::Sprite drawTrack_1();
    sf::Sprite drawTrack_2();
    std::vector<sf::Sprite> drawObstacles();
    std::vector<sf::Sprite*> drawClouds();
    std::vector<sf::FloatRect> getObstacleBounds();
    void moveTrack(sf::Time deltaTime);

    void spawnObstacle(float);
    void moveObstacles(sf::Time deltaTime);
    void resetObstacles();

    void spawnCloud();
    void moveScenary(sf::Time deltaTime);
    void resetScenary();

    void clearAllObstacles();

private:
    void setUpTracks();
    void updateTrackPosition();

private:
    sf::Texture trackTexture;
    sf::Sprite track_1;
    sf::Sprite track_2;

    float trackLength;
    float createTrackOverlap = 10;

    float windowSizeX;

    float backgroundPosY;

    sf::Vector2f trackVelocity;
    sf::Vector2f cloudVelocity;

    ObstaclePool obstaclePool;
    CloudPool cloudPool;
    float cloudEndRange = -100.0f;
};