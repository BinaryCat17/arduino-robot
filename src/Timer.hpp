#pragma once

#include "PinMap.hpp"

enum class Timer {
    T3P235,
};

class TimerMap {
public:
    static void fillFactor(Pin pin, float factor);

    static void enable(Timer t, uint16_t freq);

private:
    static TimerMap &instance();

    bool enabledPins[static_cast<int>(Pin::Count)] = {};
    bool timerTops[3] = {};
};
