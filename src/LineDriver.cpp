#include "LineDriver.hpp"

#include "AVRLib/ADCController.hpp"
#include "AVRLib/HardwareTimer.hpp"
#include "AVRLib/SerialMonitor.hpp"

void LineDriver::enable(float Kp_, float Ki_, float Kd_) {
    enableOnce([=]{
        reg.start(Kp_, Ki_, Kd_, 0, -100, 100);

        adc.freq<ADCFreq::F125000>();
        adc.enable();

        pinDirection<PinOut, DPin::D49>();
        pinDirection<PinOut, DPin::D51>();
        pinDirection<PinOut, DPin::D2>();
        pinDirection<PinOut, DPin::D3>();

        timerPWM3.freq<5000>();
        timerPWM3.mode2<PWMOutMode::NonInverting>();
        timerPWM3.mode3<PWMOutMode::NonInverting>();
        timerPWM3.enable();

        monitor.println("Driver enabled");
    });
}

LineDriver lineRobot = {};
