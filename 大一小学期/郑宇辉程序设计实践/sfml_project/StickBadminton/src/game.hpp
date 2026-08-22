#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Man.hpp"
#include "SFML/System/Clock.hpp"
#include "ball.hpp"
#include "ctx.hpp"
#include "image.hpp"
#include "setting.hpp"

class Game {
public:
    Game()
        : window(sf::VideoMode({settings::wWidth, settings::wHeight}),
                 "Stick Badminton", sf::Style::Titlebar | sf::Style::Close),
          man1(settings::leftManLimit, 1),
          man2(settings::wWidth - settings::leftManLimit - 200, 2),
          theBall(),
          ballPreX(settings::ballLeft),
          ballPreY(settings::ballHeight),
          man1preX(settings::leftManLimit),
          man1preY(settings::humanHeight),
          man2preX(settings::wWidth - settings::leftManLimit - 200),
          man2preY(settings::humanHeight),
          hitGroundTimer(120),
          mypoint{0, 0, 0, 0, 0, 0},
          kw(0),
          ka(0),
          ks(0),
          kd(0),
          kup(0),
          kdown(0),
          kleft(0),
          kright(0),
          kenter(0),
          leftAIon(1),
          rightAIon(1),
          leftAIhitted(0),
          rightAIhitted(0),
          startingBall(1),
          startingBallPre(1),
          choosingMode(1),
          someoneWins(0),
          drawingTitle(1),
          drawingInst(1),
          score1(0),
          score2(0),
          readPicture(0),
          images(),
          TIMER() {
        onPaint();
    }

    void drawTitle() {
        CTX::drawImage(window, images.title, 0, 0, settings::wWidth,
                       settings::wHeight);
    }

    void drawInst() {
        CTX::drawImage(window, images.inst, 0, 0, settings::wWidth,
                       settings::wHeight);
    }

    void drawChooseMode() {
        CTX::drawImage(window, images.start, 0, 0, settings::wWidth,
                       settings::wHeight);
    }

    void drawBackground() {
        CTX::drawImage(window, images.bg, 0, 0, settings::wWidth,
                       settings::wHeight);
    }
    void drawImage(sf::Sprite& sprite, float x, float y, float w, float h) {
        // 保存原始状态
        sf::Vector2f originalPosition = sprite.getPosition();
        sf::Vector2f originalScale = sprite.getScale();
        float originalRotation = sprite.getRotation();
        sf::Vector2f originalOrigin = sprite.getOrigin();

        if (w > 0) {
            // 正常绘制（宽度为正）
            sprite.setPosition(x, y);
            sprite.setScale(w / sprite.getLocalBounds().width,
                            h / sprite.getLocalBounds().height);

            window.draw(sprite);
        } else {
            // 水平翻转绘制（宽度为负）
            sprite.setPosition(-w - x, y);
            sprite.setScale(-w / sprite.getLocalBounds().width,
                            h / sprite.getLocalBounds().height);

            window.draw(sprite);

            // 恢复原始状态
            sprite.setPosition(originalPosition);
            sprite.setScale(originalScale);
            sprite.setRotation(originalRotation);
            sprite.setOrigin(originalOrigin);
        }
    }

