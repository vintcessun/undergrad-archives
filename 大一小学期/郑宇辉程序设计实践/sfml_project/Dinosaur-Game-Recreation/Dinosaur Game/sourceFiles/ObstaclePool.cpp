#include "..\headerFiles\ObstaclePool.h"

namespace fs = std::experimental::filesystem;

ObstaclePool::ObstaclePool()
{
    const fs::directory_iterator end{};

    // Amount of times sprites are duplicated in ObstacleSprites folder.
    for (int i = 0; i < obstacleDuplicationAmount; i++)
    {
        // Create obstacle object for each sprite in ObstacleSprites folder.
        for (fs::directory_iterator iter{"gameSprites\\ObstacleSprites"}; iter != end; ++iter)
        {
            if (fs::is_regular_file(*iter))
            {
                obstacleList.push_back(*new Obstacle(iter->path().string()));
            }
        }
    }

    // std::string pathname = "gameSprites\\ObstacleSprites\\backgroundCactusSprite_1.png";
    // obstacleList.push_back(*new Obstacle(pathname));

    // spawn rate is 0.5s to 3s
    obstacleDelayTime = rand() % 4 + 0.5f;
}

void ObstaclePool::sendNextObstacle(float windowSizeX, float backgroundObjectPositions)
{
    if (displayedObstaclesIndex.size() == obstacleList.size())
    {
        // Safety net to stop game from crashing,
        // all obstacles in pool are in use already.
        return;
    }

    if (obstacleClock.getElapsedTime().asSeconds() > obstacleDelayTime)
    {
        // Reset time and randomizer
        obstacleClock.restart();
        obstacleDelayTime = rand() % 4 + 0.5f;

        // Push obstacle to queue
        int randomObstacle;

        do
        {
            randomObstacle = rand() % obstacleList.size();
        } while (obstacleUsed(randomObstacle));

        Obstacle *tempObstacle = &obstacleList[randomObstacle];
        tempObstacle->updatePosition(windowSizeX, backgroundObjectPositions);

        displayedObstaclesIndex.push_back(randomObstacle);
    }
}

bool ObstaclePool::obstacleUsed(int randomIndex)
{
    if (displayedObstaclesIndex.empty())
    {
        return false;
    }

    for (int i = 0; i < displayedObstaclesIndex.size(); i++)
    {
        if (randomIndex == displayedObstaclesIndex[i])
        {
            return true;
        }
    }
    return false;
}

void ObstaclePool::freeObstacleIndex(Obstacle &obstacle)
{
    // Removes oldest obstacle created
    for (int i = 0; i < displayedObstaclesIndex.size(); i++)
    {
        if (&obstacle == &obstacleList[1])
        {
            displayedObstaclesIndex.erase(displayedObstaclesIndex.begin() + i);
            break;
        }
    }
}

std::vector<Obstacle *> ObstaclePool::sendObstacleList()
{
    std::vector<Obstacle *> displayedObstacles;

    for (int i = 0; i < displayedObstaclesIndex.size(); i++)
    {
        displayedObstacles.push_back(&obstacleList[displayedObstaclesIndex[i]]);
    }

    return displayedObstacles;
}