#include "PinMap.hpp"
#include <avr/io.h>
#include "assert.h"
#include "App.hpp"

void PinMap::direction(Pin p, PinDir val) {
    switch (p) {
        case Pin::D2:
        case Pin::D3:
        case Pin::D5:
            setBit(DDRE, p, val);
            break;
        case Pin::D51:
        case Pin::D13:
            setBit(DDRB, p, val);
            break;
        case Pin::D49:
            setBit(DDRL, p, val);
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
        case Pin::D49:
            return PL0;
        case Pin::D51:
            return PB2;
        default:
            assert(0 && "it is not digital pin");
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
        case Pin::D51:
            setBit(PORTB, p, static_cast<PinDir>(val));
            break;
        case Pin::D49:
            setBit(PORTL, p, static_cast<PinDir>(val));
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
        case Pin::D51:
            return getBit(PORTB, p);
        case Pin::D49:
            return getBit(PORTL, p);
        default:
            assert(0 && "pin in not digital");
    }
}