    // 绘制图像函数 - 支持旋转
    void drawImage2(sf::Sprite& sprite, float x, float y, float w, float h,
                    float deg) {
        // 保存原始状态
        sf::Vector2f originalPosition = sprite.getPosition();
        sf::Vector2f originalScale = sprite.getScale();
        float originalRotation = sprite.getRotation();
        sf::Vector2f originalOrigin = sprite.getOrigin();

        // 计算旋转中心（图像中心）
        float centerX = x - w / 2;
        float centerY = y - h / 2;

        // 设置变换
        sprite.setPosition(centerX, centerY);
        sprite.setOrigin(w / 2, h / 2);  // 设置旋转中心为图像中心
        sprite.setRotation(deg);         // 设置旋转角度（度）
        sprite.setScale(w / sprite.getLocalBounds().width,
                        h / sprite.getLocalBounds().height);

        // 绘制图像
        window.draw(sprite);

        // 恢复原始状态
        sprite.setPosition(originalPosition);
        sprite.setScale(originalScale);
        sprite.setRotation(originalRotation);
        sprite.setOrigin(originalOrigin);
    }
    void drawMan(int who) {
        int eax, ebx;
        if (who == 1) {
            drawImage(images.body, man1.mx - 145, man1.my - 88, 246, 213);
            drawImage(images.shadow, man1.mx - 145, 434, 246, 213);
            eax = man1.mx - 145;
            ebx = man1.my - 88;
            switch (man1.frame2) {
            case 0:
                drawImage(images.leg1, eax, ebx, 246, 213);
                if (man1.move != 0) {
                    man1.frame2 = 11;
                }
                break;
            case 1:
                drawImage(images.leg2, eax, ebx, 246, 213);
                man1.frame2--;
                man1.move = 0;
                break;
            case 11:
                drawImage(images.leg2, eax, ebx, 246, 213);
                man1.frame2--;
                break;
            case 2:
            case 10:
                drawImage(images.leg3, eax, ebx, 246, 213);
                man1.frame2--;
                break;
            case 3:
            case 9:
                drawImage(images.leg4, eax, ebx, 246, 213);
                man1.frame2--;
                break;
            case 4:
            case 8:
                drawImage(images.leg5, eax, ebx, 246, 213);
                man1.frame2--;
                break;
            case 5:
            case 7:
                drawImage(images.leg6, eax, ebx, 246, 213);
                man1.frame2--;
                break;
            case 6:
                drawImage(images.leg7, eax, ebx, 246, 213);
                man1.frame2--;
                break;
            }
            eax = man1.mx - 140;
            ebx = man1.my - 145;
            if (man1.frame1 > 16 || startingBall == 1) {
                ebx += 57;
                eax -= 5;
            }
            if (startingBall == 1) {
                drawImage(images.hand9, eax, ebx, 246, 213);
            } else {
                switch (man1.frame1) {
                case 0:
                    drawImage(images.hand1, eax, ebx, 246, 213);
                    break;
                case 1:
                    drawImage(images.hand2, eax, ebx, 246, 213);
                    man1.frame1--;
                    man1.hit = 0;
                    break;
                case 2:
                    drawImage(images.hand3, eax, ebx, 246, 213);
                    man1.frame1--;
                    break;
                case 3:
                    drawImage(images.hand4, eax, ebx, 246, 213);
                    man1.frame1--;
                    break;
                case 4:
                    drawImage(images.hand5, eax, ebx, 246, 213);
                    man1.frame1--;
                    break;
                case 5:
                    drawImage(images.hand6, eax, ebx, 246, 213);
                    man1.frame1--;
                    break;
                case 6:
                    drawImage(images.hand7, eax, ebx, 246, 213);
                    man1.frame1--;
                    break;
                case 7:
                    drawImage(images.hand8, eax, ebx, 246, 213);
                    man1.frame1--;
                    break;
                case 21:
                    drawImage(images.hand15, eax, ebx, 246, 213);
                    man1.frame1 = 7;
                    man1.hit = 0;
                    break;
                case 22:
                    drawImage(images.hand14, eax, ebx, 246, 213);
                    man1.frame1--;
                    break;
                case 23:
                    drawImage(images.hand13, eax, ebx, 246, 213);
                    man1.frame1--;
                    break;
                case 24:
                    drawImage(images.hand12, eax, ebx, 246, 213);
                    man1.frame1--;
                    break;
                case 25:
                    drawImage(images.hand11, eax, ebx, 246, 213);
                    man1.frame1--;
                    break;
                case 26:
                    drawImage(images.hand10, eax, ebx, 246, 213);
                    man1.frame1--;
                    break;
                case 27:
                    drawImage(images.hand9, eax, ebx, 246, 213);
                    man1.frame1--;
                    break;
                }
            }
        } else {
            eax = man2.mx + 145;
            ebx = man2.my - 88;
            drawImage(images.body, eax, ebx, -246, 213);
            drawImage(images.shadow, eax, 434, -246, 213);
            switch (man2.frame2) {
            case 0:
                drawImage(images.leg1, eax, ebx, -246, 213);
                if (man2.move != 0) {
                    man2.frame2 = 11;
                }
                break;
            case 1:
                drawImage(images.leg2, eax, ebx, -246, 213);
                man2.frame2--;
                man2.move = 0;
                break;
            case 11:
                drawImage(images.leg2, eax, ebx, -246, 213);
                man2.frame2--;
                break;
            case 2:
            case 10:
                drawImage(images.leg3, eax, ebx, -246, 213);
                man2.frame2--;
                break;
            case 3:
            case 9:
                drawImage(images.leg4, eax, ebx, -246, 213);
                man2.frame2--;
                break;
            case 4:
            case 8:
                drawImage(images.leg5, eax, ebx, -246, 213);
                man2.frame2--;
                break;
            case 5:
            case 7:
                drawImage(images.leg6, eax, ebx, -246, 213);
                man2.frame2--;
                break;
            case 6:
                drawImage(images.leg7, eax, ebx, -246, 213);
                man2.frame2--;
                break;
            }
            eax = man2.mx + 140;
            ebx = man2.my - 145;
            if (man2.frame1 > 16 || startingBall == 2) {
                ebx += 57;
                eax += 5;
            }
            if (startingBall == 2) {
                drawImage(images.hand9, eax, ebx, -246, 213);
            } else {
                switch (man2.frame1) {
                case 0:
                    drawImage(images.hand1, eax, ebx, -246, 213);
                    break;
                case 1:
                    drawImage(images.hand2, eax, ebx, -246, 213);
                    man2.frame1--;
                    man2.hit = 0;
                    break;
                case 2:
                    drawImage(images.hand3, eax, ebx, -246, 213);
                    man2.frame1--;
                    break;
                case 3:
                    drawImage(images.hand4, eax, ebx, -246, 213);
                    man2.frame1--;
                    break;
                case 4:
                    drawImage(images.hand5, eax, ebx, -246, 213);
                    man2.frame1--;
                    break;
                case 5:
                    drawImage(images.hand6, eax, ebx, -246, 213);
                    man2.frame1--;
                    break;
                case 6:
                    drawImage(images.hand7, eax, ebx, -246, 213);
                    man2.frame1--;
                    break;
                case 7:
                    drawImage(images.hand8, eax, ebx, -246, 213);
                    man2.frame1--;
                    break;
                case 21:
                    drawImage(images.hand15, eax, ebx, -246, 213);
                    man2.frame1 = 7;
                    man2.hit = 0;
                    break;
                case 22:
                    drawImage(images.hand14, eax, ebx, -246, 213);
                    man2.frame1--;
                    break;
                case 23:
                    drawImage(images.hand13, eax, ebx, -246, 213);
                    man2.frame1--;
                    break;
                case 24:
                    drawImage(images.hand12, eax, ebx, -246, 213);
                    man2.frame1--;
                    break;
                case 25:
                    drawImage(images.hand11, eax, ebx, -246, 213);
                    man2.frame1--;
                    break;
                case 26:
                    drawImage(images.hand10, eax, ebx, -246, 213);
                    man2.frame1--;
                    break;
                case 27:
                    drawImage(images.hand9, eax, ebx, -246, 213);
                    man2.frame1--;
                    break;
                }
            }
        }
    }

