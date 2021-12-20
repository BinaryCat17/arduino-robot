#pragma once

#include <stdint.h>
#include "util/atomic.h"

extern volatile uint32_t timer2_millis;

class HardwareTimer {
public:
    static void enable();

    static uint32_t millis()
    {
        unsigned long millis_return;
        // Ensure this cannot be disrupted
        ATOMIC_BLOCK(ATOMIC_FORCEON) {
            millis_return = timer2_millis;
        }
        return millis_return;
    }
};
