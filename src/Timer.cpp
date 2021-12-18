#include "Timer.hpp"
#include "HardwareTimer.hpp"

void Timer::start(uint32_t interval, uint32_t delay) {
    HardwareTimer::enable();
    m_delay = HardwareTimer::millis() + delay;
    m_interval = interval;
    m_cnt = 0;
}

bool Timer::event() {
    uint32_t val = HardwareTimer::millis();
    if(val >= m_delay + m_cnt * m_interval)
    {
        ++m_cnt;
        return true;
    }

    return false;
}
