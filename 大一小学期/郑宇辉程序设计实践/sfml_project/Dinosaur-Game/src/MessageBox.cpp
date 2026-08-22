#include "MessageBox.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "SFML/Window/Event.hpp"


MessageBox::MessageBox(const std::string& title, const std::string& content,
                       sf::Vector2u size)
    : font("resources/arial.ttf"),
      buttonText(font),
      titleText(font),
      contentText(font) {
    window.create(sf::VideoMode({size.x, size.y}), title,
                  sf::Style::Titlebar | sf::Style::Close);

    background.setSize(sf::Vector2f(size.x, size.y));
    background.setFillColor(sf::Color(40, 40, 40));

    titleText.setString(title);
    titleText.setCharacterSize(20);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition({15, 15});

    contentText.setString(content);
    contentText.setCharacterSize(16);
    contentText.setFillColor(sf::Color::White);
    contentText.setPosition({25, 55});

    button.setSize(sf::Vector2f(100, 35));
    button.setFillColor(sf::Color(70, 130, 180));
    button.setPosition({size.x / 2.0f - 50.0f, size.y - 60.0f});

    buttonText.setFont(font);
    buttonText.setString("OK");
    buttonText.setCharacterSize(16);
    buttonText.setFillColor(sf::Color::White);

    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
                          textRect.position.y + textRect.size.y / 2.0f});
    buttonText.setPosition(
        {button.getPosition().x + button.getSize().x / 2.0f,
         button.getPosition().y + button.getSize().y / 2.0f});
}

void MessageBox::show() {
    while (window.isOpen()) {
        auto e = window.pollEvent();
        if (e.has_value()) {
            auto event = e.value();
            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
            if (auto mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    auto mousePos = sf::Mouse::getPosition(window);
                    if (button.getGlobalBounds().contains(
                            {static_cast<float>(mousePos.x),
                             static_cast<float>(mousePos.y)})) {
                        window.close();
                    }
                }
            }
            if (auto key = event.getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Enter) {
                    window.close();
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(titleText);
        window.draw(contentText);
        window.draw(button);
        window.draw(buttonText);
        window.display();
    }
}
