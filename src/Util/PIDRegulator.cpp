#include "PIDRegulator.hpp"

int16_t PIDRegulator::calculate(int16_t current, uint32_t dt) {
    // ошибка
    int16_t const error = r - current;

    // пропорциональная составляющая
    int16_t const Pout = error * Kp;

    // интегрирующая составляющая
    integral += error * dt;
    int16_t const Iout = integral * Ki;

    // производная составляющая
    int16_t const deriviate = (error - prevErr) / dt;
    int16_t const Dout = Kd * deriviate;

    int16_t output = Pout + Iout + Dout;
    prevErr = error;

    if (output > max)
        output = max;
    else if (output < min)
        output = min;

    return output;
}

void
PIDRegulator::start(float Kp_, float Ki_, float Kd_, int16_t r_, int16_t min_, int16_t max_) {
    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;
    r = r_;
    min = min_;
    max = max_;
    integral = 0;
    prevErr = 0;
}
