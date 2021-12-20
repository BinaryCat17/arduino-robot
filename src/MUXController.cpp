#include "MUXController.hpp"
#include <avr/io.h>
#include "App.hpp"

ISR(ADC_vect) {
    adc_data = ADCL | (ADCH << 8);
}

void MUXController::enable() {
    static bool enabled = false;
    if(!enabled)
    {
        enabled = true;
    } else
    {
        return;
    }

    App::println("MUXController enabled");

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
}

void MUXController::freq(MUXFreq freq) {
    switch (freq) {
        case MUXFreq::F8000000:
            return PinMap::setBits<true, ADPS0>(ADCSRA);
        case MUXFreq::F4000000:
            return PinMap::setBits<true, ADPS1>(ADCSRA);
        case MUXFreq::F2000000:
            return PinMap::setBits<true, ADPS0, ADPS1>(ADCSRA);
        case MUXFreq::F1000000:
            return PinMap::setBits<true, ADPS2>(ADCSRA);
        case MUXFreq::F500000:
            return PinMap::setBits<true, ADPS0, ADPS2>(ADCSRA);
        case MUXFreq::F250000:
            return PinMap::setBits<true, ADPS1, ADPS2>(ADCSRA);
        case MUXFreq::F125000:
            return PinMap::setBits<true, ADPS0, ADPS1, ADPS2>(ADCSRA);
    }
}

volatile uint16_t adc_data = 0;
