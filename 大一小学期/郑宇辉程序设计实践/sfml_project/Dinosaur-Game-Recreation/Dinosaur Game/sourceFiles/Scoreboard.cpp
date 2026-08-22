#include "..\headerFiles\Scoreboard.h"

Scoreboard::Scoreboard(float windowSizeX)
{
    if (!symbolTexture.loadFromFile("gameSprites\\SymbolSprites\\symbolSprites.png"))
    {
        // Handle loading error
    }

    // Creates current score scoreboard
    createScoreboard(&symbolTexture, &currentScore, 1, 5, windowSizeX, sf::Color(230, 230, 230));
    // Creates highscore scoreboard
    createScoreboard(&symbolTexture, &highScore, 7, 5, windowSizeX, sf::Color(230, 230, 230, 200));
    // Creates highscore title
    createScoreboard(&symbolTexture, &highScoreTitle, 13, 2, windowSizeX, sf::Color(230, 230, 230, 200));

    // Set textures for highscore titles
    sf::Vector2u textureSize = symbolTexture.getSize();
    textureSize.x /= 12;
    highScoreTitle[0].setTextureRect(sf::IntRect(textureSize.x * 11, textureSize.y * 0, textureSize.x, textureSize.y));
    highScoreTitle[1].setTextureRect(sf::IntRect(textureSize.x * 10, textureSize.y * 0, textureSize.x, textureSize.y));
}

void Scoreboard::createScoreboard(sf::Texture *symbolsText, std::vector<sf::Sprite> *scoreboardPart,
                                  int spacing, int symbolAmount, float windowSizeX, sf::Color color)
{
    // Current score sprites
    for (int i = 0; i < symbolAmount; i++)
    {
        sf::Sprite scoreSymbol = *new sf::Sprite();
        scoreSymbol.setTexture(*symbolsText);

        sf::Vector2u textureSize = symbolsText->getSize();
        textureSize.x /= 12;

        // Default sets all symbols to 0.
        scoreSymbol.setTextureRect(sf::IntRect(textureSize.x * 0, textureSize.y * 0, textureSize.x, textureSize.y));
        scoreSymbol.scale(0.7, 0.7);
        scoreSymbol.setColor(color);
        scoreSymbol.setPosition(windowSizeX - (textureSize.x * (i + spacing)) * 0.7 - padding,
                                symbolsText->getSize().y * 0.7);
        scoreboardPart->push_back(scoreSymbol);
    }
}

// Must call converIntToArray first to update scoreDigits being used.
void Scoreboard::updateScoreboard(std::vector<sf::Sprite> *scoreboard)
{
    sf::Vector2u textureSize = symbolTexture.getSize();
    textureSize.x /= 12;

    for (int i = 0; i < scoreDigitAmount; i++)
    {
        (*scoreboard)[i].setTextureRect(sf::IntRect(textureSize.x * scoreDigits[i], textureSize.y * 0,
                                                    textureSize.x, textureSize.y));
    }
}

void Scoreboard::convertIntToArray(int curScore)
{
    for (int i = 0; i < scoreDigitAmount; i++)
    {
        scoreDigits[i] = curScore % 10;
        curScore /= 10;
    }
}

std::vector<sf::Sprite> Scoreboard::drawScoreboard()
{
    return currentScore;
}

std::vector<sf::Sprite> Scoreboard::drawHighscore()
{
    return highScore;
}

std::vector<sf::Sprite> Scoreboard::drawHighscoreTitle()
{
    return highScoreTitle;
}

bool Scoreboard::updatePlayerScore(SoundClips *audio)
{
    // 10 points every ~2 sec
    if (scoreClock.getElapsedTime().asMilliseconds() > resetTimeMS)
    {
        score++;
        convertIntToArray(score);
        updateScoreboard(&currentScore);
        scoreClock.restart();
    }

    if (scoreboardBlink > 0)
    {
        if (blinkClock.getElapsedTime().asMilliseconds() < blinkTimeMS)
        {
            return false;
        }
        if (blinkClock.getElapsedTime().asMilliseconds() < blinkTimeMS * 1.5)
        {
            return true;
        }
        scoreboardBlink--;
        blinkClock.restart();
    }

    if (score % 100 == 0)
    {
        audio->playPointSound();
        scoreboardBlink = 4;
    }
    return true;
}

void Scoreboard::updateHighscore()
{
    if (highscoreValue < score)
    {
        highscoreValue = score;
        convertIntToArray(highscoreValue);
        updateScoreboard(&highScore);
    }
}

void Scoreboard::resetScoreboard()
{
    scoreboardBlink = 0;
    score = 0;
    for (int i = 0; i < scoreDigitAmount; i++)
    {
        scoreDigits[i] = 0;
    }
    convertIntToArray(score);
    updateScoreboard(&currentScore);
}