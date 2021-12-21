#pragma once

#include "MUXController.hpp"
#include "Timer.hpp"
#include "PWMGen.hpp"
#include "PIDRegulator.hpp"

class Driver {
public:
    static void enable(int16_t Kp_, int16_t Ki_, int16_t Kd_);

    static void speed(int32_t speed)
    {
        rs = speed;
    }

    inline static void drive()
    {
        auto const s1 = (int16_t) MUXController::read<Pin::A0>();
        auto s2 = (int16_t) MUXController::read<Pin::A1>();
        if(s2 < 150)
        {
            s2 += 24;
        }

        int16_t const dif = s1 - s2;
        int16_t const out = reg.calculate(dif);

        App::println("S ", s1, " ", s2, " F ", rs - out, " ", rs + out);
        speedL((rs - out));
        speedR((rs + out));
    }

private:
    template<Pin engine, Pin control>
    static void engineSpeed(int16_t speed) {
        if (speed > 0) {
            // крутим вперёд
            PWMGen<PWMTimer::T3P235>::fillFactor<engine>(speed);
            PinMap::write<PinLow, control>();
        } else {
            // нужно инвертировать значение, если крутим колесом назад
            PWMGen<PWMTimer::T3P235>::fillFactor<engine>(10000 + speed);
            PinMap::write<PinHigh, control>();
        }
    }

    static void speedL(int16_t v)
    {
        return engineSpeed<Pin::D3, Pin::D51>(v);
    }

    static void speedR(int16_t v)
    {
        return engineSpeed<Pin::D2, Pin::D49>(v);
    }

    static int16_t rs;
    static PIDRegulator reg;
};
