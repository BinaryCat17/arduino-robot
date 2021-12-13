#pragma once
#include "MUXController.hpp"
#include "Timer.hpp"

const float robotSpeed = 0.3;
const float robotKp = 0.001;

class Driver {
public:
    static void enable()
    {
        MUXController::enable();
        TimerMap::enable(Timer::T3P235, 5000);
    }

    static void correct() {
        uint16_t s1 = MUXController::read(Pin::A0);
        Serial.print("ADC0 : ");
        Serial.println(s1);
        uint16_t s2 = MUXController::read(Pin::A1);
        Serial.print("ADC1 : ");
        Serial.println(s2);
        uint16_t cur = s1 - s2;

        int16_t err = 0 - cur;
        Serial.print("Err : ");
        Serial.println(err);
        float out = robotKp * err;

        Serial.print("Out : ");
        Serial.println(out);

        TimerMap::fillFactor(Pin::D2, robotSpeed + out);
        TimerMap::fillFactor(Pin::D3, robotSpeed - out);
    }
};
