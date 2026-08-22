#include "..\headerFiles\CloudPool.h"

CloudPool::CloudPool()
{
    if (!cloudTexture.loadFromFile("gameSprites\\BackgroundSprites\\cloudSprite.png"))
    {
        // Handle loading error
    }
    for (int i = 0; i < cloudCapacity; i++)
    {
        sf::Sprite tempCloud = *new sf::Sprite;
        tempCloud.setTexture(cloudTexture);
        tempCloud.scale(0.5f, 0.5f);
        cloudPool.push_back(tempCloud);
    }

    cloudDelayTime = rand() % 4 + 1.0f;
}

void CloudPool::sendNextCloud(float windowSizeX)
{
    if (displayedCloudIndex.size() == cloudPool.size())
    {
        // Safety net to stop game from crashing,
        // all clouds in pool are in use already.
        return;
    }

    if (cloudGenTimer.getElapsedTime().asSeconds() > cloudDelayTime)
    {
        // Reset time and randomizer
        cloudGenTimer.restart();
        cloudDelayTime = rand() % 4 + 1.5f;

        sf::Sprite *tempCloud;
        // Find unused cloud template;
        for (int i = 0; i < cloudPool.size(); i++)
        {
            if (!cloudUsed(i))
            {
                tempCloud = &cloudPool[i];
                float cloudY = rand() % cloudSpawnRange[0] + cloudSpawnRange[1];

                tempCloud->setPosition(windowSizeX, cloudY);
                displayedCloudIndex.push_back(i);
                break;
            }
        }
    }
}

bool CloudPool::cloudUsed(int randomIndex)
{
    if (displayedCloudIndex.empty())
    {
        return false;
    }

    for (int i = 0; i < displayedCloudIndex.size(); i++)
    {
        if (randomIndex == displayedCloudIndex[i])
        {
            return true;
        }
    }
    return false;
}

void CloudPool::freeCloudIndex(sf::Sprite &cloud)
{
    // Removes oldest cloud created
    for (int i = 0; i < displayedCloudIndex.size(); i++)
    {
        if (&cloud == &cloudPool[1])
        {
            displayedCloudIndex.erase(displayedCloudIndex.begin() + i);
            break;
        }
    }
}

std::vector<sf::Sprite *> CloudPool::sendCloudList()
{
    std::vector<sf::Sprite *> displayedClouds;

    for (int i = 0; i < displayedCloudIndex.size(); i++)
    {
        displayedClouds.push_back(&cloudPool[displayedCloudIndex[i]]);
    }

    return displayedClouds;
}