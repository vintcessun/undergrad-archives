#pragma once

#include <SFML/Graphics.hpp>

class Image : public sf::Sprite {
public:
    Image(const std::string& src) {
        texture.loadFromFile(src);
        setTexture(texture);
    }

    sf::Texture texture;
};

class Images {
public:
    Images()
        : _0("img/0.png"),
          _1("img/1.png"),
          _2("img/2.png"),
          _3("img/3.png"),
          _4("img/4.png"),
          _5("img/5.png"),
          _6("img/6.png"),
          _7("img/7.png"),
          ball("img/ball.png"),
          bg("img/bg.jpg"),
          body("img/body.png"),
          hand1("img/hand1.png"),
          hand2("img/hand2.png"),
          hand3("img/hand3.png"),
          hand4("img/hand4.png"),
          hand5("img/hand5.png"),
          hand6("img/hand6.png"),
          hand7("img/hand7.png"),
          hand8("img/hand8.png"),
          hand9("img/hand9.png"),
          hand10("img/hand10.png"),
          hand11("img/hand11.png"),
          hand12("img/hand12.png"),
          hand13("img/hand13.png"),
          hand14("img/hand14.png"),
          hand15("img/hand15.png"),
          inst("img/inst.jpg"),
          leg1("img/leg1.png"),
          leg2("img/leg2.png"),
          leg3("img/leg3.png"),
          leg4("img/leg4.png"),
          leg5("img/leg5.png"),
          leg6("img/leg6.png"),
          leg7("img/leg7.png"),
          shadow("img/shadow.png"),
          start("img/start.jpg"),
          title("img/title.jpg") {}
    Image _0;
    Image _1;
    Image _2;
    Image _3;
    Image _4;
    Image _5;
    Image _6;
    Image _7;
    Image ball;
    Image bg;
    Image body;
    Image hand1;
    Image hand2;
    Image hand3;
    Image hand4;
    Image hand5;
    Image hand6;
    Image hand7;
    Image hand8;
    Image hand9;
    Image hand10;
    Image hand11;
    Image hand12;
    Image hand13;
    Image hand14;
    Image hand15;
    Image inst;
    Image leg1;
    Image leg2;
    Image leg3;
    Image leg4;
    Image leg5;
    Image leg6;
    Image leg7;
    Image shadow;
    Image start;
    Image title;
};