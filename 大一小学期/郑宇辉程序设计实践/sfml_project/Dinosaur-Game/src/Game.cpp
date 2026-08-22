#include "Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <boost/serialization/split_free.hpp>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "MessageBox.h"
#include "SFML/Window/Keyboard.hpp"

Game::Game()
	: mWindow(sf::VideoMode({740, 250}), "Dinosaur Game"),
	  timePerFrame(sf::seconds(1.f / 60.f)),
	  scoreboard(mWindow.getSize().x),
	  gameLevelY(mWindow.getSize().y - 50),
	  player(gameLevelY),
	  scene(mWindow.getSize().x, gameLevelY),
	  gameOverTexture("resources/gameOverSymbolSprite.png"),
	  gameOverText(gameOverTexture),
	  replayTexture("resources/replaySymbolSprite.png"),
	  replayButton(replayTexture),
	  startTexture("resources/start.png"),
	  startMenu(startTexture),
	  MenuState(0),
	  helpTexture("resources/help.png"),
	  helpMenu(helpTexture),
	  TextFont("resources/arial.ttf"),
	  pauseTexture("resources/pause.png"),
	  pauseMenu(pauseTexture),
	  saveTexture("resources/save.png"),
	  saveMenu(saveTexture) {
	float centerOfScreenX = mWindow.getSize().x / 2.0f;
	float centerOfScreenY = mWindow.getSize().y / 2.0f;
	gameOverText.setScale({endgameScale, endgameScale});
	gameOverText.setPosition(
		{centerOfScreenX - (gameOverTexture.getSize().x * endgameScale / 2),
		 centerOfScreenY - (gameOverTexture.getSize().y * endgameScale / 2) -
			 55});

	replayButton.setScale({endgameScale, endgameScale});
	replayButton.setPosition(
		{centerOfScreenX - (replayTexture.getSize().x * endgameScale / 2),
		 centerOfScreenY - (replayTexture.getSize().y * endgameScale / 2)});
}

void Game::startGame() {
	sf::Texture texture("resources/pre.png");
	sf::Sprite sprite(texture);
	sf::Clock clock;
	sf::Time startTime = clock.getElapsedTime();

	while (mWindow.isOpen() &&
		   clock.getElapsedTime().asSeconds() - startTime.asSeconds() < 3.0f) {
		const auto e = mWindow.pollEvent();
		if (e.has_value()) {
			auto event = e.value();
			if (event.is<sf::Event::Closed>()) mWindow.close();
		}

		mWindow.clear();

		mWindow.draw(sprite);

		mWindow.display();
	}
}

void Game::run() {
	startGame();
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen()) {
		processEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame) {
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
			update(timePerFrame);
		}
		render();
	}
}

void Game::processEvents() {
	const auto e = mWindow.pollEvent();
	if (e.has_value()) {
		auto event = e.value();
		if (const auto key = event.getIf<sf::Event::KeyPressed>()) {
			player.handlePlayerInput(key->code, true);

			if ((key->code == sf::Keyboard::Key::Space ||
				 key->code == sf::Keyboard::Key::Up) &&
				!playingGame) {
				playingGame = true;
				gameClock.start();
				player.startPlayer();
			}

			if ((key->code == sf::Keyboard::Key::Escape) && !playingGame) {
				MenuState = 0;
			}

			if ((key->code == sf::Keyboard::Key::H) && !playingGame) {
				MenuState = 1;
			}

			if ((key->code == sf::Keyboard::Key::S) && !playingGame) {
				MenuState = 2;
			}

			if ((key->code == sf::Keyboard::Key::P) && playingGame &&
				player.isPlayerAlive()) {
				pauseGame();
			}

			if ((key->code == sf::Keyboard::Key::Escape) && playingGame) {
				restartGame();
			}

			if (key->code == sf::Keyboard::Key::L) {
				loadFromFile("resources/save.dat");
			}

			if ((key->code == sf::Keyboard::Key::J) && playingGame &&
				player.isPlayerAlive()) {
				pauseGame();
				saveToFile("resources/save.dat");
				MessageBox msgBox(
					"Hint",
					"The state has been saved at \"resources/save.dat\"");
				msgBox.show();
			}
		}
		if (const auto key = event.getIf<sf::Event::KeyReleased>()) {
			player.handlePlayerInput(key->code, false);
		}
		if (event.is<sf::Event::MouseButtonPressed>()) {
			checkGameReplayRequest();
		}
		if (event.is<sf::Event::Closed>()) {
			mWindow.close();
		}
	}
}