    void drawBall() {
        int bxp =
            (settings::bx3 - settings::bx1) / 2 + settings::bx1 - settings::bx2;
        int byp =
            (settings::by3 - settings::by1) / 2 + settings::by1 - settings::by2;
        mypoint[0] = theBall.mx + settings::bx1;
        mypoint[1] = theBall.my + settings::by1;
        mypoint[2] = theBall.mx + settings::bx2;
        mypoint[3] = theBall.my + settings::by2;
        mypoint[4] = theBall.mx + settings::bx3;
        mypoint[5] = theBall.my + settings::by3;
        // drawImage2(images.ball, mypoint);
        // drawImage(images.ball, theBall.mx + bx1, theBall.my + by1, -bx1 * 2,
        // -by1 * 2, theBall.deg);
        drawImage2(images.ball, theBall.mx, theBall.my, -settings::bx1 * 2,
                   -settings::by1 * 2, theBall.deg);
        // console.log(theBall.deg);
    }

    Image& get_image_by_score(int score) {
        switch (score) {
        case 0:
            return images._0;
        case 1:
            return images._1;
        case 2:
            return images._2;
        case 3:
            return images._3;
        case 4:
            return images._4;
        case 5:
            return images._5;
        case 6:
            return images._6;
        case 7:
            return images._7;
        default:
            return images._0;
        }
    }

    void drawPoint() {
        drawImage(get_image_by_score(score1), 448, 34, 22, 34);
        drawImage(get_image_by_score(score2), 518, 34, 22, 34);
    }

