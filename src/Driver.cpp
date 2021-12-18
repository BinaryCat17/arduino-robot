#include "Driver.hpp"

#include "MUXController.hpp"
#include "PWMGen.hpp"
#include "App.hpp"
#include "math.h"

float robotKp = 0.0005;
uint16_t divTime = 30;

void Driver::enable() {

    MUXController::enable();
    PWMGen::enable(PWMTimer::T3P235, 5000);

    App::println("Driver enabled");
    PinMap::direction(Pin::D49, PinDir::Out);
    PinMap::direction(Pin::D51, PinDir::Out);

    PinMap::write(Pin::D49, PinValue::High);
    PinMap::write(Pin::D51, PinValue::High);
}

void Driver::speed(float speed) {
    instance().m_robotSpeed = speed;
    if (!instance().inited) {
        enable();
        instance().inited = true;
    }
}

void Driver::drive() {
    auto div1 = instance().gray.diverge(0);
    auto div2 = instance().gray.diverge(1);

    auto norm1 = div1 / (div1 + instance().gray.avgDif(0));
    auto norm2 = div2 / (div2 + instance().gray.avgDif(1));

    speed1(div1 * robotKp);
    speed2(div2 * robotKp);

    speed1(instance().m_robotSpeed + norm1);
    speed2(instance().m_robotSpeed + norm2);
}

Driver &Driver::instance() {
    static Driver driver;
    return driver;
}

void Driver::calibrate() {
    Timer timer;

    // калибруем белый цвет
    timer.start(2000, 2000);
    speed1(0.3f);
    speed2(0.3f);
    while (!timer.event()) {
        instance().white.read();
    }
    App::print("Avg white: ");
    App::print(instance().white.avg(0));
    App::print(" ");
    App::println(instance().white.avg(1));

    // едем до черного цвета
    while (!(instance().white.diverge(0) > 0 && instance().white.diverge(1) > 0)) {}
    App::println("stop");

    // калибруем черный цвет
    speed1(0);
    speed2(0);

    timer.start(2000, 2000);
    while (!timer.event()) {
        instance().black.read();
    }

    App::print("Avg black: ");
    App::print(instance().black.avg(0));
    App::print(" ");
    App::println(instance().black.avg(1));

    speed1(instance().m_robotSpeed);
    speed2(-instance().m_robotSpeed);

    int32_t maxErr = 4;
    int32_t prevErr = 0;

    timer.start(10000, 10000);
    while (!timer.event()) {
        int32_t err = instance().white.diverge(0) - instance().black.diverge(0);
        if (err > maxErr) {
            robotKp += 0.001f * (float) (prevErr - err);
            prevErr = err;
            maxErr = err;
            float cur = (float) (maxErr - err) * robotKp;
            speed1(cur);
            speed1(-cur);
        }
    }

    timer.start(2000, 2000);
    while (!timer.event()) {
        instance().gray.read();
    }
}

int32_t sum = 0;

void Driver::correct() {
    uint16_t s1 = MUXController::read(Pin::A0);
    //App::print("ADC0 : ");
    //App::println(s1);
    uint16_t s2 = MUXController::read(Pin::A1);
    //App::print("ADC1 : ");
    //App::println(s2);
    uint16_t cur = s1 - s2;


    int16_t err = 0 - cur;
    //App::print("Err : ");
    //App::println(err);
    float out = robotKp * err;

    //App::print("Out : ");
    //App::println(out);



    if (s1 < 250 && s2 < 250) {
        ++instance().cnt;
        if (sum > 0) {
            speed1((-instance().m_robotSpeed - out * (float) instance().cnt / 3));
            speed2((instance().m_robotSpeed + out * (float) instance().cnt / 3));
        } else {
            speed1((instance().m_robotSpeed + out * (float) instance().cnt/ 3));
            speed2((-instance().m_robotSpeed - out * (float) instance().cnt / 3));
        }

    } else {
        sum += err;
        instance().cnt = 0;
        speed1((instance().m_robotSpeed - out));
        speed2((instance().m_robotSpeed + out));
    }

}

void Driver::speed2(float v) {
    if (v > 1.0) {
        v = 1.0;
    } else if (v < -1.0) {
        v = -1.0;
    }

    if (v > 0.0) {
        PWMGen::fillFactor(Pin::D2, v);
    } else {
        PWMGen::fillFactor(Pin::D2, 1.0f + v);
    }

    if (v > 0) {
        PinMap::write(Pin::D49, PinValue::Low);
    } else {
        PinMap::write(Pin::D49, PinValue::High);
    }
}

void Driver::speed1(float v) {
    if (v > 1.0) {
        v = 1.0;
    } else if (v < -1.0) {
        v = -1.0;
    }

    if (v > 0.0) {
        PWMGen::fillFactor(Pin::D3, v);
    } else {
        PWMGen::fillFactor(Pin::D3, 1.0f + v);
    }

    if (v > 0) {
        PinMap::write(Pin::D51, PinValue::Low);
        App::println("Low");
    } else {
        PinMap::write(Pin::D51, PinValue::High);
        App::println("High");
    }
}

void SensorColor::read() {
    auto val1 = (int16_t) MUXController::read(Pin::A0);
    auto val2 = (int16_t) MUXController::read(Pin::A1);

    //App::println("Dif");
    uint32_t dif1 = fabs((int32_t) avg(0) - (int32_t) val1);
    //App::println(avg(0));
    //App::println(val1);
    uint32_t dif2 = fabs((int32_t) avg(1) - (int32_t) val2);
    //App::println(avg(1));
    //App::println(val2);
    data[0].avgSum += val1;
    data[1].avgSum += val2;
    data[0].avgDifSum += dif1;
    data[1].avgDifSum += dif2;
    ++cnt;
}

int SensorColor::diverge(uint8_t sensor) {
    uint32_t val = 0;
    if (sensor == 0) {
        val = (int16_t) MUXController::read(Pin::A0);
    } else {
        val = (int16_t) MUXController::read(Pin::A1);
    }

    auto avgv = avg(sensor);
    auto avgd = avgDif(sensor);

    if (val > avgv + avgd) {
        data[sensor].divCnt += 2;
    } else if (val < avgv - avgd) {
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
        data[sensor].res = false;
    } else {
        if (!data[sensor].div) {
            data[sensor].divTimer.start(divTime);
            data[sensor].div = true;
        } else if (data[sensor].divTimer.event()) {
            data[sensor].res = true;
        }
    }

    App::print("Div: ");
    if (data[sensor].res) {
        App::println(data[sensor].divCnt);
        return data[sensor].divCnt;
    } else {
        App::println(0);
        return 0;
    }
}
