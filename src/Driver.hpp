#pragma once

#include "PinMap.hpp"
#include "Timer.hpp"

class Driver {
public:
    static void enable();

    static void speed(float speed);

    static void correct();

private:

    static void speed1(float v) {
        engineSpeed(v, Pin::D3, Pin::D51);
    }

    static void speed2(float v) {
        engineSpeed(v, Pin::D2, Pin::D49);
    }

    static void engineSpeed(float speed, Pin engine, Pin control);

    static Driver &instance();

    int32_t cnt = 0;
    int32_t sum = 0;

    float m_robotSpeed = 0.3;
};
