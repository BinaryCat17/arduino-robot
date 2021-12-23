#include "Driver.hpp"

#include "MUXController.hpp"
#include "PWMGen.hpp"
#include "App.hpp"

void Driver::enable(float Kp_, float Ki_, float Kd_) {
    static bool enabled = false;
    if(!enabled)
    {
        enabled = true;
    } else
    {
        return;
    }

    reg.start(Kp_, Ki_, Kd_, 0, rs - PWMRange, PWMRange - rs);

    MUXController::freq(MUXFreq::F125000);
    MUXController::enable();
    PWMGen<PWMTimer::T3P235>::enable(5000);

    PinMap::direction<PinOut, Pin::D49>();
    PinMap::direction<PinOut, Pin::D51>();
    PinMap::direction<PinOut, Pin::D2>();
    PinMap::direction<PinOut, Pin::D3>();

    App::println("Driver enabled");
}

int16_t Driver::rs = 300;
int16_t Driver::cnt = 0;
PIDRegulator Driver::reg = {};