    void ballHitWall() {
        if (theBall.mx > settings::wWidth - settings::leftLimit) {
            theBall.mvx *= -1;
            theBall.mx -= 2 * (settings::wWidth - settings::leftLimit);
            theBall.mx *= -1;
        }
        if (theBall.mx < settings::leftLimit) {
            theBall.mvx *= -1;
            theBall.mx -= 2 * settings::leftLimit;
            theBall.mx *= -1;
        }
    }

    void ballHitGround() {
        theBall.mvy *= -1;
        theBall.mvy /= 4;
        theBall.mvx /= 2;

        theBall.my -= 2 * settings::groundHeight;
        theBall.my *= -1;

        theBall.hitGroundTime++;
        theBall.cannotHit = 1;
        if (theBall.hitGroundTime == 1) {
            if (ballPreX < settings::midWidth) {
                man2.score++;
                score2++;
            } else {
                man1.score++;
                score1++;
            }
        }
        if (theBall.hitGroundTime != 1)
            return;
        if (ballPreX < settings::wWidth / 2) {
            startingBallPre = 2;
        } else {
            startingBallPre = 1;
        }
    }

    void ballStop() {
        theBall.my = settings::groundHeight;
        theBall.mvx = 0;
        theBall.mvy = 0;
        theBall.max = 0;
        theBall.may = 0;

        if (someoneWins == 0) {
            hitGroundTimer--;
        }

        if (hitGroundTimer == 0) {
            theBall.hitGroundTime = 0;
            hitGroundTimer = 30;
            theBall.cannotHit = 0;
        } else {
            return;
        }
        startingBall = startingBallPre;
        if (theBall.mx < settings::wWidth / 2) {
            leftAIhitted = 0;
            rightAIhitted = 1;
        } else {
            leftAIhitted = 1;
            rightAIhitted = 0;
        }
    }

    void ballHitNet() {
        theBall.mvx /= -2;
        theBall.mx -= settings::wWidth;
        theBall.mx *= -1;
    }

    void checkBallStatus() {
        if (theBall.my > settings::groundHeight)
            ballHitGround();
        if (theBall.mx > settings::wWidth - settings::leftLimit ||
            theBall.mx < settings::leftLimit)
            ballHitWall();
        if ((theBall.mx >= settings::midWidth) ^
            (ballPreX >= settings::midWidth)) {
            if (theBall.my >= settings::humanHeight &&
                theBall.my <= settings::groundHeight &&
                ballPreY >= settings::humanHeight &&
                ballPreY <= settings::groundHeight) {
                ballHitNet();
            }
        }
        if (theBall.hitGroundTime > 3)
            ballStop();
    }

    void ballDeg() {
        // var tmp00, tmp01, tmpvx, tmpvy, tmpvv = 1000;
        // tmpvx = theBall.mvx * tmpvv;
        // tmpvy = theBall.mvy * tmpvv;
        // if (tmpvx == 0) {
        //     if (tmpvy > 0) {

        //     }
        // }
        theBall.deg = std::atan2(theBall.mvy, theBall.mvx);
    }

    void ballThreePoint() {}

    void ballMove() {
        if (startingBall == 0) {
            theBall.max = -theBall.mvx * settings::u2;
            theBall.may = settings::u1 - theBall.mvy * settings::u2;
            theBall.mvx += theBall.max;
            theBall.mvy += theBall.may;
            ballPreX = theBall.mx;
            theBall.mx += theBall.mvx;
            ballPreY = theBall.my;
            theBall.my += theBall.mvy;
            ballDeg();
            checkBallStatus();
        } else {
            if (startingBall == 1) {
                auto eax = man1.mx;
                eax += settings::ballLeft;
                if (leftAIon == 0) {
                    eax -= 10;
                }
                theBall.mx = eax;
                eax = settings::ballHeight;
                eax -= settings::humanHeight;
                eax += man1.my;
                theBall.my = eax;
            } else {
                auto eax = man2.mx;
                eax -= settings::ballLeft;
                if (rightAIon == 0) {
                    eax += 10;
                }
                theBall.mx = eax;
                eax = settings::ballHeight;
                eax -= settings::humanHeight;
                eax += man2.my;
                theBall.my = eax;
            }
        }
    }

