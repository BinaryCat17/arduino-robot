#include "ADCController.hpp"
#include <avr/io.h>
#include "SerialMonitor.hpp"
#include "Util/Utils.hpp"

ISR(ADC_vect) {
    AvrLib::_impl_adc_data = ADCL | (ADCH << 8);
}

namespace AvrLib {
    void ADCController::enable() {
        enableOnce([] {
            // запускаем АЦП
            ADCSRA = 1 << ADEN;
            // запускаем преобразование
            ADCSRA |= 1 << ADSC;
            // запускаем непрерывные преобразования
            ADCSRA |= 1 << ADATE;
            // включаем прерывания
            ADCSRA |= 1 << ADIE;
            // опорное напряжение - напряжение питания AVCC
            ADMUX |= 1 << REFS0;

            monitor.println("MUXController enabled");
        });
    }

    volatile uint16_t _impl_adc_data = 0;

    ADCController adc = {};
}
