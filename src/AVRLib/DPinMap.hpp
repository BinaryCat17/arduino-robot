#pragma once

#include <avr/io.h>
#include "Util/Utils.hpp"
#include "util/atomic.h"

namespace AvrLib {
    const bool PinIn = false;
    const bool PinOut = true;

    const bool PinLow = false;
    const bool PinHigh = true;

    enum class DPin {
        D2,
        D3,
        D5,
        D11,
        D12,
        D13,
        D18,
        D19,
        D20,
        D21,
        D49,
        D51,
    };

    template<DPin p>
    static constexpr uint8_t convertPin() {
        switch (p) {
            case DPin::D2:
                return PE4;
            case DPin::D3:
                return PE5;
            case DPin::D5:
                return PE3;
            case DPin::D11:
                return PB5;
            case DPin::D12:
                return PB6;
            case DPin::D13:
                return PB7;
            case DPin::D18:
                return PD3;
            case DPin::D19:
                return PD2;
            case DPin::D20:
                return PD1;
            case DPin::D21:
                return PD0;
            case DPin::D49:
                return PL0;
            case DPin::D51:
                return PB2;
            default:
                static_assert("it is not digital pin");
        }
    }

    template<bool d, DPin p>
    void pinDirection() {
        if constexpr(p == DPin::D2 || p == DPin::D3 || p == DPin::D5) {
            setBits<d, convertPin<p>()>(DDRE);
        } else if constexpr(p == DPin::D51 || p == DPin::D13 || p == DPin::D12 || p == DPin::D11) {
            setBits<d, convertPin<p>()>(DDRB);
        } else if constexpr(p == DPin::D18 || p == DPin::D19 || p == DPin::D20 || p == DPin::D21) {
            setBits<d, convertPin<p>()>(DDRD);
        } else if constexpr(p == DPin::D49) {
            setBits<d, convertPin<p>()>(DDRL);
        } else {
            static_assert("pin is not digital");
        }
    }

    template<bool d, DPin p>
    void pinWrite() {
        if constexpr(p == DPin::D2 || p == DPin::D3 || p == DPin::D5) {
            setBits<d, convertPin<p>()>(PORTE);
        } else if constexpr(p == DPin::D51 || p == DPin::D13 || p == DPin::D12 || p == DPin::D11) {
            setBits<d, convertPin<p>()>(PORTB);
        } else if constexpr(p == DPin::D18 || p == DPin::D19 || p == DPin::D20 || p == DPin::D21) {
            setBits<d, convertPin<p>()>(PORTD);
        } else if constexpr(p == DPin::D49) {
            setBits<d, convertPin<p>()>(PORTL);
        } else {
            static_assert("pin is not digital");
        }
    }

    template<DPin p>
    bool pinRead() {
        if constexpr(p == DPin::D2 || p == DPin::D3 || p == DPin::D5) {
            return getBit<convertPin<p>()>(PINE);
        } else if constexpr(p == DPin::D51 || p == DPin::D13 || p == DPin::D12 || p == DPin::D11) {
            return getBit<convertPin<p>()>(PINB);
        } else if constexpr(p == DPin::D18 || p == DPin::D19 || p == DPin::D20 || p == DPin::D21) {
            return getBit<convertPin<p>()>(PIND);
        } else if constexpr(p == DPin::D49) {
            return getBit<convertPin<p>()>(PINL);
        } else {
            static_assert("pin is not digital");
        }
    }
}