void Game::update(sf::Time deltaTime) {
	if (player.isPlayerAlive() && playingGame) {
		player.handlePlayerMovement(deltaTime, &audioPlayer);

		if (playingGame &&
			gameClock.getElapsedTime().asSeconds() > gameStartDelay) {
			scene.moveTrack(deltaTime);

			drawScoreboard = scoreboard.updatePlayerScore(&audioPlayer);
			scoreboard.updateHighscore();

			scene.spawnCloud();
			scene.moveScenary(deltaTime);
			scene.resetScenary();

			if (gameClock.getElapsedTime().asSeconds() >
				gameObstacleStartDelay) {
				scene.spawnObstacle(gameLevelY);
				scene.moveObstacles(deltaTime);
				scene.resetObstacles();
			}

			checkGameCollisions();
			player.movementAnimation();
		}
	} else if (!player.isPlayerAlive()) {
		drawHighscore = true;
		drawScoreboard = true;
	}
}

void Game::checkGameCollisions() {
	for (auto obstacles : scene.getObstacleBounds()) {
		if (player.getPaddedGlobalBounds().findIntersection(obstacles)) {
			player.playerStatus(false);
			audioPlayer.playDeathSound();
			break;
		}
	}
}

void Game::checkGameReplayRequest() {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		sf::Vector2i mousePosition = sf::Mouse::getPosition(mWindow);

		float replayButtonWidth =
			replayButton.getPosition().x +
			replayButton.getTexture().getSize().x * endgameScale;

		float replayButtonHeight =
			replayButton.getPosition().y +
			replayButton.getTexture().getSize().y * endgameScale;

		if (mousePosition.x >= replayButton.getPosition().x &&
			mousePosition.x <= replayButtonWidth &&
			mousePosition.y >= replayButton.getPosition().y &&
			mousePosition.y <= replayButtonHeight) {
			restartGame();
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
		restartGame();
	}
}

void Game::restartGame() {
	scoreboard.resetScoreboard();
	scene.clearAllObstacles();
	player.resetPlayer();
	playingGame = false;
}

void Game::render() {
	mWindow.clear(sf::Color(255, 255, 255));
	mWindow.draw(scene.drawTrack_1());
	mWindow.draw(scene.drawTrack_2());
	for (auto cloud : scene.drawClouds()) {
		mWindow.draw(*cloud);
	}
	if (drawScoreboard) {
		for (auto score : scoreboard.drawScoreboard()) {
			mWindow.draw(score);
		}
	}
	if (drawHighscore) {
		for (auto highscore : scoreboard.drawHighscore()) {
			mWindow.draw(highscore);
		}
		for (auto highscoreTitle : scoreboard.drawHighscoreTitle()) {
			mWindow.draw(highscoreTitle);
		}
	}
	for (auto obstacle : scene.drawObstacles()) {
		mWindow.draw(obstacle);
	}
	if (!player.isPlayerAlive()) {
		mWindow.draw(gameOverText);
		mWindow.draw(replayButton);
	}
	mWindow.draw(player.drawPlayer());
	if (!playingGame) {
		if (MenuState == 0) {
			mWindow.draw(startMenu);
			if (std::filesystem::exists("resources/save.dat")) {
				mWindow.draw(saveMenu);
			}
		} else if (MenuState == 1) {
			mWindow.draw(helpMenu);
		} else if (MenuState == 2) {
			sf::Text text(TextFont);
			text.setString("The highest Record\n" +
						   std::to_string(scoreboard.getHighscoreValue()) +
						   " At " + scoreboard.getHighscoreTime());
			text.setCharacterSize(24);
			text.setFillColor(sf::Color::Black);
			text.setPosition({150.0f, 50.0f});
			mWindow.draw(text);
		} else if (MenuState == 3) {
			mWindow.draw(pauseMenu);
		}
	}
	mWindow.display();
}

bool Game::saveToFile(const std::string &filename) {
	try {
		std::ofstream file(filename, std::ios::binary);
		if (!file.is_open()) {
			std::cerr << "Failed to open file for writing: " << filename
					  << std::endl;
			return false;
		}
		boost::archive::text_oarchive oa(file);
		oa << *this;
		return true;
	} catch (const std::exception &e) {
		std::cerr << "Serialization error: " << e.what() << std::endl;
		return false;
	}
}

bool Game::loadFromFile(const std::string &filename) {
	scene.clearAllObstacles();
	player.resetPlayer();
	playingGame = false;
	try {
		std::ifstream file(filename, std::ios::binary);
		if (!file.is_open()) {
			std::cerr << "Failed to open file for reading: " << filename
					  << std::endl;
			return false;
		}
		boost::archive::text_iarchive ia(file);
		ia >> *this;
		return true;
	} catch (const std::exception &e) {
		std::cerr << "Deserialization error: " << e.what() << std::endl;
		return false;
	}
	player.playerStatus(true);
	drawScoreboard = true;
}

void Game::pauseGame() {
	playingGame = false;
	MenuState = 3;
	gameClock.stop();
	player.pausePlayer();
}