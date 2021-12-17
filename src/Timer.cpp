#include "Timer.hpp"
#include "HardwareTimer.hpp"

void Timer::start() {
    HardwareTimer::enable();
}

void Timer::interval(uint32_t ms) {
    m_interval = ms;
}

bool Timer::event() {
    uint32_t val = HardwareTimer::millis();


    if (val - m_prev > m_interval) {
        m_prev = val;
        return true;
    }

    return false;
}