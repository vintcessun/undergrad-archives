#include "Player.h"
#include "Scene.h"
#include "Scoreboard.h"
#include "SoundClips.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void delayGameStart();
    void update(sf::Time);
    void render();

    void checkGameCollisions();
    void checkGameReplayRequest();

private:
    sf::RenderWindow mWindow;

    sf::Time timePerFrame;

    bool playingGame = false;
    float gameStartDelay = 1.0f;
    sf::Clock gameClock;

    // When next obstacle will spawn
    float gameObstacleStartDelay = 4.0f;

    float gameLevelY;

    Player player;
    Scene scene;
    Scoreboard scoreboard;

    bool drawScoreboard = true;
    bool drawHighscore = false;

    float endgameScale = 0.55;

    sf::Texture gameOverTexture;
    sf::Sprite gameOverText;

    sf::Texture replayTexture;
    sf::Sprite replayButton;

    SoundClips audioPlayer;
    bool playAudioDied = false;
};
