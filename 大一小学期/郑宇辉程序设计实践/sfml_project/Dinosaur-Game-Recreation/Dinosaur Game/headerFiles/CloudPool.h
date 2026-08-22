#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <stdlib.h>

class CloudPool
{
public:
    CloudPool();
    std::vector<sf::Sprite *> sendCloudList();

    void sendNextCloud(float windowSizeX);
    bool cloudUsed(int randomIndex);
    void freeCloudIndex(sf::Sprite &cloud);

private:
    sf::Texture cloudTexture;
    std::vector<sf::Sprite> cloudPool;
    int cloudCapacity = 7;

    std::vector<int> displayedCloudIndex;

    int cloudSpawnRange[2] = {100, 30};

    sf::Clock cloudGenTimer;
    float cloudDelayTime;
};