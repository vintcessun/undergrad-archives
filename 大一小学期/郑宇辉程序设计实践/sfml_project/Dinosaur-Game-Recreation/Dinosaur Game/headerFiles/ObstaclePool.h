#include <vector>
#include <queue>
#include <stdlib.h>
#include <experimental/fileSystem>

#include "Obstacle.h"

class ObstaclePool
{
public:
    ObstaclePool();
    void sendNextObstacle(float, float);
    bool obstacleUsed(int);
    void freeObstacleIndex(Obstacle &obstacle);
    std::vector<Obstacle *> sendObstacleList();

private:
private:
    float obstacleDuplicationAmount = 2;

    std::vector<Obstacle> obstacleList;

    std::vector<int> displayedObstaclesIndex;

    sf::Vector2f obstacleSpeed;

    float obstacleDelayTime;
    sf::Clock obstacleClock; // 0.5 sec - 2 sec
};