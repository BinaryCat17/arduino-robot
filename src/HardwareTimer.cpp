#include "HardwareTimer.hpp"
#include "avr/io.h"

#if not defined(ARDUINO_LIB)
ISR(TIMER2_COMPA_vect) {
    ++timer2_millis;
}
#endif


volatile uint32_t timer2_millis = 0;

void HardwareTimer::enable() {
    static bool enabled = false;
    if(!enabled)
    {
        enabled = true;
    } else
    {
        return;
    }

#if not defined(ARDUINO_LIB)
    unsigned long ctc_match_overflow = ((16000000 / 1000) / 128);

    // CTC counter mode
    TCCR2A &= ~(1 << WGM20);
    TCCR2A |= (1 << WGM21);
    TCCR2A &= ~(1 << WGM22);

    // clear on compare
    TCCR2A |= (1 << COM2A1);
    TCCR2A &= ~(1 << COM2A0);

    // enable interruptions
    TIMSK2 |= (1 << OCIE2A);

    // 128 prescaling
    TCCR2B |= (1 << CS22) | (1 << CS20);

    TCNT1 = 0u;
    OCR2A = ctc_match_overflow;
#endif
}
