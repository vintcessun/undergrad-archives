#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <fileSystem>

#include "SoundClips.h"
#include "sfml_serialization.hpp"

class Player {
   private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		ar & playerAlive;
		ar & playerCanJump;
		ar & playerRequestedJump;
		ar & jumpSpeed;

		ar & playerMinHeightY;

		ar & velocity;

		ar & animWalkTimeFrame;
		ar & step;
	}

   public:
	Player(float gameLevelY);
	sf::Sprite drawPlayer();
	void handlePlayerMovement(sf::Time, SoundClips *audio);
	void handlePlayerInput(sf::Keyboard::Key, bool);

	sf::FloatRect getPaddedGlobalBounds();

	void movementAnimation();

	bool isPlayerAlive();
	void playerStatus(bool status);

	void resetPlayer();

	std::array<sf::Texture, 10> loadTextures();

	void startPlayer();
	void pausePlayer();

   private:
	std::array<sf::Texture, 10> playerTextures;
	sf::Sprite player;

	bool playerAlive = true;
	bool playerCanJump;
	bool playerRequestedJump;
	float jumpSpeed;

	float playerMinHeightY;

	sf::Vector2f velocity;

	float animWalkTimeFrame = 0.1f;
	sf::Clock walkingTimer;
	int step = 0;
};
