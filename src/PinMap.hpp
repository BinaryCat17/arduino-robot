#pragma once
#include <stdint.h>

enum class PinDir
{
    In = 0,
    Out = 1,
};

enum class PinValue
{
    Low = 0,
    High = 1,
};

enum class Pin {
    A0,
    A1,
    A2,
    A3,
    D2,
    D3,
    D5,
    D13,
    D49,
    D51,
    Count,
};

class PinMap {
public:
    static void direction(Pin p, PinDir val);

    static void write(Pin p, PinValue val);

    static PinValue read(Pin p);

private:
    static uint8_t convertPin(Pin pin);

    inline static void setBit(volatile uint8_t &port, Pin pin, PinDir val) {
        if (val == PinDir::Out) {
            port |= (1 << convertPin(pin));
        } else {
            port &= ~(1 << convertPin(pin));
        }
    }

    inline static PinValue getBit(volatile uint8_t &port, Pin pin) {
        if(port & (1 << convertPin(pin)))
        {
            return PinValue::High;
        } else
        {
            return PinValue::Low;
        }
    }
};