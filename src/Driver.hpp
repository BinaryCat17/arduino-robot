#pragma once

#include "MUXController.hpp"
#include "Timer.hpp"
#include "PWMGen.hpp"

// const float robotKp = 0.5;

class Driver {
public:
    static void enable();

    static void speed(int32_t speed)
    {
        rs = speed;
    }

    inline static void drive()
    {
        auto s1 = (int16_t) MUXController::read<Pin::A0>();
        auto s2 = (int16_t) MUXController::read<Pin::A1>();

        int16_t cur = s1 - s2;

        int16_t err = 0 - cur;
        // int16_t out = robotKp * err;
        int16_t out = err / 2;

        App::println("S ", s1, " ", s2, " F ", rs - out, " ", rs + out);
        speedL((rs - out));
        speedR((rs + out));
    }

private:
    template<Pin engine, Pin control>
    static void engineSpeed(int16_t speed) {
        // проверяем на выход из допустимого диапазона
        if (speed > 1000) {
            speed = 1000;
        } else if (speed < -1000) {
            speed = -1000;
        }

        if (speed > 0) {
            // крутим вперёд
            PWMGen<PWMTimer::T3P235>::fillFactor<engine>(speed);
            PinMap::write<PinLow, control>();
        } else {
            // нужно инвертировать значение, если крутим колесом назад
            PWMGen<PWMTimer::T3P235>::fillFactor<engine>(1000 + speed);
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
};
