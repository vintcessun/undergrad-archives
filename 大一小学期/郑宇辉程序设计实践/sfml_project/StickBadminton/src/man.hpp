#pragma once

#include "setting.hpp"

class Man {
public:
    Man(int limit, int p)
        : mx(limit),
          my(settings::humanHeight),
          mvx(0),
          mvy(0),
          player(p),
          move(0),
          hit(0),
          frame1(0),
          frame2(0),
          score(0),
          lkey(0),
          rkey(0) {}
    int mx;
    int my;
    int mvx;
    int mvy;
    int player;
    int move;
    int hit;
    int frame1;
    int frame2;
    int score;
    int lkey;
    int rkey;
};