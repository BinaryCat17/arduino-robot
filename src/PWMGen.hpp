#pragma once

#include "PinMap.hpp"
#include "App.hpp"
#include "avr/interrupt.h"
#include "assert.h"

enum class PWMTimer {
    T3P235,
};

template<PWMTimer t>
class PWMGen {
public:
    static void enable(uint16_t freq)
    {
        static bool enabled = false;

        if(!enabled)
        {
            enabled = true;
        } else
        {
            return;
        }

        cli();
        if constexpr(t == PWMTimer::T3P235)
        {
            TCCR3A = (1 << WGM31);
            TCCR3B = (1 << WGM32) | (1 << WGM33);
            // неинвертированный режим работы на каналах B и C
            TCCR3A |= (1 << COM3B1) | (1 << COM3C1);

            if (F_CPU / freq <= 65535) {
                App::println("Prescaling 1");
                TCCR3B |= 1 << CS30;
                icr_val = F_CPU / freq <= 65535;
            } else if (F_CPU / freq <= 65535 * 8) {
                App::println("Prescaling 8");
                TCCR3B |= 1 << CS31;
                icr_val = F_CPU / freq / 8 <= 65535;
            } else {
                assert("invalid frequency");
            }

            ICR3 = icr_val;
        }
        sei();
    }

    // factor 0 to 1000
    template<Pin p>
    static void fillFactor(uint16_t factor)
    {
        auto val = static_cast<uint16_t>(icr_val * factor / 1000);

        if constexpr(p == Pin::D2 && t == PWMTimer::T3P235)
        {
            OCR3B = val;
        } else if constexpr(p == Pin::D3 && t == PWMTimer::T3P235)
        {
            OCR3C = val;
        } else if constexpr(p == Pin::D5 && t == PWMTimer::T3P235)
        {
            OCR3A = val;
        } else
        {
            static_assert("invalid timer pin");
        }
    }

private:
    static uint32_t icr_val;
};

template<PWMTimer t>
uint32_t PWMGen<t>::icr_val = 0;
