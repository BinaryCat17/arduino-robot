#include "Driver.hpp"

#include "MUXController.hpp"
#include "PWMGen.hpp"
#include "App.hpp"
#include "Timer.hpp"
#include "math.h"

const float robotKp = 0.002;

void Driver::enable() {
    App::println("Driver enabled");

    MUXController::enable();
    PWMGen::enable(PWMTimer::T3P235, 5000);

    PWMGen::fillFactor(Pin::D2, 0.3);
    PWMGen::fillFactor(Pin::D3, 0.8);
}

void Driver::correct() {


    auto s1 = (int16_t) MUXController::read(Pin::A0);
    App::print("ADC0 : ");
    App::println(s1);

    auto s2 = (int16_t) MUXController::read(Pin::A1);
    App::print("ADC1 : ");
    App::println(s2);

    int16_t cur = s1 - s2;

    instance().m_cnt += 1;
    instance().m_errSum += fabs(cur);

    if (fabs(cur) <= instance().m_errSum / instance().m_cnt) {
      //  cur = 0;
    }
    App::print("Err : ");
    App::println(cur);

    if (cur > 0) {
       // cur -= instance().m_errSum / instance().m_cnt;
    } else if (cur < 0) {
        //cur += instance().m_errSum / instance().m_cnt;
    }

    float out = robotKp * (-cur);

    App::print("out : ");
    App::println(out);

    auto n1 = instance().m_robotSpeed + out;
    if(n1 > 1.0f)
    {
        n1 = 1.0f;
    }
    if(n1 < 0.0f)
    {
        n1 = 0.0f;
    }
    auto n2 = instance().m_robotSpeed - out;
    if(n2 > 1.0f)
    {
        n2 = 1.0f;
    }
    if(n2 < 0.0f)
    {
        n2 = 0.0f;
    }
    PWMGen::fillFactor(Pin::D2, n1);
    PWMGen::fillFactor(Pin::D3, n2);
}

Driver &Driver::instance() {
    static Driver driver;
    return driver;
}

void Driver::speed(float speed) {
    instance().m_robotSpeed = speed;
}

void Driver::calibrate() {
    Timer timer;
    timer.interval(3000);
    timer.start();

    while (!timer.event()) {
        ++instance().m_cnt;

        auto s1 = (int16_t) MUXController::read(Pin::A0);
        auto s2 = (int16_t) MUXController::read(Pin::A1);
        auto dif = fabs(s1 - s2);
        instance().m_errSum += dif;

    }

    App::print("AVG Dif:");
    App::println(instance().m_errSum / instance().m_cnt);
}
