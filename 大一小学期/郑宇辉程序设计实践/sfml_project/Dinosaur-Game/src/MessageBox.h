#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class MessageBox {
private:
    sf::RenderWindow window;
    sf::RectangleShape background;
    sf::Text titleText;
    sf::Text contentText;
    sf::RectangleShape button;
    sf::Text buttonText;
    sf::Font font;

public:
    MessageBox(const std::string& title, const std::string& content,
               sf::Vector2u size = sf::Vector2u(400, 200));

    void show();
};