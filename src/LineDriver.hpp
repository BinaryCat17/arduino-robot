#pragma once

#include "AVRLib/ADCController.hpp"
#include "AVRLib/HardwareTimer.hpp"
#include "Util/PIDRegulator.hpp"
#include "DeltaTimer.hpp"

using namespace AvrLib;

class LineDriver {
public:
    void enable(float Kp_, float Ki_, float Kd_);

    void speed(int16_t speed) {
        rs = speed;
    }

    void drive() {
        auto const s1 = (int16_t) adc.read<APin::A0>();
        auto s2 = (int16_t) adc.read<APin::A1>();

        // балансируем датчики
        if (s2 < 150) {
            s2 -= 35;
        }

        int16_t const dif = s2 - s1;
        int16_t const out = reg.calculate(dif, deltaTimer.deltaMillis());

        monitor.println("S ", s1, " ", s2, " F ", rs - out, " ", rs + out);
        speedL(rs - out);
        speedR(rs + out);
    }

private:
    template<DPin control, typename FF>
    void engineSpeed(float speed, FF fillFactor) {
        if (speed > 0) {
            // крутим вперёд
            fillFactor(speed);
            pinWrite<PinLow, control>();
        } else {
            // нужно инвертировать значение, если крутим колесом назад
            fillFactor(100.0f + speed);
            pinWrite<PinHigh, control>();
        }
    }

    void speedL(int16_t v) {
        // балансируем двигатели на третьем роботе
        return engineSpeed<DPin::D51>(v * 1.4f, [](auto v) { timerPWM3.fillFactor3(v); });
    }

    void speedR(int16_t v) {
        return engineSpeed<DPin::D49>(v, [](auto v) { timerPWM3.fillFactor2(v); });
    }

    int16_t rs = 300;
    PIDRegulator reg = {};
};

extern LineDriver lineRobot;
