#pragma once

#include "setting.hpp"

class Ball {
public:
    Ball()
        : mx(settings::ballLeft),
          my(settings::ballHeight),
          mvx(22),
          mvy(-22),
          max(0),
          may(0),
          marc(0),
          hitGroundTime(0),
          cannotHit(0),
          deg(0) {}
    int mx;
    int my;
    int mvx;
    int mvy;
    int max;
    int may;
    int marc;
    int hitGroundTime;
    int cannotHit;
    float deg;
};