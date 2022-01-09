#include "HardwareTimer.hpp"
#include "Main.hpp"

#define MAKE_HARDWARE_TIMER_IMPL_16_BIT(NUM, AN, BN, CN) \
ISR(TIMER##NUM##_COMPA_vect) {\
    AvrLib::TimerCounter##NUM##Interrupts<Main>::match##AN();\
}\
\
ISR(TIMER##NUM##_COMPB_vect) {\
    AvrLib::TimerCounter##NUM##Interrupts<Main>::match##BN();\
}\
\
ISR(TIMER##NUM##_COMPC_vect) {\
    AvrLib::TimerCounter##NUM##Interrupts<Main>::match##CN();\
}\
\
AvrLib::HardwareTimerPWM##NUM AvrLib::timerPWM##NUM = {};\
AvrLib::HardwareTimerCounter##NUM AvrLib::timerCounter##NUM = {};\
\
void AvrLib::HardwareTimerCounter##NUM::enable() {\
            if(timerPWM##NUM.isInited())                \
            {                                       \
                monitor.println("Timer " #NUM " already initialized as PWM");\
            } else if(!inited) {\
                BitVal(TCCR##NUM##A).off<WGM##NUM##0, WGM##NUM##1>();\
                BitVal(TCCR##NUM##B).on<WGM##NUM##2>().off<WGM##NUM##3>();\
                inited = true;                      \
                monitor.println("Counter timer " #NUM " initialized");\
            }\
        }\
\
void AvrLib::HardwareTimerPWM##NUM::enable() {\
            if(timerCounter##NUM.isInited())\
            {\
                monitor.println("Timer " #NUM " already initialized as Counter");\
            } else if(!inited) {                         \
                BitVal(TCCR##NUM##A).off<WGM##NUM##0>().on<WGM##NUM##1>();\
                BitVal(TCCR##NUM##B).on<WGM##NUM##2, WGM##NUM##3>();\
                inited = true;                      \
                monitor.println("PWM timer " #NUM " initialized");\
            }\
        }

MAKE_HARDWARE_TIMER_IMPL_16_BIT(1, 11, 12, 13)

MAKE_HARDWARE_TIMER_IMPL_16_BIT(3, 5, 2, 3)

MAKE_HARDWARE_TIMER_IMPL_16_BIT(4, 6, 7, 8)

MAKE_HARDWARE_TIMER_IMPL_16_BIT(5, 46, 45, 44)
