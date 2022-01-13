#pragma once

#include "Util/PIDRegulator.hpp"


class LineDriver {
public:
    void enable(float Kp_, float Ki_, float Kd_);

    void speed(int16_t speed);

    void drive();

private:
    int16_t rs = 300;
    PIDRegulator reg = {};
};

extern LineDriver lineRobot;
