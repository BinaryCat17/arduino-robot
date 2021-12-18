#pragma once
#include <stdint.h>

class Timer
{
public:
    void start(uint32_t interval, uint32_t delay = 0);

    bool event();

private:
    uint32_t m_delay = 0;
    uint32_t m_interval = 0;
    uint32_t m_cnt = 0;
};
