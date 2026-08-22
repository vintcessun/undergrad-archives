#include "Player.h"

#include <math.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "SFML/Window/Keyboard.hpp"

Player::Player(float gameLevelY)
	: playerTextures(loadTextures()),
	  player(playerTextures[1]),
	  playerCanJump(true),
	  playerRequestedJump(false),
	  jumpSpeed(100.0f),
	  playerMinHeightY(),
	  velocity(0.0, 0.0) {
	player.setScale({0.5f, 0.5f});

	playerMinHeightY = gameLevelY - (player.getTextureRect().size.y / 2.0f);
	player.setPosition({100.f, playerMinHeightY});
}

sf::Sprite Player::drawPlayer() { return player; }

void Player::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
	if (key == sf::Keyboard::Key::Space ||
		key == sf::Keyboard::Key::Up && playerCanJump) {
		playerRequestedJump = true;
	}
}

void Player::handlePlayerMovement(sf::Time deltaTime, SoundClips *audio) {
	if (playerRequestedJump && playerCanJump) {
		audio->playJumpSound();
		playerCanJump = false;
		velocity.y = -sqrtf(8.0f * 981.0f * jumpSpeed);
	} else if (player.getPosition().y < playerMinHeightY) {
		velocity.y += 4.0f * 981.0f * deltaTime.asSeconds();
	} else if (player.getPosition().y > playerMinHeightY - 40) {
		float interpolatedY = player.getPosition().y - playerMinHeightY;
		velocity.y = 0 * deltaTime.asSeconds();
		player.move({0.0f, -interpolatedY});

		playerCanJump = true;
	}

	playerRequestedJump = false;
	player.move(velocity * deltaTime.asSeconds());
}

sf::FloatRect Player::getPaddedGlobalBounds() {
	sf::FloatRect paddedRect = player.getGlobalBounds();

	paddedRect.position.y += paddedRect.size.x * 0.1f;
	paddedRect.position.x += paddedRect.size.y * 0.1f;
	paddedRect.size.x *= 0.8f;
	paddedRect.size.y *= 0.8f;

	return paddedRect;
}

void Player::movementAnimation() {
	if (!playerCanJump) {
		player.setTexture(playerTextures[1]);
	} else if (playerAlive) {
		if (walkingTimer.getElapsedTime().asSeconds() > animWalkTimeFrame) {
			walkingTimer.restart();
			if (step == 0) {
				step = 1;
				player.setTexture(playerTextures[3]);
			} else {
				step = 0;
				player.setTexture(playerTextures[4]);
			}
		}
	} else if (!playerAlive) {
		player.setTexture(playerTextures[5]);
	}
}

bool Player::isPlayerAlive() { return playerAlive; }

void Player::playerStatus(bool status) { playerAlive = status; }

void Player::resetPlayer() {
	player.setPosition({100.f, playerMinHeightY});
	playerCanJump = true;
	playerRequestedJump = false;
	playerAlive = true;

	player.setTexture(playerTextures[1]);
}

std::array<sf::Texture, 10> Player::loadTextures() {
	std::array<sf::Texture, 10> textures;
	for (int i = 1; i <= 9; ++i) {
		const std::string filename =
			"resources/dinosprite_0" + std::to_string(i) + ".png";
		if (!textures[i - 1].loadFromFile(filename)) {
		}
	}
	return textures;
}

void Player::startPlayer() { walkingTimer.start(); }

void Player::pausePlayer() { walkingTimer.stop(); }