#include "Scene.h"

Scene::Scene(float windowSizeX, float gameLevelY)
    : trackTexture("resources/backgroundtrack.png"),
      track_1(trackTexture),
      track_2(trackTexture),
      trackLength(),
      trackVelocity(-400.0f, 0),
      cloudVelocity(-80.0f, 0),
      windowSizeX(windowSizeX),
      backgroundPosY(gameLevelY - 25) {
    setUpTracks();
}

void Scene::setUpTracks() {
    trackLength = track_1.getTextureRect().size.x;

    track_1.setPosition({0.0f, backgroundPosY});
    track_2.setPosition({trackLength - createTrackOverlap, backgroundPosY});
}

sf::Sprite Scene::drawTrack_1() { return track_1; }

sf::Sprite Scene::drawTrack_2() { return track_2; }

std::vector<sf::Sprite> Scene::drawObstacles() {
    std::vector<sf::Sprite> obstacleSprites;
    for (auto obstacles : obstaclePool.sendObstacleList()) {
        obstacleSprites.push_back(obstacles->getObstacle());
    }
    return obstacleSprites;
}

std::vector<sf::Sprite *> Scene::drawClouds() {
    return cloudPool.sendCloudList();
}

std::vector<sf::FloatRect> Scene::getObstacleBounds() {
    std::vector<sf::FloatRect> obstacleRects;
    for (auto obstacles : obstaclePool.sendObstacleList()) {
        obstacleRects.push_back(obstacles->getPaddedGlobalBounds());
    }
    return obstacleRects;
}

void Scene::moveTrack(sf::Time deltaTime) {
    track_1.move(trackVelocity * deltaTime.asSeconds());
    track_2.move(trackVelocity * deltaTime.asSeconds());

    updateTrackPosition();
}

void Scene::spawnCloud() { cloudPool.sendNextCloud(windowSizeX); }

void Scene::spawnObstacle(float gameLevelY) {
    obstaclePool.sendNextObstacle(windowSizeX, gameLevelY);
}

void Scene::moveObstacles(sf::Time deltaTime) {
    for (auto obstacle : obstaclePool.sendObstacleList()) {
        obstacle->moveObstacle(trackVelocity, deltaTime);
    }
}

void Scene::moveScenary(sf::Time deltaTime) {
    for (auto cloud : cloudPool.sendCloudList()) {
        cloud->move(cloudVelocity * deltaTime.asSeconds());
    }
}

void Scene::resetObstacles() {
    for (auto obstacle : obstaclePool.sendObstacleList()) {
        if (obstacle->checkPosition()) {
            obstaclePool.freeObstacleIndex(*obstacle);
        }
    }
}

void Scene::resetScenary() {
    for (auto cloud : cloudPool.sendCloudList()) {
        if (cloud->getPosition().x < cloudEndRange) {
            cloud->move(sf::Vector2f(0.0f, 0.0f));
            cloud->setPosition({-150.0f, -100.0f});

            cloudPool.freeCloudIndex(*cloud);
        }
    }
}

void Scene::updateTrackPosition() {
    if (track_1.getPosition().x < -trackLength) {
        track_1.setPosition(
            {track_2.getPosition().x + trackLength - createTrackOverlap,
             backgroundPosY});
    }
    if (track_2.getPosition().x < -trackLength) {
        track_2.setPosition(
            {track_1.getPosition().x + trackLength - createTrackOverlap,
             backgroundPosY});
    }
}

void Scene::clearAllObstacles() {
    for (auto obstacle : obstaclePool.sendObstacleList()) {
        obstacle->moveObstacle(sf::Vector2f(0.0f, 0.0f), sf::Time());
        obstacle->updatePosition(-100.0f, -100.0f);

        obstaclePool.freeObstacleIndex(*obstacle);
    }
}