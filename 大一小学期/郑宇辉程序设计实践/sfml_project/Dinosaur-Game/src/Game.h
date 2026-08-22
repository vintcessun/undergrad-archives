#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>

#include "Player.h"
#include "SFML/Graphics/Sprite.hpp"
#include "Scene.h"
#include "Scoreboard.h"
#include "SoundClips.h"
#include "sfml_serialization.hpp"

extern sf::RenderWindow mWindow;

class Game {
   private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & playingGame;
		ar & gameStartDelay;
		ar & gameObstacleStartDelay;
		ar & gameLevelY;
		ar & player;
		ar & scene;
		ar & scoreboard;
		ar & drawScoreboard;
		ar & drawHighscore;
		ar & endgameScale;
		ar & playAudioDied;
		ar & MenuState;

		ar & timePerFrame;
	}

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

	void startGame();
	void restartGame();
	void pauseGame();

	bool saveToFile(const std::string& filename);
	bool loadFromFile(const std::string& filename);

   private:
	sf::RenderWindow mWindow;

	sf::Time timePerFrame;

	bool playingGame = false;
	float gameStartDelay = 1.0f;
	sf::Clock gameClock;

	float gameObstacleStartDelay = 4.0f;

	float gameLevelY;

	Player player;
	Scene scene;
	Scoreboard scoreboard;

	bool drawScoreboard = true;
	bool drawHighscore = true;

	float endgameScale = 0.55;

	sf::Texture gameOverTexture;
	sf::Sprite gameOverText;

	sf::Texture replayTexture;
	sf::Sprite replayButton;

	SoundClips audioPlayer;
	bool playAudioDied = true;

	sf::Texture startTexture;
	sf::Sprite startMenu;

	int MenuState;

	sf::Texture helpTexture;
	sf::Sprite helpMenu;

	sf::Font TextFont;

	sf::Texture pauseTexture;
	sf::Sprite pauseMenu;

	sf::Texture saveTexture;
	sf::Sprite saveMenu;
};
