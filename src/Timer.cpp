#include "Timer.hpp"
#include <avr/io.h>
#include <assert.h>

void TimerMap::fillFactor(Pin pin, float factor) {
    if (!instance().enabledPins[static_cast<int>(pin)]) {
        instance().enabledPins[static_cast<int>(pin)] = true;
        PinMap::direction(pin, Dir::Out);
    }

    switch (pin) {
        case Pin::D2:
            OCR3B = static_cast<int>(instance().timerTops[2] * factor);
            break;
        case Pin::D3:
            OCR3C = static_cast<int>(instance().timerTops[2] * factor);
            break;
        case Pin::D5:
            OCR3A = static_cast<int>(instance().timerTops[2] * factor);
            break;
        default:
            assert(0 && "invalid timer pin");
    }
}

void TimerMap::enable(Timer t, uint16_t freq) {
    assert(freq <= 1.0f);

    switch (t) {
        case Timer::T3P235:
            // выбираем режим работы таймера Fast PWM (TOP = ICR3)
            TCCR3A = (1 << WGM31);
            TCCR3B = (1 << WGM32) | (1 << WGM33);
            // неинвертированный режим работы на каналах B и C
            TCCR3A |= (1 << COM3B1) | (1 << COM3C1);

            TCCR3A |= (1 << COM3B1) | (1 << COM3C1);

            if (F_CPU / freq <= 65535) {
                TCCR3B |= 1 << CS30;
                ICR3 = F_CPU / freq;
                instance().timerTops[2] = F_CPU / freq;
            } else if (F_CPU / freq <= 65535 * 8) {
                TCCR3B |= 1 << CS31;
                ICR3 = F_CPU / freq / 8;
                instance().timerTops[2] = F_CPU / freq / 8;
            } else {
                assert(0 && "invalid frequency");
            }
            break;
        default:
            assert(0 && "invalid timer pin");
    }
}

TimerMap &TimerMap::instance() {
    static TimerMap i;
    return i;
}
