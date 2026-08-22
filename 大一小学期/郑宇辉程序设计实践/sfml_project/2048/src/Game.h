#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>

#include "Settings.h"

namespace tw {
class Game {
   public:
	Game(int w, int h);
	void OnEvent(sf::Event e);
	void Update();
	void Render(sf::RenderTarget& tgt);

	void Spawn();
	void Reset();

   private:
	int tileSize;
	char map[FIELD_WIDTH][FIELD_HEIGHT] = {0};
	char tempMap[FIELD_WIDTH][FIELD_HEIGHT] = {0};
	sf::Clock animClock;
	bool animState;

	sf::Font font;

	std::vector<std::pair<std::pair<sf::Vector2i, sf::Vector2i>, char>> moves;

	sf::Color getTileColor(char tile);
	sf::Color getTextColor(char tile);
	std::string getText(char tile);
	int getTextSize(char tile);
	void move(char dirX, char dirY);
	void applyMove(sf::Vector2i from, sf::Vector2i to, int dx, int dy);
};
}  // namespace tw