    void manMove() {
        man1preX = man1.mx;
        man1preY = man1.my;
        man2preX = man2.mx;
        man2preY = man2.my;
        if (ka == 1 && kd == 0) {
            man1.mvx = settings::manvneg;
            man1.move = 1;
        } else if (kd == 1 && ka == 0) {
            man1.mvx = settings::manv;
            man1.move = 1;
        } else {
            man1.mvx = 0;
        }
        if (kleft == 1 && kright == 0) {
            man2.mvx = settings::manvneg;
            man2.move = 1;
        } else if (kright == 1 && kleft == 0) {
            man2.mvx = settings::manv;
            man2.move = 1;
        } else {
            man2.mvx = 0;
        }
        if (kw == 1 && man1.my == settings::humanHeight) {
            kw = 0;
            man1.mvy = settings::manjmpv;
        }
        if (kup == 1 && man2.my == settings::humanHeight) {
            kup = 0;
            man2.mvy = settings::manjmpv;
        }

        man1.mx += man1.mvx;
        man2.mx += man2.mvx;

        man1.mvy -= settings::u3;
        man2.mvy -= settings::u3;

        man1.my -= man1.mvy;
        man2.my -= man2.mvy;

        if (man1.my > settings::humanHeight) {
            man1.my = settings::humanHeight;
            man1.mvy = 0;
        }
        if (man2.my > settings::humanHeight) {
            man2.my = settings::humanHeight;
            man2.mvy = 0;
        }

        if (man1.mx < settings::leftManLimit)
            man1.mx = settings::leftManLimit;
        if (man1.mx > settings::midWidth - settings::leftLimit)
            man1.mx = settings::midWidth - settings::leftLimit;
        if (man2.mx > settings::wWidth - settings::leftManLimit)
            man2.mx = settings::wWidth - settings::leftManLimit;
        if (man2.mx < settings::midWidth + settings::leftLimit)
            man2.mx = settings::midWidth + settings::leftLimit;
        if (startingBall == 1 && man1.mx > 280)
            man1.mx = 280;
        if (startingBall == 2 && man2.mx < settings::wWidth - 280)
            man2.mx = settings::wWidth - 280;
    }

    template <typename T>
    T getDistance(T x1, T y1, T x2, T y2) {
        const T dx = x1 - x2;
        const T dy = y1 - y2;
        return static_cast<T>(std::sqrt(dx * dx + dy * dy));
    }

    int getSpeed(int who) {
        int tmp0, tmp1;
        // ;速度=vvv*球到人距离/拍子最大长度
        // ;如果不在拍子范围则返回0
        int eax, ebx;
        if (who == 1) {
            // ;这段代码判断球在拍子后面的那个扇形里面
            if (theBall.mx < man1.mx) {
                ebx = man1.mx;
                ebx -= theBall.mx;
                eax = theBall.my;
                eax -= man1.my;
                tmp1 = eax;
                if (tmp1 < 0)
                    eax *= -1;
                eax *= 90;
                tmp0 = eax;
                eax = ebx;
                eax *= 79;
                if (tmp0 < eax) {
                    eax = 0;
                    return eax;
                }
            }
        } else {
            if (theBall.mx > man2.mx) {
                ebx = theBall.mx;
                ebx -= man2.mx;
                eax = theBall.my;
                eax -= man2.my;
                tmp1 = eax;
                if (tmp1 < 0)
                    eax *= -1;
                eax *= 90;
                tmp0 = eax;
                eax = ebx;
                eax *= 79;
                if (tmp0 < eax) {
                    eax = 0;
                    return 0;
                }
            }
        }
        if (who == 1) {
            eax = getDistance(man1.mx, man1.my, theBall.mx, theBall.my);
        } else {
            eax = getDistance(man2.mx, man2.my, theBall.mx, theBall.my);
        }
        tmp0 = eax;
        if (tmp0 >= 45 && tmp0 <= 150) {
            eax *= settings::vvv;
            eax /= 120;
            return eax;
        }
        eax = 0;
        return eax;
    }

    void checkHitBall() {
        if (ks == 1) {
            if (startingBall == 1) {
                startingBall = 0;
                return;
            }
            ks = 0;
            if (man1.hit == 0) {
                if (man1.my < theBall.my) {
                    man1.frame1 = 27;
                } else {
                    man1.frame1 = 7;
                }
                man1.hit = 1;
            }
            if (theBall.cannotHit == 0) {
                if (getSpeed(1) != 0 && theBall.mx < settings::midWidth) {
                    manHitBall(man1.mx, man1.my, theBall.mx, theBall.my,
                               settings::vvv);
                }
            }
        }
        if (kdown == 1) {
            if (startingBall == 2) {
                startingBall = 0;
                return;
            }
            kdown = 0;
            if (man2.hit == 0) {
                if (man2.my < theBall.my) {
                    man2.frame1 = 27;
                } else {
                    man2.frame1 = 7;
                }
                man2.hit = 1;
            }
            if (theBall.cannotHit == 0) {
                if (getSpeed(2) != 0 && theBall.mx > settings::midWidth) {
                    manHitBall(man2.mx, man2.my, theBall.mx, theBall.my,
                               settings::vvv);
                }
            }
        }
    }

