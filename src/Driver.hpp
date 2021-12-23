#pragma once

#include "MUXController.hpp"
#include "Timer.hpp"
#include "PWMGen.hpp"
#include "PIDRegulator.hpp"

class Driver {
public:
    static void enable(float Kp_, float Ki_, float Kd_);

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
            s2 -= 35;
        }

        if(s1 > 200 && s2 < 200)
        {
            if(cnt >= 0)
            {
                ++cnt;
            } else if(cnt < 0)
            {
                cnt = 0;
            }
        }

        if(s2 > 200 && s1 < 200)
        {
            if(cnt <= 0)
            {
                --cnt;
            } else if(cnt > 0)
            {
                cnt = 0;
            }
        }

        App::println("Cnt: ", cnt);

//        if(cnt > 27 || cnt < -27)
//        {
//            Timer timer;
//            timer.start(250, 350);
//
//            while (!timer.event())
//            {
//                if(cnt > 0)
//                {
//                    speedL(-2000);
//                    speedR(3000);
//                } else
//                {
//                    speedL(3000);
//                    speedR(-2000);
//                }
//            }
//            cnt = 0;
//        }

        int16_t const dif = s2 - s1;
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
            PWMGen<PWMTimer::T3P235>::fillFactor<engine>(PWMRange + speed);
            PinMap::write<PinHigh, control>();
        }
    }

    static void speedL(int16_t v)
    {
        return engineSpeed<Pin::D3, Pin::D51>(v * 1.4f);
    }

    static void speedR(int16_t v)
    {
        return engineSpeed<Pin::D2, Pin::D49>(v);
    }

    static int16_t rs;
    static int16_t cnt;
    static PIDRegulator reg;
};
