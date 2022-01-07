#include "LineDriver.hpp"

#include "AVRLib/ADCController.hpp"
#include "AVRLib/HardwareTimer.hpp"
#include "AVRLib/SerialMonitor.hpp"

void LineDriver::enable(float Kp_, float Ki_, float Kd_) {
    static bool enabled = false;
    if (!enabled) {
        enabled = true;
    } else {
        return;
    }

    reg.start(Kp_, Ki_, Kd_, 0, -100.f, 100.f);

    adc.freq<ADCFreq::F125000>();
    adc.enable();

    pinDirection<PinOut, DPin::D49>();
    pinDirection<PinOut, DPin::D51>();
    pinDirection<PinOut, DPin::D2>();
    pinDirection<PinOut, DPin::D3>();

    timerPWM3.freq<5000>();
    timerPWM3.enable();

    monitor.println("Driver enabled");
}

LineDriver lineRobot = {};
