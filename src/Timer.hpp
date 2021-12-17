#pragma once
#include <stdint.h>

class Timer
{
public:
    void start();

    void interval(uint32_t ms);

    bool event();

private:
    uint16_t m_prev = 0;
    uint16_t m_interval = 0;
};
