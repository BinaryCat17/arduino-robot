#pragma once
#include "math.h"

using Custom = void;

namespace AvrLib {

    template<typename FnT>
    void enableOnce(FnT &&f) {
        static bool enabled = false;

        if (!enabled) {
            enabled = true;
            f();
        }
    }

    template<bool v, int... p>
    void setBits(volatile uint8_t &port) {
        constexpr uint8_t mask = ((1 << p) | ...);
        if constexpr(v) {
            port |= mask;
        } else {
            port &= ~mask;
        }
    }

    template<int... p>
    void setBits(volatile uint8_t &port, bool v) {
        constexpr uint8_t mask = ((1 << p) | ...);
        if (v) {
            port |= mask;
        } else {
            port &= ~mask;
        }
    }

    template<int p>
    bool getBit(volatile uint8_t const &port) {
        if (port & (1 << p)) {
            return true;
        } else {
            return false;
        }
    }

    struct BitVal {
        explicit BitVal(volatile uint8_t &port) : mPort(port) {}

        template<int... p>
        BitVal on() {
            constexpr uint8_t mask = ((1 << p) | ...);
            mPort |= mask;
            return *this;
        }

        template<int... p>
        BitVal off() {
            constexpr uint8_t mask = ((1 << p) | ...);
            mPort &= ~mask;
            return *this;
        }

        volatile uint8_t &mPort;
    };

    template<typename T>
    bool near(T target, T distance, T v)
    {
        return ::fabs(v - target) < distance;
    }
}
