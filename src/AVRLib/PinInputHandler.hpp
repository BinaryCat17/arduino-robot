#pragma once

#include "DPinMap.hpp"
#include "SerialMonitor.hpp"

namespace AvrLib {
    enum class PinHandleMode {
        Low,
        Change,
        Falling,
        Rising,
    };

    enum class ISRPin {
        D18 = static_cast<int>(DPin::D18),
        D19 = static_cast<int>(DPin::D19),
        D20 = static_cast<int>(DPin::D20),
        D21 = static_cast<int>(DPin::D21),
    };

    template<typename>
    struct PinHandlerInterrupts {
        static void handle18() {}

        static void handle19() {}

        static void handle20() {}

        static void handle21() {}
    };

    class PinInputHandler {
    public:
        template<ISRPin ipin, PinHandleMode mode>
        void enable() {
            cli();
            constexpr DPin pin = static_cast<DPin>(ipin);
            pinDirection<PinIn, pin>();
            // подтяжка к питанию
            pinWrite<true, pin>();

            auto constexpr inter = interNum<pin>();
            monitor.println("Enable inter ", inter);
            if constexpr(inter < 4) {
                setMode<mode, ISC00 + interNum<pin>() * 2>(EICRA);
            } else if (inter < 8) {
                setMode<mode, ISC40 + interNum<pin>() * 2 - 4>(EICRB);
            }

            setBits<true, INT0 + inter>(EIMSK);
            sei();
        }

        template<ISRPin ipin>
        void disable() {
            constexpr DPin pin = static_cast<DPin>(ipin);
            setBits<false, INT0 + interNum<pin>()>(EIMSK);
        }

    private:
        template<DPin pin>
        static constexpr uint8_t interNum() {
            if constexpr(pin == DPin::D18) {
                return 3;
            } else if constexpr(pin == DPin::D19) {
                return 2;
            } else if constexpr(pin == DPin::D20) {
                return 1;
            } else if constexpr(pin == DPin::D21) {
                return 0;
            }
        }

        template<PinHandleMode mode, uint8_t first>
        void setMode(volatile uint8_t &reg) {
            if constexpr(mode == PinHandleMode::Low) {
                BitVal(reg).off<first, first + 1>();
            } else if constexpr(mode == PinHandleMode::Change) {
                BitVal(reg).off<first + 1>().template on<first>();
            } else if constexpr(mode == PinHandleMode::Falling) {
                BitVal(reg).off<first>().template on<first + 1>();
            } else if constexpr(mode == PinHandleMode::Rising) {
                BitVal(reg).on<first, first + 1>();
            }
        }
    };

    extern PinInputHandler pinInputHandler;

}
