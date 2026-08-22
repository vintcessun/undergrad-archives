#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "image.hpp"

class CTX {
public:
    static void drawImage(sf::RenderWindow& window, Image& sprite, float x,
                          float y, float w, float h) {
        sprite.setPosition({x, y});
        sprite.setScale(w / sprite.getLocalBounds().width,
                        h / sprite.getLocalBounds().height);

        // 绘制精灵
        window.draw(sprite);
    }
};