#pragma once
#include "PinMap.hpp"

enum class MUXFreq {
    F8000000,
    F4000000,
    F2000000,
    F1000000,
    F500000,
    F250000,
    F125000,
};

class MUXController {
public:
    static void enable();

    static void freq(MUXFreq freq);

    static uint16_t read(Pin channel);
};
