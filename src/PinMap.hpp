#pragma once

#include <avr/io.h>

const bool PinIn = false;
const bool PinOut = true;

const bool PinLow = false;
const bool PinHigh = true;

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
    template<bool d, Pin p>
    static void direction() {
        if constexpr(p == Pin::D2 || p == Pin::D3 || p == Pin::D5) {
            setBits<d, convertPin<p>()>(DDRE);
        } else if constexpr(p == Pin::D51 || p == Pin::D13) {
            setBits<d, convertPin<p>()>(DDRB);
        } else if constexpr(p == Pin::D49) {
            setBits<d, convertPin<p>()>(DDRL);
        } else {
            static_assert("pin is not digital");
        }
    }

    template<bool d, Pin p>
    static void write() {
        if constexpr(p == Pin::D2 || p == Pin::D3 || p == Pin::D5) {
            setBits<d, convertPin<p>()>(PORTE);
        } else if constexpr(p == Pin::D51 || p == Pin::D13) {
            setBits<d, convertPin<p>()>(PORTB);
        } else if constexpr(p == Pin::D49) {
            setBits<d, convertPin<p>()>(PORTL);
        } else {
            static_assert("pin is not digital");
        }
    }

    template<Pin p>
    static bool read() {
        if constexpr(p == Pin::D2 || p == Pin::D3 || p == Pin::D5) {
            return getBit<convertPin<p>()>(PINE);
        } else if constexpr(p == Pin::D51 || p == Pin::D13) {
            return getBit<convertPin<p>()>(PINB);
        } else if constexpr(p == Pin::D49) {
            return getBit<convertPin<p>()>(PINL);
        } else {
            static_assert("pin is not digital");
        }
    }

    template<Pin p>
    constexpr static uint8_t convertPin() {
        switch (p) {
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
                static_assert("it is not digital pin");
        }
    }

    template<bool v, int... p>
    static void setBits(volatile uint8_t &port) {
        constexpr uint8_t mask = ((1 << p) | ...);
        if constexpr(v) {
            port |= mask;
        } else {
            port &= ~mask;
        }
    }

    template<int p>
    static bool getBit(volatile uint8_t &port) {
        if (port & (1 << p)) {
            return true;
        } else {
            return false;
        }
    }
};