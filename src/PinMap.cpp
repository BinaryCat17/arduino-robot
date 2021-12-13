#include "PinMap.hpp"
#include <avr/io.h>
#include "assert.h"

void PinMap::direction(Pin p, Dir val) {
    switch (p) {
        case Pin::D2:
        case Pin::D3:
        case Pin::D5:
            setBit(DDRE, p, val);
            DDRE |= (1 << convertPin(p));
            break;
        default:
            assert(0 && "pin in not digital");
    }
}

uint8_t PinMap::convertPin(Pin pin) {
    switch (pin) {
        case Pin::D2:
            return PE4;
        case Pin::D3:
            return PE5;
        case Pin::D5:
            return PE3;
        default:
            assert(0 && "it is not digital pin");
    }
}

void PinMap::setBit(volatile uint8_t &port, Pin pin, Dir val) {
    if(val == Dir::Out)
    {
        port |= (1 << convertPin(pin));
    } else
    {
        port &= ~(1 << convertPin(pin));
    }
}
