#pragma once

#include "PinMap.hpp"
#include "App.hpp"
#include "avr/interrupt.h"
#include "assert.h"
//#define ARDUINO_LIB
#if defined(ARDUINO_LIB)
#include "Arduino.h"
#endif

const int32_t PWMRange = 10000;

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

#if not defined(ARDUINO_LIB)
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
                icr_val = F_CPU / freq;
            } else if (F_CPU / freq <= 65535 * 8) {
                App::println("Prescaling 8");
                TCCR3B |= 1 << CS31;
                icr_val = F_CPU / freq / 8;
            } else {
                assert("invalid frequency");
            }

            ICR3 = icr_val;
        }
        sei();
#endif
    }

    template<Pin p>
    static void fillFactor(uint16_t factor)
    {
#if defined(ARDUINO_LIB)
        analogWrite(static_cast<int>(p), factor / 39);
#else
        auto const val = static_cast<uint16_t>(icr_val * factor / PWMRange);

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
#endif
    }

private:
    static uint32_t icr_val;
};

template<PWMTimer t>
uint32_t PWMGen<t>::icr_val = 0;
