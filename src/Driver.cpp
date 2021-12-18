#include "Driver.hpp"

#include "MUXController.hpp"
#include "PWMGen.hpp"
#include "App.hpp"
#include "math.h"

float robotKp = 0.002;
uint16_t divTime = 30;

void enableDriver() {
    App::println("Driver enabled");

    MUXController::enable();
    PWMGen::enable(PWMTimer::T3P235, 5000);

    PWMGen::fillFactor(Pin::D2, 0.3);
    PWMGen::fillFactor(Pin::D3, 0.8);
}

void Driver::speed(float speed) {
    instance().m_robotSpeed = speed;
    if (!instance().inited) {
        enableDriver();
        instance().inited = true;
    }
}

void Driver::drive() {
    auto s1 = (int16_t) MUXController::read(Pin::A0);
    App::print("ADC0 : ");
    App::println(s1);

    auto s2 = (int16_t) MUXController::read(Pin::A1);
    App::print("ADC1 : ");
    App::println(s2);

    int16_t cur = s1 - s2;

    App::print("Err : ");
    App::println(cur);

    float out = robotKp * (-cur);

    App::print("out : ");
    App::println(out);

    PWMGen::fillFactor(Pin::D2, instance().m_robotSpeed + out);
    PWMGen::fillFactor(Pin::D3, instance().m_robotSpeed - out);
}

Driver &Driver::instance() {
    static Driver driver;
    return driver;
}

void Driver::calibrate() {
    Timer timer;

    // калибруем белый цвет
    timer.start(2000);
    while (!timer.event()) {
        instance().white.read();
    }

    // едем до черного цвета
    instance().white.read();
    PWMGen::fillFactor(Pin::D2, 0.2);
    PWMGen::fillFactor(Pin::D3, 0.2);
    while (!(instance().white.diverge(0) > 0 && instance().white.diverge(1) > 0)) {}
    PWMGen::fillFactor(Pin::D3, 0);
    PWMGen::fillFactor(Pin::D3, 0);

    // калибруем черный цвет
    timer.start(2000);
    while (!timer.event()) {
        instance().black.read();
    }

    PWMGen::fillFactor(Pin::D2, 0.2);
    PWMGen::fillFactor(Pin::D3, -0.2);
    while (!(instance().white.diverge(0) > 0 && instance().white.diverge(1) > 0) &&
            !(instance().black.diverge(0) < 0 && instance().white.diverge(1) < 0)) {}
}

void Driver::correct() {

}

void SensorColor::read() {
    auto val1 = (int16_t) MUXController::read(Pin::A0);
    auto val2 = (int16_t) MUXController::read(Pin::A1);

    uint32_t dif1 = fabs((int32_t) data[0].avgSum - (int32_t) val1);
    uint32_t dif2 = fabs((int32_t) data[1].avgSum - (int32_t) val2);
    data[0].avgSum += val1;
    data[1].avgSum += val2;
    data[0].avgDifSum += dif1;
    data[1].avgDifSum += dif2;
    ++cnt;
}

int SensorColor::diverge(uint8_t sensor) {
    uint32_t val;
    if (sensor == 0) {
        val = (int16_t) MUXController::read(Pin::A0);
    } else {
        val = (int16_t) MUXController::read(Pin::A1);
    }

    if (val > avg(sensor) + avgDif(sensor)) {
        data[sensor].divCnt += 2;
    } else if (val < avg(sensor) - avgDif(sensor)) {
        data[sensor].divCnt -= 2;
    } else {
        if (data[sensor].divCnt > 0) {
            data[sensor].divCnt -= 1;
        } else if (data[sensor].divCnt < 0) {
            data[sensor].divCnt += 1;
        }
    }

    if (data[sensor].divCnt <= 1 && data[sensor].divCnt >= -1) {
        data[sensor].div = false;
    } else {
        if (!data[sensor].div) {
            data[sensor].divTimer.start(divTime);
            data[sensor].div = true;
        } else if (data[sensor].divTimer.event()) {
            return data[sensor].divCnt;
        }
    }

    return 0;
}
