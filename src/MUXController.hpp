#pragma once

#include "PinMap.hpp"
#include <avr/io.h>
#include "util/atomic.h"

enum class MUXFreq {
    F8000000,
    F4000000,
    F2000000,
    F1000000,
    F500000,
    F250000,
    F125000,
};

extern volatile uint16_t adc_data;

class MUXController {
public:
    static void enable();

    static void freq(MUXFreq freq);

    // производим тяжелые вычисления во время компиляции
    template<Pin p>
    static uint16_t read() {
        if constexpr (p == Pin::A0) {
            PinMap::setBits<false, MUX0, MUX1, MUX2, MUX3, MUX4, MUX5>(ADMUX);
        } else if constexpr(p == Pin::A1) {
            PinMap::setBits<false, MUX1, MUX2, MUX3, MUX4, MUX5>(ADMUX);
            PinMap::setBits<true, MUX0>(ADMUX);
        } else if constexpr(p == Pin::A2) {
            PinMap::setBits<false, MUX0, MUX2, MUX3, MUX4, MUX5>(ADMUX);
            PinMap::setBits<true, MUX1>(ADMUX);
        } else if constexpr(p == Pin::A3) {
            PinMap::setBits<false, MUX2, MUX3, MUX4, MUX5>(ADMUX);
            PinMap::setBits<true, MUX1, MUX0>(ADMUX);
        } else
        {
            static_assert("selected pin is not analog");
        }

        // Дожидаемся срабатывания прерывания и блокируем другие,
        // чтобы не вернуть значение, пока оно еще на записалось
        unsigned long data_return = 0;
        ATOMIC_BLOCK(ATOMIC_FORCEON) {
            data_return = adc_data;
        }

        return data_return;
    }
};


