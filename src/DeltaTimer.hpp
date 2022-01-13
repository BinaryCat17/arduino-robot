#pragma once

#include "AVRLib/HardwareTimer.hpp"
#include "util/atomic.h"

using namespace AvrLib;

class Main;

enum class Precision {
    Millis,
    Micros,
};

#define MAKE_DELTA_TIMER(NUM) \
class DeltaTimer##NUM {\
public:\
    void enable(Precision pr) {\
        AvrLib::enableOnce([this, pr] {               \
            if (pr == Precision::Micros) \
            {\
                AvrLib::timerNormal##NUM.freq<PreScaling::P8>();\
            } else if (pr == Precision::Millis)    \
            {\
                AvrLib::timerNormal##NUM.freq<PreScaling::P1024>();\
            }\
            AvrLib::timerNormal##NUM.interrupt<true>();\
            AvrLib::timerNormal##NUM.enable();\
            mPr = pr;                \
        });\
    }\
\
    uint16_t delta() {\
        return mLastDt;\
    }\
\
    uint16_t current()\
    {                   \
        auto cnt = timerNormal##NUM.cnt();\
        if (mPr == Precision::Micros) \
        {               \
            return cnt / 2;\
        } else\
        {\
            return cnt * 64 / 1000; \
        } \
    }\
\
    uint16_t pass() {\
        mLastDt = current();\
        timerNormal##NUM.release();                   \
        return mLastDt;\
    }\
\
private:\
    uint16_t mLastDt;\
    Precision mPr;\
};\
extern DeltaTimer##NUM deltaTimer##NUM;

MAKE_DELTA_TIMER(1)
MAKE_DELTA_TIMER(3)
MAKE_DELTA_TIMER(4)
MAKE_DELTA_TIMER(5)

