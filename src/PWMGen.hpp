#pragma once

#include "PinMap.hpp"

enum class PWMTimer {
    T3P235,
};

class PWMGen {
public:
    static void fillFactor(Pin pin, float factor);

    static void enable(PWMTimer t, uint16_t freq);

private:
    static PWMGen &instance();

    bool enabledPins[static_cast<int>(Pin::Count)] = {};
    uint16_t timerTops[3] = {};
};
