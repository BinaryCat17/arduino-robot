#include "HardwareTimer.hpp"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/atomic.h"

volatile uint32_t timer2_millis = 0;

ISR(TIMER2_COMPA_vect) {
    timer2_millis++;
}

uint32_t HardwareTimer::millis() {
    unsigned long millis_return;

    // Ensure this cannot be disrupted
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        millis_return = timer2_millis;
    }
    return millis_return;
}

void HardwareTimer::enable() {
    if(instance().m_enabled)
    {
        return;
    }
    instance().m_enabled = true;

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
}

HardwareTimer &HardwareTimer::instance() {
    static HardwareTimer timer;
    return timer;
}