    void hitBall(int sx, int sy, int svx, int svy) {
        theBall.mx = sx;
        theBall.my = sy;
        theBall.mvx = svx;
        theBall.mvy = svy;
    }

    void hitBallDegree(int sx, int sy, int v0, float deg) {
        hitBall(sx, sy, v0 * std::cos(-deg), v0 * std::sin(-deg));
    }

    void manHitBall(int mx, int my, int sx, int sy, int speed) {
        auto symy2 = std::pow(sy - my, 2);
        auto sxmx2 = std::pow(sx - mx, 2);
        auto symysxmx = symy2 + sxmx2;
        auto a2 = std::pow(speed, 2) * symy2 / symysxmx;
        auto b2 = std::pow(speed, 2) - a2;

        auto a = std::sqrt(a2);
        auto b = std::sqrt(b2);

        if (sx > mx)
            a *= -1;
        if (sy < my)
            b *= -1;
        if (mx < settings::midWidth && a < 0) {
            a *= -1;
            b *= -1;
        }
        if (mx > settings::midWidth && a > 0) {
            a *= -1;
            b *= -1;
        }
        if (mx < settings::midWidth) {
            a += 12;
            b -= 12;
        } else {
            a -= 12;
            b -= 12;  // ?
        }

        if (sy > my) {
            if (mx < settings::midWidth) {
                a = 29;
                b = -29;
            } else {
                a = -29;
                b = -29;
            }
        }
        hitBall(sx, sy, a, b);
    }

    void leftAIconsider() {
        /*
        如何写一个AI ?
            · 下面提示要注意的一定不能改
            · 写AI也就是根据当前各个物体的状态参数等，确定要完成的键盘操作
            · 左边AI可用的键盘操作有kw/ka/ks/kd
            · 按下按键就是把kw等等变成1，抬起就是变成0
        */
        // ;注意! 下面这三行不能删!!!
        int temp2;
        if (theBall.mx > settings::midWidth) {
            leftAIhitted = 0;
        }
        // ;控制左右移动的逻辑，会跟随球的横坐标
        if (theBall.mx > settings::midWidth) {
            if (man1.mx > settings::midWidth / 4) {
                ka = 1;
                kd = 0;
            } else if (man1.mx < settings::midWidth / 4) {
                kd = 1;
                ka = 0;
            }
        } else {
            if (theBall.mx > man1.mx + 150) {
                kd = 1;
                ka = 0;
            } else if (man1.mx - 30 > theBall.mx) {
                ka = 1;
                kd = 0;
            } else {
                ka = 0;
                kd = 0;
            }
        }
        // ;控制跳跃的逻辑，会跟随球的横坐标
        // ;控制击球动作的逻辑
        // ;注意!
        // 击球动作发出前，必须判断leftAIhitted为0，击球后要将之置为1!!!!!!
        if (leftAIhitted == 0) {
            temp2 = getDistance(theBall.mx, theBall.my, man1.mx, man1.my);
            if (temp2 < 110 && temp2 > 45) {
                ks = 1;
                leftAIhitted = 1;
            }
        }
    }

