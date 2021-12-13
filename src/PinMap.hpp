#pragma once
#include <stdint.h>

enum class Dir
{
    In = 0,
    Out = 1,
};

enum class Pin {
    A0,
    A1,
    A2,
    A3,
    D2,
    D3,
    D5,
    Count,
};

class PinMap {
public:
    static void direction(Pin p, Dir val);

private:
    static uint8_t convertPin(Pin pin);

    static void setBit(volatile uint8_t& port, Pin pin, Dir val);
};