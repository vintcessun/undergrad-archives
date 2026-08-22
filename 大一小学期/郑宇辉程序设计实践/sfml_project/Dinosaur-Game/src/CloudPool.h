#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

class CloudPool {
private:
    friend class boost::serialization::access;

    template <class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & cloudCapacity;
        ar & displayedCloudIndex;
        ar & cloudSpawnRange;
        ar & cloudDelayTime;
    }

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