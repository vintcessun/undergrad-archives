#include "ObstaclePool.h"

ObstaclePool::ObstaclePool()
    : obstacleList{Obstacle("resources/backgroundCactusSprite_1.png"),
                   Obstacle("resources/backgroundCactusSprite_2.png"),
                   Obstacle("resources/backgroundCactusSprite_3.png"),
                   Obstacle("resources/backgroundCactusSprite_4.png"),
                   Obstacle("resources/backgroundCactusSprite_5.png"),
                   Obstacle("resources/backgroundCactusSprite_6.png")} {
    obstacleDelayTime = rand() % 4 + 0.5f;
}

void ObstaclePool::sendNextObstacle(float windowSizeX,
                                    float backgroundObjectPositions) {
    if (displayedObstaclesIndex.size() == obstacleList.size()) {
        return;
    }

    if (obstacleClock.getElapsedTime().asSeconds() > obstacleDelayTime) {
        obstacleClock.restart();
        obstacleDelayTime = rand() % 4 + 0.5f;

        int randomObstacle;

        do {
            randomObstacle = rand() % obstacleList.size();
        } while (obstacleUsed(randomObstacle));

        Obstacle *tempObstacle = &obstacleList[randomObstacle];
        tempObstacle->updatePosition(windowSizeX, backgroundObjectPositions);

        displayedObstaclesIndex.push_back(randomObstacle);
    }
}

bool ObstaclePool::obstacleUsed(int randomIndex) {
    if (displayedObstaclesIndex.empty()) {
        return false;
    }

    for (int i = 0; i < displayedObstaclesIndex.size(); i++) {
        if (randomIndex == displayedObstaclesIndex[i]) {
            return true;
        }
    }
    return false;
}

void ObstaclePool::freeObstacleIndex(Obstacle &obstacle) {
    for (int i = 0; i < displayedObstaclesIndex.size(); i++) {
        if (&obstacle == &obstacleList[1]) {
            displayedObstaclesIndex.erase(displayedObstaclesIndex.begin() + i);
            break;
        }
    }
}

std::vector<Obstacle *> ObstaclePool::sendObstacleList() {
    std::vector<Obstacle *> displayedObstacles;

    for (int i = 0; i < displayedObstaclesIndex.size(); i++) {
        displayedObstacles.push_back(&obstacleList[displayedObstaclesIndex[i]]);
    }

    return displayedObstacles;
}