    void rightAIconsider() {
        /*
        如何写一个AI ?
            · 下面提示要注意的一定不能改
            · 写AI也就是根据当前各个物体的状态参数等，确定要完成的键盘操作
            · 右边AI可用的键盘操作有kup/kdown/kleft/kright
            · 按下按键就是把kup等等变成1，抬起就是变成0
        */
        int temp1, eax, ebx, ecx;
        // ;注意! 下面这三行不能删!!!
        if (theBall.mx < settings::midWidth) {
            rightAIhitted = 0;
        }
        if (theBall.mx < settings::midWidth) {
            eax = settings::midWidth * 1.5 + 30;
            ebx = settings::midWidth * 1.5 - 30;
            if (man2.mx < ebx) {
                kright = 1;
                kleft = 0;
            } else if (eax < man2.mx) {
                kleft = 1;
                kright = 0;
            } else {
                kleft = 0;
                kright = 0;
            }
        } else {
            eax = man2.mx + 30;
            ebx = man2.mx - 30;
            ecx = theBall.mx + 50;
            if (ecx > eax) {
                kright = 1;
                kleft = 0;
            } else if (ebx > ecx) {
                kleft = 1;
                kright = 0;
            } else {
                kleft = 0;
                kright = 0;
            }
        }
        // ;控制击球动作的逻辑
        // ;注意!
        // 击球动作发出前，必须判断leftAIhitted为0，击球后要将之置为1!!!!!!
        if (rightAIhitted == 0) {
            temp1 = getDistance(theBall.mx, theBall.my, man2.mx, man2.my);
            if (temp1 < 110 && temp1 > 45) {
                kdown = 1;
                rightAIhitted = 1;
            }
        } else {
            kup = 0;
            return;
        }
        ebx = man2.my - 80;
        temp1 = getDistance(theBall.mx, theBall.my, man2.mx, ebx);
        if (temp1 < 110 && temp1 > 45) {
            kup = 1;
        }
    }

