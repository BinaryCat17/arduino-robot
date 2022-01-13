#pragma once

#include "DPinMap.hpp"
#include "SerialMonitor.hpp"
#include "Util/Utils.hpp"
#include "avr/interrupt.h"

namespace AvrLib {
    enum class CompareMatch {
        None = 0,
        Toggle = 1,
        Clear = 2,
        Set = 3,
    };

    enum class PWMOutMode {
        Disconnected = 0,
        NonInverting = 2,
        Inverting = 3,
    };

    enum class PreScaling : uint32_t {
        Auto = 0,
        P1 = 16000000,
        P8 = 2000000,
        P64 = 250000,
        P256 = 62500,
        P1024 = 15625,
    };

#define MAKE_HARDWARE_TIMER_16_BIT(NUM, AN, BN, CN) \
    class HardwareTimer16Bit##NUM##Base { \
protected:\
\
        template<uint32_t fr, PreScaling ps> \
        uint16_t freq(uint16_t volatile &reg) { \
            return topImpl<F_CPU / fr, ps>(reg); \
        }\
\
        template<uint32_t millis, PreScaling ps>\
        uint16_t period(uint16_t volatile &reg) {\
            return topImpl<F_CPU * millis / 1000, ps>(reg);\
        }\
\
        template<CompareMatch mode>\
        void modeA() {\
            if constexpr(mode == CompareMatch::None) {\
                BitVal(TCCR##NUM##A).off<COM##NUM##A0, COM##NUM##A1>();\
            } else if constexpr(mode == CompareMatch::Toggle) {\
                BitVal(TCCR##NUM##A).on<COM##NUM##A0>().off<COM##NUM##A1>(); \
            } else if constexpr(mode == CompareMatch::Clear) {\
                BitVal(TCCR##NUM##A).on<COM##NUM##A1>().off<COM##NUM##A0>();\
            } else if (mode == CompareMatch::Set) {\
                BitVal(TCCR##NUM##A).on<COM##NUM##A1, COM##NUM##A0>();\
            }\
        }\
\
        template<bool enable>\
        void interruptMatchC() {\
            setBits<enable, OCIE##NUM##C>(TIMSK##NUM);\
        }\
\
        template<bool enable>\
        void interruptMatchA() {\
            setBits<enable, OCIE##NUM##A>(TIMSK##NUM);\
        }                                           \
                                                    \
         template<bool enable>\
        void interruptMatchB() {\
            setBits<enable, OCIE##NUM##B>(TIMSK##NUM);\
        }\
\
        template<bool enable>\
        void interruptOverflow() {\
            setBits<enable, TOIE##NUM>(TIMSK##NUM);\
        }                                           \
                                                    \
        template<bool enable>\
        void interruptInputCapture() {\
            setBits<enable, ICIE##NUM>(TIMSK##NUM);\
        }\
\
        template<CompareMatch mode>\
        void modeB() {\
            if constexpr(mode == CompareMatch::None) {\
                BitVal(TCCR##NUM##A).off<COM##NUM##B0, COM##NUM##B1>();\
            } else if constexpr(mode == CompareMatch::Toggle) {\
                BitVal(TCCR##NUM##A).on<COM##NUM##B0>().off<COM##NUM##B1>();\
            } else if constexpr(mode == CompareMatch::Clear) {\
                BitVal(TCCR##NUM##A).on<COM##NUM##B1>().off<COM##NUM##B0>();\
            } else if (mode == CompareMatch::Set) {\
                BitVal(TCCR##NUM##A).on<COM##NUM##B1, COM##NUM##B0>();\
            }\
        }\
\
        template<CompareMatch mode>\
        void modeC() {\
            if constexpr(mode == CompareMatch::None) {\
                BitVal(TCCR##NUM##A).off<COM##NUM##C0, COM##NUM##C1>();\
            } else if constexpr(mode == CompareMatch::Toggle) {\
                BitVal(TCCR##NUM##A).on<COM##NUM##C0>().off<COM##NUM##C1>();\
            } else if constexpr(mode == CompareMatch::Clear) {\
                BitVal(TCCR##NUM##A).on<COM##NUM##C1>().off<COM##NUM##C0>();\
            } else if (mode == CompareMatch::Set) {\
                BitVal(TCCR##NUM##A).on<COM##NUM##C1, COM##NUM##C0>();\
            }\
        }\
\
    private:\
        template<uint32_t top, PreScaling ps>\
        uint16_t topImpl(uint16_t volatile &reg) {\
            cli();\
            TCNT##NUM = 0;\
            uint16_t prescaling = 0;\
\
            if constexpr (top <= 0) {\
                monitor.println("Timer " #NUM " Off");\
                BitVal(TCCR##NUM##B).off<CS##NUM##0, CS##NUM##1, CS##NUM##2>();\
            } else if constexpr ((top <= 65535) && (ps == PreScaling::Auto || ps == PreScaling::P1)) {\
                monitor.println("Timer " #NUM " Prescaling 1");  \
                prescaling = 1;\
                BitVal(TCCR##NUM##B).off<CS##NUM##1, CS##NUM##2>().on<CS##NUM##0>();\
            } else if constexpr ((top <= 65535 * 8) && (ps == PreScaling::Auto || ps == PreScaling::P8)) {\
                monitor.println("Timer " #NUM " Prescaling 8");\
                prescaling = 8;\
                BitVal(TCCR##NUM##B).off<CS##NUM##0, CS##NUM##2>().on<CS##NUM##1>();\
            } else if constexpr ((top <= 65535 * 64) && (ps == PreScaling::Auto || ps == PreScaling::P64)) {\
                monitor.println("Timer " #NUM " Prescaling 64");\
                prescaling = 64;\
                BitVal(TCCR##NUM##B).off<CS##NUM##2>().on<CS##NUM##0, CS##NUM##1>();\
            } else if constexpr ((top <= 65535 * 256) && (ps == PreScaling::Auto || ps == PreScaling::P256)) {\
                monitor.println("Timer " #NUM " Prescaling 256");\
                prescaling = 256;\
                BitVal(TCCR##NUM##B).off<CS##NUM##0, CS##NUM##1>().on<CS##NUM##2>();\
            } else if constexpr ((top <= 65535 * 1024) && (ps == PreScaling::Auto || ps == PreScaling::P1024)) {\
                monitor.println("Timer " #NUM " Prescaling 1024");\
                prescaling = 1024;\
                BitVal(TCCR##NUM##B).off<CS##NUM##1>().on<CS##NUM##0, CS##NUM##2>();\
            } else {\
                monitor.println("Timer " #NUM " Maximum period is 243 seconds");\
            }\
            reg = top / prescaling;\
            monitor.println("Timer " #NUM " top: ", reg);\
            sei();\
            return prescaling;\
        }\
    };\
\
    class HardwareTimerNormal##NUM : HardwareTimer16Bit##NUM##Base {\
    public:\
        void enable();\
\
        template<PreScaling ps>\
        void freq()\
        {                                           \
            volatile uint16_t r;                                        \
            HardwareTimer16Bit##NUM##Base::freq<static_cast<uint32_t>(ps), ps>(r);\
        }                                           \
                                                    \
        uint16_t cnt() const\
        {\
            unsigned char sreg = SREG;\
            cli();\
            unsigned int i = TCNT##NUM;\
            SREG = sreg;\
            return i;\
        }                                           \
                                                    \
        void release() const\
        {\
            TCNT##NUM = 0;                                        \
        }\
\
        template<CompareMatch mode>\
        void mode##AN() {\
            modeA<mode>();\
        }\
\
        template<CompareMatch mode>\
        void mode##CN() {\
            modeC<mode>();\
        }\
\
        template<CompareMatch mode>\
        void mode##BN() {\
            modeB<mode>();\
        }\
\
        template<bool enable>\
        void interrupt() {\
            interruptOverflow<enable>();\
        }\
\
        bool isInited() const\
        {\
            return inited;\
        }\
        private:\
            bool inited = false;\
    };\
extern HardwareTimerNormal##NUM timerNormal##NUM;\
\
    class HardwareTimerPWM##NUM : HardwareTimer16Bit##NUM##Base {\
    public:\
        void enable();\
\
        template<uint32_t fr>\
        void freq() {\
            HardwareTimer16Bit##NUM##Base::freq<fr, PreScaling::Auto>(ICR##NUM);\
        }\
\
        template<uint32_t millis>\
        void period() {\
            HardwareTimer16Bit##NUM##Base::period<millis, PreScaling::Auto>(ICR##NUM);\
        }\
\
        template<PWMOutMode mode>\
        void mode##BN() {\
            modeB<static_cast<CompareMatch>(mode)>();\
        }\
        /* 0 to 100*/\
        void fillFactor##BN(float factor) {         \
            OCR##NUM##B = static_cast<uint16_t>(ICR##NUM * factor / 100.f);\
        }\
\
        template<PWMOutMode mode>\
        void mode##CN() {\
            modeC<static_cast<CompareMatch>(mode)>();\
        }\
        /* 0 to 100*/\
        void fillFactor##CN(float factor) {\
            OCR##NUM##C = static_cast<uint16_t>(ICR##NUM * factor / 100.f);\
        }\
\
        template<PWMOutMode mode>\
        void mode##AN() {\
            modeA<static_cast<CompareMatch>(mode)>();\
        }\
\
        /* 0 to 100*/\
        void fillFactor##AN(float factor) {\
            OCR##NUM##A = static_cast<uint16_t>(ICR##NUM * factor / 100.f);\
        }\
        bool isInited() const\
        {\
            return inited;\
        }\
        private:\
            bool inited = false;\
    };\
\
    extern HardwareTimerPWM##NUM timerPWM##NUM;\
    class HardwareTimerCounter##NUM : HardwareTimer16Bit##NUM##Base {  \
    public:\
        void enable();\
\
        uint16_t cnt() const\
        {\
            unsigned char sreg = SREG;\
            cli();\
            unsigned int i = TCNT##NUM;\
            SREG = sreg;\
            return i;\
        }\
                                                    \
        template<uint32_t fr, PreScaling ps>\
        void freq##AN##Max() {\
            prescaling = HardwareTimer16Bit##NUM##Base::freq<fr, ps>(OCR##NUM##A);\
        }                                           \
\
        template<uint32_t fr, PreScaling ps>\
        void freq##BN() {\
            prescaling = HardwareTimer16Bit##NUM##Base::freq<fr, ps>(OCR##NUM##B);\
        }                                           \
                                                    \
        template<uint32_t fr, PreScaling ps>\
        void freq##CN() {\
            prescaling = HardwareTimer16Bit##NUM##Base::freq<fr, ps>(OCR##NUM##C);\
        }\
                                                    \
        template<CompareMatch mode>\
        void mode##AN() {\
            modeA<mode>();\
        }\
\
        template<CompareMatch mode>\
        void mode##CN() {\
            modeC<mode>();\
        }\
\
        template<CompareMatch mode>\
        void mode##BN() {\
            modeB<mode>();\
        }\
\
        template<bool enable>\
        void interrupt##AN() {\
            interruptMatchA<enable>();\
        }\
\
        template<bool enable>\
        void interrupt##CN() {\
            interruptMatchC<enable>();\
        }\
\
        template<bool enable>\
        void interrupt##BN() {\
            interruptMatchB<enable>();\
        }\
\
        bool isInited() const\
        {\
            return inited;\
        }\
    private:\
        uint16_t prescaling;\
        bool inited = false;\
    };\
\
    extern HardwareTimerCounter##NUM timerCounter##NUM;\
\
/* нужно создать специализацию для типа Custom для настройки прерываний*/\
    template<typename>\
    struct TimerCounter##NUM##Interrupts {\
        static void match##AN() {}\
\
        static void match##CN() {}\
\
        static void match##BN() {}                  \
\
    }; \
\
    template<typename>\
    struct TimerNormal##NUM##Interrupts {\
        static void overflow() {}\
    };

    MAKE_HARDWARE_TIMER_16_BIT(1, 11, 12, 13)

    MAKE_HARDWARE_TIMER_16_BIT(3, 5, 2, 3)

    MAKE_HARDWARE_TIMER_16_BIT(4, 6, 7, 8)

    MAKE_HARDWARE_TIMER_16_BIT(5, 46, 45, 44)
}
