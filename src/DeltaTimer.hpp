#pragma once
#include "AVRLib/HardwareTimer.hpp"
#include "util/atomic.h"

class Main;

class DeltaTimer {
public:
    void enable() {
        AvrLib::enableOnce([] {
            AvrLib::timerCounter1.freq11Max<1000>();
            AvrLib::timerCounter1.interrupt11<true>();
            AvrLib::timerCounter1.enable();
        });
    }

    uint8_t deltaMillis() {
        return lastDt;
    }

    void loopPassed() {
        ATOMIC_BLOCK(ATOMIC_FORCEON) {
            lastDt = dt;
        }
        dt = 0;
    }

    volatile uint8_t dt;
    uint8_t lastDt;
};

extern DeltaTimer deltaTimer;

namespace AvrLib {
    template<>
    struct TimerCounter1Interrupts<Main> : TimerCounter3Interrupts<void> {
        static void match11() {
            deltaTimer.dt += 1;
        }
    };
}