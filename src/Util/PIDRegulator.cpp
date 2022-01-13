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

    if (output > max)
        output = max;
    else if (output < min)
        output = min;

    return output;
}

void
PIDRegulator::start(float Kp_, float Ki_, float Kd_, float r_, float min_, float max_) {
    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;
    r = r_;
    min = min_;
    max = max_;
    integral = 0;
    prevErr = 0;
}
