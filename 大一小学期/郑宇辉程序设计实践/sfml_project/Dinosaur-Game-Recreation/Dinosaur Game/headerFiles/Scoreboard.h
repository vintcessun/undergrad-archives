#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "SoundClips.h"

class Scoreboard
{
public:
    Scoreboard(float windowSizeX);
    void createScoreboard(sf::Texture *symbols,
                          std::vector<sf::Sprite> *scoreboardPart,
                          int spacing,
                          int symbolAmount,
                          float windowSizeX,
                          sf::Color color);
    std::vector<sf::Sprite> drawScoreboard();
    std::vector<sf::Sprite> drawHighscore();
    std::vector<sf::Sprite> drawHighscoreTitle();
    bool updatePlayerScore(SoundClips *audio);
    void updateHighscore();
    void resetScoreboard();

private:
    void updateScoreboard(std::vector<sf::Sprite> *scoreboard);
    void convertIntToArray(int curScore);

private:
    int score = 0;
    //[0] : ones, [1] : tens ...
    int scoreDigitAmount = 5;
    int scoreDigits[5] = {0, 0, 0, 0, 0};

    float padding = 15.0f;

    sf::Texture symbolTexture;

    std::vector<sf::Sprite> currentScore;

    int highscoreValue = 0;
    std::vector<sf::Sprite> highScore;
    std::vector<sf::Sprite> highScoreTitle;

    int scoreboardBlink = 0;
    float resetTimeMS = 100.0f;
    sf::Clock scoreClock;

    float blinkTimeMS = 350.0f;
    sf::Clock blinkClock;
};