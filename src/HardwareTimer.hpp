#pragma once

#include <stdint.h>

class HardwareTimer {
public:
    static void enable();

    static uint32_t millis();

private:
    static HardwareTimer &instance();

    bool m_enabled = false;
};
