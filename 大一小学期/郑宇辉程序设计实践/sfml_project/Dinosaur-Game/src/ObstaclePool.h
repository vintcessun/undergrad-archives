#pragma once

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/vector.hpp>
#include <fileSystem>
#include <vector>

#include "Obstacle.h"
#include "sfml_serialization.hpp"


class ObstaclePool {
private:
    friend class boost::serialization::access;

    template <class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & obstacleDuplicationAmount;

        ar & obstacleList;

        ar & displayedObstaclesIndex;

        ar & obstacleSpeed;

        ar & obstacleDelayTime;
    }

public:
    ObstaclePool();
    void sendNextObstacle(float, float);
    bool obstacleUsed(int);
    void freeObstacleIndex(Obstacle &obstacle);
    std::vector<Obstacle *> sendObstacleList();

private:
    float obstacleDuplicationAmount = 2;

    std::array<Obstacle, 6> obstacleList;

    std::vector<int> displayedObstaclesIndex;

    sf::Vector2f obstacleSpeed;

    float obstacleDelayTime;
    sf::Clock obstacleClock;
};