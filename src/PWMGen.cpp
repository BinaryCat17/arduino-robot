#include "PWMGen.hpp"
#include <avr/io.h>
#include <assert.h>
#include "App.hpp"
#include "avr/interrupt.h"

void PWMGen::fillFactor(Pin pin, float factor) {
    // чтобы не изменять режим пина лишний раз
    if (!instance().enabledPins[static_cast<int>(pin)]) {
        App::print("Pin enabled: ");
        App::println(static_cast<uint32_t>(pin));
        instance().enabledPins[static_cast<int>(pin)] = true;
        PinMap::direction(pin, PinDir::Out);
    }

    App::print("Fill factor: ");
    App::println(factor);

    // можно настраивать коэффициент заполнения на каждой ножке отдельно
    switch (pin) {
        case Pin::D2:
            OCR3B = static_cast<uint16_t>(instance().timerTops[2] * factor);
            break;
        case Pin::D3:
            OCR3C = static_cast<uint16_t>(instance().timerTops[2] * factor);
            break;
        case Pin::D5:
            OCR3A = static_cast<uint16_t>(instance().timerTops[2] * factor);
            break;
        default:
            assert(0 && "invalid timer pin");
    }
}

void PWMGen::enable(PWMTimer t, uint16_t freq) {
    cli();
    switch (t) {
        case PWMTimer::T3P235:
            // выбираем режим работы таймера Fast PWM (TOP = ICR3)
            TCCR3A = (1 << WGM31);
            TCCR3B = (1 << WGM32) | (1 << WGM33);
            // неинвертированный режим работы на каналах B и C
            TCCR3A |= (1 << COM3B1) | (1 << COM3C1);

            if (F_CPU / freq <= 65535) {
                App::println("Prescaling 1");
                TCCR3B |= 1 << CS30;
                ICR3 = F_CPU / freq;
                instance().timerTops[2] = F_CPU / freq;
            } else if (F_CPU / freq <= 65535 * 8) {
                App::println("Prescaling 8");
                TCCR3B |= 1 << CS31;
                ICR3 = F_CPU / freq / 8;
                instance().timerTops[2] = F_CPU / freq / 8;
            } else {
                // нужно добавить еще предделители
                assert(0 && "invalid frequency");
            }
            break;
        default:
            // нужно добавить еще таймеров
            assert(0 && "invalid timer pin");
    }
    sei();
}

PWMGen &PWMGen::instance() {
    static PWMGen i;
    return i;
}
