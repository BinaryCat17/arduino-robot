#include "PIDRegulator.hpp"
#include "AVRLib/SerialMonitor.hpp"

float PIDRegulator::calculate(float current, float dt) {
    // ошибка
    float const error = r - current;

    // пропорциональная составляющая
    float const Pout = error * Kp;

    // интегрирующая составляющая
    integral += error * dt;
    float const Iout = integral * Ki;

    // производная составляющая
    float const deriviate = (error - prevErr) / dt;
    float const Dout = Kd * deriviate;

    float output = Pout + Iout + Dout;
    prevErr = error;

    if (output > mMax)
        output = mMax;
    else if (output < mMin)
        output = mMin;

    return output;
}

void
PIDRegulator::start(float Kp_, float Ki_, float Kd_, float r_, float min_, float max_) {
    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;
    r = r_;
    mMin = min_;
    mMax = max_;
    integral = 0;
    prevErr = 0;
}
