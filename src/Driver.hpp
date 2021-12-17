#pragma once
#include "stdint.h"

class Driver {
public:
    static void enable();

    static void calibrate();

    static void speed(float speed);

    static void correct();

    static Driver& instance();

private:
    float m_robotSpeed = 0.3;
    uint32_t m_cnt = 0;
    uint32_t m_errSum = 0;
};
