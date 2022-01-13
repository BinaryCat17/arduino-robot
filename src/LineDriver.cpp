#include "LineDriver.hpp"

#include "AVRLib/ADCController.hpp"
#include "AVRLib/SerialMonitor.hpp"
#include "Driver.hpp"

using namespace AvrLib;

void LineDriver::enable(float Kp_, float Ki_, float Kd_) {
    enableOnce([=]{
        reg.start(Kp_, Ki_, Kd_, 0, -100, 100);

        driver.enable();

        adc.freq<ADCFreq::F125000>();
        adc.enable();

        monitor.println("Line driver enabled");
    });
}

void LineDriver::speed(int16_t speed) {
    monitor.println("Robot speed: ", speed);
    rs = speed;
}

void LineDriver::drive() {
    auto const s1 = (int16_t) adc.read<APin::A0>();
    auto const s2 = (int16_t) adc.read<APin::A1>();

    int16_t const dif = s2 - s1;
    int16_t const out = reg.calculate(dif, 1);

    monitor.println("S ", s1, " ", s2, " F ", rs - out, " ", rs + out);
    driver.speedL(rs - out);
    driver.speedR(rs + out);
}

LineDriver lineRobot = {};
