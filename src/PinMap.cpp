#include "PinMap.hpp"
#include <avr/io.h>
#include "assert.h"

void PinMap::direction(Pin p, PinDir val) {
    switch (p) {
        case Pin::D2:
        case Pin::D3:
        case Pin::D5:
            setBit(DDRE, p, val);
            break;
        case Pin::D13:
            setBit(DDRB, p, val);
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
        case Pin::D13:
            return PB7;
        default:
            assert(0 && "it is not digital pin");
    }
}

void PinMap::setBit(volatile uint8_t &port, Pin pin, PinDir val) {
    if (val == PinDir::Out) {
        port |= (1 << convertPin(pin));
    } else {
        port &= ~(1 << convertPin(pin));
    }
}

PinValue PinMap::getBit(volatile uint8_t &port, Pin pin) {
    if(port & (1 << convertPin(pin)))
    {
        return PinValue::High;
    } else
    {
        return PinValue::Low;
    }
}

void PinMap::write(Pin p, PinValue val) {
    switch (p) {
        case Pin::D2:
        case Pin::D3:
        case Pin::D5:
            setBit(PORTE, p, static_cast<PinDir>(val));
            break;
        case Pin::D13:
            setBit(PORTB, p, static_cast<PinDir>(val));
            break;
        default:
            assert(0 && "pin in not digital");
    }
}

PinValue PinMap::read(Pin p) {
    switch (p) {
        case Pin::D2:
        case Pin::D3:
        case Pin::D5:
            return getBit(PORTE, p);
        case Pin::D13:
            return getBit(PORTB, p);
        default:
            assert(0 && "pin in not digital");
    }
}
