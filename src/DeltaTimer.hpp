#pragma once
#include "AVRLib/HardwareTimer.hpp"
#include "util/atomic.h"

class Main;

extern volatile uint16_t _impl_dt;

class DeltaTimer {
public:
    void enable() {
        AvrLib::enableOnce([] {
            AvrLib::timerCounter4.freq6Max<1000000>();
            AvrLib::timerCounter4.interrupt6<true>();
            AvrLib::timerCounter4.enable();
        });
    }

    uint16_t deltaMicros() {
        return lastDt;
    }

    uint16_t currentMicros()
    {
        uint16_t returnCnt;
        ATOMIC_BLOCK(ATOMIC_FORCEON) {
            returnCnt = _impl_dt;
        }
        return returnCnt;
    }

    void pass() {
        ATOMIC_BLOCK(ATOMIC_FORCEON) {
            lastDt = _impl_dt;
        }
        _impl_dt = 0;
    }

private:
    uint16_t lastDt;
};


extern DeltaTimer deltaTimer;

namespace AvrLib {
    template<>
    struct TimerCounter4Interrupts<Main> : TimerCounter4Interrupts<void> {
        static void match6() {
            _impl_dt += 1;
        }
    };
}