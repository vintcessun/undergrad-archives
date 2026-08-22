#pragma once

namespace settings {
const auto wWidth = 990;  // 窗口大小
const auto wHeight = 660;
const auto midWidth = 495;      // 球网水平位置
const auto humanHeight = 522;   // 球网高&人中心高
const auto groundHeight = 619;  // 地面高
const auto ballHeight = 573;    // 发球时球的高度
const auto leftLimit = 53;      // 左边界，球碰到会反弹
const auto leftManLimit = 89;   // 人能运动到的最左的位置
const auto ballLeft = 39;       // 发球时球距离左边距离
const auto manArmX = 90;        // 人手的位置
const auto manArmY = 79;
const auto armLen = 120;  // 胳膊长

const auto bSquare = 8;
const auto bSize = 80;
const auto btnLength = 24;
const auto btnLeft = (bSize * bSquare - btnLength) / 2;  // 控制开始按钮位置的
const auto lInterval = 6;

// IDC_START = 1                         ;开始按钮的编号
// idTimer1 dd 1                           ;定时器编号

const auto b1 = 0.0f;
const auto u1 = 0.37f;   //;羽毛球下落加速度
const auto u2 = 0.045f;  //;空气粘滞系数
const auto u3 = 0.55f;   //;人下落加速度
const auto vvv = 24;     //;球的初始速度，需要加一个向量做修正
const auto manv = 5.5f;
const auto manvneg = -5.5f;
const auto manjmpv = 10.5f;

const auto bx1 = -26;
const auto bx2 = -26;
const auto bx3 = 26;
const auto by1 = -26;
const auto by2 = 26;
const auto by3 = -26;
const auto bxp = 0;
const auto byp = 0;
};  // namespace settings