#pragma once

#include "HardwareTimer.hpp"

class PIDRegulator {
public:
    void start(float Kp_, float Ki_, float Kd_, int16_t r_, int16_t min_, int16_t max_);

    int16_t calculate(int16_t current);

private:
    //  коэффициенты усиления пропорциональной, интегрирующей и дифференцирующей составляющих регулятора соответственно
    float Kp;
    float Ki;
    float Kd;
    // целевой результат
    int16_t r;

    // границы скорости двигателя
    int16_t min;
    int16_t max;

    int16_t integral;
    int16_t prevErr;
    uint32_t prevTime;
};

