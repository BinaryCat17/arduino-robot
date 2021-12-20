#pragma once

#include "HardwareTimer.hpp"

class PIDRegulator {
public:
    void start(int16_t Kp_, int16_t Ki_, int16_t Kd_, int16_t r_, int16_t min_, int16_t max_);

    int16_t calculate(int16_t current);

private:
    //  коэффициенты усиления пропорциональной, интегрирующей и дифференцирующей составляющих регулятора соответственно
    int16_t Kp;
    int16_t Ki;
    int16_t Kd;
    // целевой результат
    int16_t r;

    // границы скорости двигателя
    int16_t min;
    int16_t max;

    int16_t integral;
    int16_t prevErr;
    uint32_t prevTime;
};

