#include "..\headerFiles\Game.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

Game::Game()
    : mWindow(sf::VideoMode(740, 250), "Dinosaur Game recreated by RedDogClifford"),
      timePerFrame(sf::seconds(1.f / 60.f)),
      scoreboard(mWindow.getSize().x),
      gameLevelY(mWindow.getSize().y - 50),
      player(gameLevelY),
      scene(mWindow.getSize().x, gameLevelY)
{
    float centerOfScreenX = mWindow.getSize().x / 2;
    float centerOfScreenY = mWindow.getSize().y / 2;

    if (!gameOverTexture.loadFromFile("gameSprites\\SymbolSprites\\gameOverSymbolSprite.png"))
    {
        // Handle loading error
    }
    gameOverText.setTexture(gameOverTexture);
    gameOverText.setScale(endgameScale, endgameScale);
    gameOverText.setPosition(centerOfScreenX - (gameOverTexture.getSize().x * endgameScale / 2),
                             centerOfScreenY - (gameOverTexture.getSize().y * endgameScale / 2) - 55);

    if (!replayTexture.loadFromFile("gameSprites\\SymbolSprites\\replaySymbolSprite.png"))
    {
        // Handle loading error
    }
    replayButton.setTexture(replayTexture);
    replayButton.setScale(endgameScale, endgameScale);
    replayButton.setPosition(centerOfScreenX - (replayTexture.getSize().x * endgameScale / 2),
                             centerOfScreenY - (replayTexture.getSize().y * endgameScale / 2));
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen())
    {
        processEvents();
        // Fixed time steps
        /*
        Guarentees that in any circumstances, we always give the same delta time to the
        update function, no matter what happens.
        */
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;
            processEvents();
            update(timePerFrame);
        }
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::KeyPressed:
            player.handlePlayerInput(event.key.code, true);

            if ((event.key.code == sf::Keyboard::Space ||
                 event.key.code == sf::Keyboard::Up) &&
                !playingGame)
            {
                playingGame = true;
                gameClock.restart();
            }
            break;
        case sf::Event::KeyReleased:
            player.handlePlayerInput(event.key.code, false);
            break;
        case sf::Event::MouseButtonPressed:
            checkGameReplayRequest();
            break;
        case sf::Event::Closed:
            mWindow.close();
            break;
        }
    }
}

// Player position cant be below 300.f
// constant gravity
void Game::update(sf::Time deltaTime)
{
    if (player.isPlayerAlive())
    {
        player.handlePlayerMovement(deltaTime, &audioPlayer);

        if (playingGame && gameClock.getElapsedTime().asSeconds() > gameStartDelay)
        {
            scene.moveTrack(deltaTime);

            drawScoreboard = scoreboard.updatePlayerScore(&audioPlayer);
            scoreboard.updateHighscore();

            scene.spawnCloud();
            scene.moveScenary(deltaTime);
            scene.resetScenary();

            if (gameClock.getElapsedTime().asSeconds() > gameObstacleStartDelay)
            {
                scene.spawnObstacle(gameLevelY);
                scene.moveObstacles(deltaTime);
                scene.resetObstacles();
            }

            checkGameCollisions();
            player.movementAnimation();
        }
    }
    else if (!player.isPlayerAlive())
    {
        // Player has lost
        drawHighscore = true;
        drawScoreboard = true;
    }
}

void Game::checkGameCollisions()
{
    for (auto obstacles : scene.getObstacleBounds())
    {
        if (player.getPaddedGlobalBounds().intersects(obstacles))
        {
            player.playerStatus(false);
            audioPlayer.playDeathSound();
            break;
        }
    }
}

void Game::checkGameReplayRequest()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(mWindow);

        float replayButtonWidth = replayButton.getPosition().x +
                                  replayButton.getTexture()->getSize().x * endgameScale;

        float replayButtonHeight = replayButton.getPosition().y +
                                   replayButton.getTexture()->getSize().y * endgameScale;

        if (mousePosition.x >= replayButton.getPosition().x && mousePosition.x <= replayButtonWidth &&
            mousePosition.y >= replayButton.getPosition().y && mousePosition.y <= replayButtonHeight)
        {
            // restart game
            scoreboard.resetScoreboard();
            scene.clearAllObstacles();
            player.resetPlayer();
            playingGame = false;
        }
    }
    // mousePosition.y >= replayButton.getPosition().y && mousePosition.y <= replayButton.getPosition().y
}

void Game::render()
{
    mWindow.clear(sf::Color(255, 255, 255));
    mWindow.draw(scene.drawTrack_1());
    mWindow.draw(scene.drawTrack_2());
    for (auto cloud : scene.drawClouds())
    {
        mWindow.draw(*cloud);
    }
    if (drawScoreboard)
    {
        for (auto score : scoreboard.drawScoreboard())
        {
            mWindow.draw(score);
        }
    }
    if (drawHighscore)
    {
        for (auto highscore : scoreboard.drawHighscore())
        {
            mWindow.draw(highscore);
        }
        for (auto highscoreTitle : scoreboard.drawHighscoreTitle())
        {
            mWindow.draw(highscoreTitle);
        }
    }
    for (auto obstacle : scene.drawObstacles())
    {
        mWindow.draw(obstacle);
    }
    if (!player.isPlayerAlive())
    {
        mWindow.draw(gameOverText);
        mWindow.draw(replayButton);
    }
    mWindow.draw(player.drawPlayer());
    mWindow.display();
}