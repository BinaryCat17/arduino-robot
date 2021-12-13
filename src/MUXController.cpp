#include "MUXController.hpp"
#include <avr/io.h>
#include <assert.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t adc_data = 0;
volatile uint16_t data_accepted = false;

ISR(ADC_vect) {
    adc_data = ADCL | (ADCH << 8);
    data_accepted = true;
}

void MUXController::enable() {
    static bool isEnabled = false;
    if (!isEnabled) {
        // запускаем АЦП
        ADCSRA = 1 << ADEN;
        // запускаем преобразование
        ADCSRA |= 1 << ADSC;
        // запускаем непрерывные преобразования
        ADCSRA |= 1 << ADATE;
        // включаем прерывания
        ADCSRA |= 1 << ADIE;
        // опорное напряжение - напряжение питания AVCC
        ADMUX = 1 << REFS0;
    }
}

void MUXController::freq(MUXFreq freq) {
    switch (freq) {
        case MUXFreq::F8000000:
            ADCSRA |= (1 << ADPS0);
            break;
        case MUXFreq::F4000000:
            ADCSRA |= (1 << ADPS1);
            break;
        case MUXFreq::F2000000:
            ADCSRA |= (1 << ADPS1) | (1 << ADPS0);
            break;
        case MUXFreq::F1000000:
            ADCSRA |= (1 << ADPS2);
            break;
        case MUXFreq::F500000:
            ADCSRA |= (1 << ADPS2) | (1 << ADPS0);
            break;
        case MUXFreq::F250000:
            ADCSRA |= (1 << ADPS2) | (1 << ADPS1);
            break;
        case MUXFreq::F125000:
            ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
            break;
    }
}

uint16_t MUXController::read(Pin channel) {
    switch (channel) {
        case Pin::A0:
            ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | 1 << MUX3 | (1 << MUX4) |
                    (1 << MUX5));
            break;
        case Pin::A1:
            ADMUX &= ~((1 << MUX1) | (1 << MUX2) | (1 << MUX3) | (1 << MUX4) | (1 << MUX5));
            ADMUX |= (1 << MUX0);
            break;
        case Pin::A2:
            ADMUX &= ~((1 << MUX0) | (1 << MUX2) | (1 << MUX3) | (1 << MUX4) | (1 << MUX5));
            ADMUX |= (1 << MUX1);
            break;
        case Pin::A3:
            ADMUX &= ~((1 << MUX2) | (1 << MUX3) | (1 << MUX4) | (1 << MUX5));
            ADMUX |= (1 << MUX1) | (1 << MUX0);
            break;
        default:
            assert(0 && "selected pin is not analog");
    }

    while (!data_accepted);
    data_accepted = false;

    return adc_data;
}
