#pragma once

#include <stdint.h>
#include "util/atomic.h"

#define ARDUINO_LIB
#if defined(ARDUINO_LIB)
#include "Arduino.h"
#endif

extern volatile uint32_t timer2_millis;

class HardwareTimer {
public:
    static void enable();

    static uint32_t millis()
    {
#if defined(ARDUINO_LIB)
        return millis();
#else
        unsigned long millis_return;
        // Ensure this cannot be disrupted
        ATOMIC_BLOCK(ATOMIC_FORCEON) {
            millis_return = timer2_millis;
        }
        return millis_return;
#endif
    }
};
