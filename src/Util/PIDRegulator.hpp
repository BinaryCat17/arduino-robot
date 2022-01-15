#pragma once
#include "stdint.h"

class PIDRegulator {
public:
    void start(float Kp_, float Ki_, float Kd_, float r_, float min_, float max_);

    void target(float r_)
    {
        r = r_;
    }

    void min(float v)
    {
        mMin = v;
    }

    void max(float v)
    {
        mMax = v;
    }

    void reset()
    {
        integral = 0;
    }

    float calculate(float current, float dt);

private:
    //  коэффициенты усиления пропорциональной, интегрирующей и дифференцирующей составляющих регулятора соответственно
    float Kp;
    float Ki;
    float Kd;
    // целевой результат
    float r;

    // границы скорости двигателя
    float mMin;
    float mMax;

    float integral;
    float prevErr;
};