    void showWinMessageBox(int who) {
        sf::RectangleShape box(sf::Vector2f(300, 150));
        box.setPosition(window.getSize().x / 2.0 - 150,
                        window.getSize().y / 2.0 - 75);
        box.setFillColor(sf::Color(50, 50, 50, 220));
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color::White);

        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
            return;
        }

        sf::Text text("Player " + std::to_string(who) + " wins!", font, 24);
        text.setPosition(box.getPosition().x + 50, box.getPosition().y + 60);
        text.setFillColor(sf::Color::White);

        window.draw(box);
        window.draw(text);
        window.display();

        // 等待3秒（非阻塞）
        sf::Clock clock;
        while (clock.getElapsedTime().asSeconds() < 3.f) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return;
                }
            }
        }
    }

    // 绘制一帧
    void drawOneFrame() {
        if (choosingMode == 1) {
            readPicture = 1;
        }
        onPaint();
    }

    // 游戏主循环
    void stepOver() {
        CLOCK++;
        drawOneFrame();
        ballMove();
        manMove();
        checkHitBall();
        if (leftAIon == 1)
            leftAIconsider();
        if (rightAIon == 1)
            rightAIconsider();

        if (man1.score == 7) {
            man1.score = 0;
            man2.score = 0;
            someoneWins = 1;
            showWinMessageBox(1);

            // 使用 SFML 定时器替代 setTimeout
            sf::Clock timer;
            while (timer.getElapsedTime().asSeconds() < 3.f) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                        return;
                    }
                }
            }

            choosingMode = 1;
            drawChooseMode();
        }

        if (man2.score == 7) {
            man1.score = 0;
            man2.score = 0;
            someoneWins = 1;
            showWinMessageBox(2);

            sf::Clock timer;
            while (timer.getElapsedTime().asSeconds() < 3.f) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                        return;
                    }
                }
            }

            choosingMode = 1;
            drawChooseMode();
        }
    }

    // 绘制处理
    void onPaint() {
        window.clear();  // 清屏

        if (readPicture == 0) {
            // loadImages();
            drawTitle();
            readPicture = 1;
        } else {
            if (drawingTitle == 1) {
                drawTitle();
            } else if (drawingInst == 1) {
                drawInst();
            } else if (choosingMode == 1) {
                drawChooseMode();
            } else {
                drawBackground();
                drawPoint();
                drawMan(1);
                drawMan(2);
                drawBall();
            }
        }

        window.display();  // 显示绘制内容
    }

    // 开始按钮处理
    void onBtnStart() {
        std::cout << "onBtnStart" << std::endl;

        man1.mx = settings::leftManLimit;
        man1.my = settings::humanHeight;
        man2.mx = settings::wWidth - settings::leftManLimit - 200;
        man2.my = settings::humanHeight;
        hitGroundTimer = 30;
        someoneWins = 0;
        startingBall = 1;
        startingBallPre = 1;
        leftAIhitted = 0;
        rightAIhitted = 0;
        theBall.hitGroundTime = 0;
        theBall.cannotHit = 0;
        kw = 0;
        ka = 0;
        ks = 0;
        kd = 0;
        kleft = 0;
        kright = 0;
        kup = 0;
        kright = 0;
        score1 = 0;
        score2 = 0;
        CLOCK = 0;

        TIMER.restart();
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                // 处理窗口关闭事件
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                // 处理键盘按下事件
                if (event.type == sf::Event::KeyPressed) {
                    if (leftAIon == 0) {
                        switch (event.key.code) {
                        case sf::Keyboard::W:
                            kw = 1;
                            break;
                        case sf::Keyboard::S:
                            ks = 1;
                            break;
                        case sf::Keyboard::A:
                            ka = 1;
                            break;
                        case sf::Keyboard::D:
                            kd = 1;
                            break;
                        default:
                            break;
                        }
                    }

                    if (rightAIon == 0) {
                        switch (event.key.code) {
                        case sf::Keyboard::Up:
                            kup = 1;
                            break;
                        case sf::Keyboard::Down:
                            kdown = 1;
                            break;
                        case sf::Keyboard::Left:
                            kleft = 1;
                            break;
                        case sf::Keyboard::Right:
                            kright = 1;
                            break;
                        default:
                            break;
                        }
                    }
                }

                if (event.type == sf::Event::KeyReleased) {
                    if (leftAIon == 0) {
                        switch (event.key.code) {
                        case sf::Keyboard::W:
                            kw = 0;
                            break;
                        case sf::Keyboard::S:
                            ks = 0;
                            break;
                        case sf::Keyboard::A:
                            ka = 0;
                            break;
                        case sf::Keyboard::D:
                            kd = 0;
                            break;
                        default:
                            break;
                        }
                    }

                    if (rightAIon == 0) {
                        switch (event.key.code) {
                        case sf::Keyboard::Up:
                            kup = 0;
                            break;
                        case sf::Keyboard::Down:
                            kdown = 0;
                            break;
                        case sf::Keyboard::Left:
                            kleft = 0;
                            break;
                        case sf::Keyboard::Right:
                            kright = 0;
                            break;
                        default:
                            break;
                        }
                    }
                }

                // 处理鼠标点击事件
                if (event.type == sf::Event::MouseButtonReleased &&
                    event.mouseButton.button == sf::Mouse::Left) {
                    // 获取鼠标坐标并转换为游戏坐标系
                    float mousex = static_cast<float>(event.mouseButton.x) /
                                   window.getSize().x * 990;
                    float mousey = static_cast<float>(event.mouseButton.y) /
                                   window.getSize().y * 660;

                    if (drawingTitle == 1) {
                        drawingTitle = 0;
                        onPaint();
                        continue;
                    }

                    if (drawingInst == 1) {
                        drawingInst = 0;
                        onPaint();
                        continue;
                    }

                    if (choosingMode == 0) {
                        continue;
                    }

                    // 处理模式选择按钮点击
                    if (mousex < 900 && mousex > 650) {
                        if (mousey > 320 && mousey < 380) {
                            leftAIon = 0;
                            rightAIon = 0;
                            choosingMode = 0;
                            onBtnStart();
                        } else if (mousey > 380 && mousey < 440) {
                            leftAIon = 0;
                            rightAIon = 1;
                            choosingMode = 0;
                            onBtnStart();
                        } else if (mousey > 440 && mousey < 500) {
                            leftAIon = 1;
                            rightAIon = 0;
                            choosingMode = 0;
                            onBtnStart();
                        } else if (mousey > 500 && mousey < 560) {
                            leftAIon = 1;
                            rightAIon = 1;
                            choosingMode = 0;
                            onBtnStart();
                        }
                    }
                }
                drawOneFrame();
            }
        }
    }

    void imageLoadComplete() {
        // onBtnStart();
        onPaint();
    }

    sf::RenderWindow window;
    Man man1, man2;
    Ball theBall;
    int ballPreX;
    int ballPreY;
    int man1preX;
    int man1preY;
    int man2preX;
    int man2preY;
    int hitGroundTimer;
    int mypoint[6];
    int kw;
    int ka;
    int ks;
    int kd;
    int kup;
    int kdown;
    int kleft;
    int kright;
    int kenter;
    int leftAIon;
    int rightAIon;
    int leftAIhitted;
    int rightAIhitted;

    int startingBall;  //  ;0:不在发球，1:左边发球，2:右边发球
    int startingBallPre;

    int choosingMode;
    int someoneWins;
    int drawingTitle;
    int drawingInst;

    int score1;
    int score2;

    int readPicture;

    Images images;

    int CLOCK = 0;
    sf::Clock TIMER;
};