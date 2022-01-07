#pragma once

#include "DPinMap.hpp"
#include <avr/io.h>
#include "util/atomic.h"

namespace AvrLib {
    enum class APin {
        A0,
        A1,
        A2,
        A3,
    };

    enum class ADCFreq {
        F8000000,
        F4000000,
        F2000000,
        F1000000,
        F500000,
        F250000,
        F125000,
    };

    extern volatile uint16_t _impl_adc_data;

    class ADCController {
    public:
        void enable();

        template<ADCFreq f>
        void freq() {
            if constexpr(f == ADCFreq::F8000000) {
                BitVal(ADCSRA).on<ADPS0>().off<ADPS1, ADPS2>();
            } else if constexpr(f == ADCFreq::F4000000) {
                BitVal(ADCSRA).on<ADPS1>().off<ADPS0, ADPS2>();
            } else if constexpr(f == ADCFreq::F2000000) {
                BitVal(ADCSRA).on<ADPS0, ADPS1>().off<ADPS2>();
            } else if constexpr(f == ADCFreq::F1000000) {
                BitVal(ADCSRA).on<ADPS2>().off<ADPS0, ADPS1>();
            } else if constexpr(f == ADCFreq::F500000) {
                BitVal(ADCSRA).on<ADPS0, ADPS2>().off<ADPS1>();
            } else if constexpr(f == ADCFreq::F250000) {
                BitVal(ADCSRA).on<ADPS1, ADPS2>().off<ADPS0>();
            } else if constexpr(f == ADCFreq::F125000) {
                BitVal(ADCSRA).on<ADPS0, ADPS1, ADPS2>();
            }
        }

        // производим тяжелые вычисления во время компиляции
        template<APin p>
        uint16_t read() {
            if constexpr (p == APin::A0) {
                BitVal(ADMUX).off<MUX0, MUX1, MUX2, MUX3, MUX4, MUX5>();
            } else if constexpr(p == APin::A1) {
                BitVal(ADMUX).off<MUX1, MUX2, MUX3, MUX4, MUX5>().on<MUX0>();
            } else if constexpr(p == APin::A2) {
                BitVal(ADMUX).off<MUX0, MUX2, MUX3, MUX4, MUX5>().on<MUX1>();
            } else if constexpr(p == APin::A3) {
                BitVal(ADMUX).off<MUX2, MUX3, MUX4, MUX5>().on<MUX0, MUX1>();
            } else {
                static_assert("selected pin is not analog");
            }

            // Дожидаемся срабатывания прерывания и блокируем другие,
            // чтобы не вернуть значение, пока оно еще на записалось
            unsigned long data_return = 0;
            ATOMIC_BLOCK(ATOMIC_FORCEON) {
                data_return = _impl_adc_data;
            }

            return data_return;
        }
    };

    extern ADCController adc;
}
