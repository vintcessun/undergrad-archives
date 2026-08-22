#include "Scoreboard.h"

#include <chrono>
#include <fstream>

Scoreboard::Scoreboard(float windowSizeX) {
    if (!symbolTexture.loadFromFile("resources/symbolSprites.png")) {
    }

    createScoreboard(&symbolTexture, &currentScore, 1, 5, windowSizeX,
                     sf::Color(230, 230, 230));
    createScoreboard(&symbolTexture, &highScore, 7, 5, windowSizeX,
                     sf::Color(230, 230, 230, 200));
    createScoreboard(&symbolTexture, &highScoreTitle, 13, 2, windowSizeX,
                     sf::Color(230, 230, 230, 200));

    sf::Vector2u textureSize = symbolTexture.getSize();
    textureSize.x /= 12;
    auto a = sf::IntRect();
    a.position.x = textureSize.x * 11;
    a.position.y = textureSize.y * 0;
    a.size.x = textureSize.x;
    a.size.y = textureSize.y;
    highScoreTitle[0].setTextureRect(a);
    auto b = sf::IntRect();
    b.position.x = textureSize.x * 10;
    b.position.y = textureSize.y * 0;
    b.size.x = textureSize.x;
    b.size.y = textureSize.y;
    highScoreTitle[1].setTextureRect(b);

    std::ifstream file("resources/save.txt");
    if (file.is_open()) {
        file >> highscoreValue >> highscoreTime;
        file.close();
    }

    convertIntToArray(highscoreValue);
    updateScoreboard(&highScore);
}

void Scoreboard::createScoreboard(sf::Texture *symbolsText,
                                  std::vector<sf::Sprite> *scoreboardPart,
                                  int spacing, int symbolAmount,
                                  float windowSizeX, sf::Color color) {
    for (int i = 0; i < symbolAmount; i++) {
        sf::Sprite scoreSymbol(*symbolsText);

        sf::Vector2u textureSize = symbolsText->getSize();
        textureSize.x /= 12;

        auto a = sf::IntRect();
        a.position.x = textureSize.x * 0;
        a.position.y = textureSize.y * 0;
        a.size.x = textureSize.x;
        a.size.y = textureSize.y;
        scoreSymbol.setTextureRect(a);
        scoreSymbol.scale({0.7, 0.7});
        scoreSymbol.setColor(color);
        scoreSymbol.setPosition(
            {windowSizeX - (textureSize.x * (i + spacing)) * 0.7f - padding,
             symbolsText->getSize().y * 0.7f});
        scoreboardPart->push_back(scoreSymbol);
    }
}

void Scoreboard::updateScoreboard(std::vector<sf::Sprite> *scoreboard) {
    sf::Vector2u textureSize = symbolTexture.getSize();
    textureSize.x /= 12;

    for (int i = 0; i < scoreDigitAmount; i++) {
        auto a = sf::IntRect();
        a.position.x = textureSize.x * scoreDigits[i];
        a.position.y = textureSize.y * 0;
        a.size.x = textureSize.x;
        a.size.y = textureSize.y;
        (*scoreboard)[i].setTextureRect(a);
    }
}

void Scoreboard::convertIntToArray(int curScore) {
    for (int i = 0; i < scoreDigitAmount; i++) {
        scoreDigits[i] = curScore % 10;
        curScore /= 10;
    }
}

std::vector<sf::Sprite> Scoreboard::drawScoreboard() { return currentScore; }

std::vector<sf::Sprite> Scoreboard::drawHighscore() { return highScore; }

std::vector<sf::Sprite> Scoreboard::drawHighscoreTitle() {
    return highScoreTitle;
}

bool Scoreboard::updatePlayerScore(SoundClips *audio) {
    if (scoreClock.getElapsedTime().asMilliseconds() > resetTimeMS) {
        score++;
        convertIntToArray(score);
        updateScoreboard(&currentScore);
        scoreClock.restart();
    }

    if (scoreboardBlink > 0) {
        if (blinkClock.getElapsedTime().asMilliseconds() < blinkTimeMS) {
            return false;
        }
        if (blinkClock.getElapsedTime().asMilliseconds() < blinkTimeMS * 1.5) {
            return true;
        }
        scoreboardBlink--;
        blinkClock.restart();
    }

    if (score % 100 == 0) {
        audio->playPointSound();
        scoreboardBlink = 4;
    }
    return true;
}

void Scoreboard::updateHighscore() {
    if (highscoreValue < score) {
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        struct tm *localTime = std::localtime(&currentTime);

        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%y-%m-%d-%H:%M:%S", localTime);
        highscoreTime = buffer;
        highscoreValue = score;

        convertIntToArray(highscoreValue);
        updateScoreboard(&highScore);
        updateSaveFile(highscoreValue, highscoreTime);
    }
}

void Scoreboard::updateSaveFile(int value, std::string &t) {
    std::fstream file("resources/save.txt", std::ios::out | std::ios::trunc);
    if (file.is_open()) {
        file << value << ' ' << t;
        file.close();
    }
}

int Scoreboard::getHighscoreValue() { return highscoreValue; }

std::string &Scoreboard::getHighscoreTime() { return highscoreTime; }

void Scoreboard::resetScoreboard() {
    scoreboardBlink = 0;
    score = 0;
    for (int i = 0; i < scoreDigitAmount; i++) {
        scoreDigits[i] = 0;
    }
    convertIntToArray(score);
    updateScoreboard(&currentScore);
}