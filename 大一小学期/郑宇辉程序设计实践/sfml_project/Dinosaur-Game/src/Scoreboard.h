#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>

#include "SoundClips.h"

class Scoreboard {
private:
    friend class boost::serialization::access;

    template <class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & score;
        ar & scoreDigitAmount;
        ar & scoreDigits;

        ar & padding;

        ar & highscoreValue;
        ar & highscoreTime;

        ar & scoreboardBlink;
        ar & resetTimeMS;

        ar & blinkTimeMS;
    }

public:
    Scoreboard(float windowSizeX);
    void createScoreboard(sf::Texture *symbols,
                          std::vector<sf::Sprite> *scoreboardPart, int spacing,
                          int symbolAmount, float windowSizeX, sf::Color color);
    std::vector<sf::Sprite> drawScoreboard();
    std::vector<sf::Sprite> drawHighscore();
    std::vector<sf::Sprite> drawHighscoreTitle();
    bool updatePlayerScore(SoundClips *audio);
    void updateHighscore();
    void resetScoreboard();
    int getHighscoreValue();
    std::string &getHighscoreTime();

private:
    void updateScoreboard(std::vector<sf::Sprite> *scoreboard);
    void convertIntToArray(int curScore);
    void updateSaveFile(int value, std::string &t);

private:
    int score = 0;
    int scoreDigitAmount = 5;
    int scoreDigits[5] = {0, 0, 0, 0, 0};

    float padding = 15.0f;

    sf::Texture symbolTexture;

    std::vector<sf::Sprite> currentScore;

    int highscoreValue = 0;
    std::string highscoreTime;
    std::vector<sf::Sprite> highScore;
    std::vector<sf::Sprite> highScoreTitle;

    int scoreboardBlink = 0;
    float resetTimeMS = 100.0f;
    sf::Clock scoreClock;

    float blinkTimeMS = 350.0f;
    sf::Clock blinkClock;
};