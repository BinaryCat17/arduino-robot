#pragma once
#include "HardwareTimer.hpp"

class Timer
{
public:
    void start(uint32_t interval, uint32_t delay = 0)
    {
        HardwareTimer::enable();
        m_delay = HardwareTimer::millis() + delay;
        m_interval = interval;
        m_cnt = 0;
    }

    bool event()
    {
        uint32_t const val = HardwareTimer::millis();
        if(val >= m_delay + m_cnt * m_interval)
        {
            ++m_cnt;
            return true;
        }

        return false;
    }

private:
    uint32_t m_delay = 0;
    uint32_t m_interval = 0;
    uint32_t m_cnt = 0;
};
