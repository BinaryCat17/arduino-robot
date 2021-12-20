#include "Driver.hpp"

#include "MUXController.hpp"
#include "PWMGen.hpp"
#include "App.hpp"


void Driver::enable() {
    static bool enabled = false;
    if(!enabled)
    {
        enabled = true;
    } else
    {
        return;
    }

    //MUXController::freq(MUXFreq::F125000);
    MUXController::enable();
    PWMGen<PWMTimer::T3P235>::enable(5000);

    PinMap::direction<PinOut, Pin::D49>();
    PinMap::direction<PinOut, Pin::D51>();
    PinMap::direction<PinOut, Pin::D2>();
    PinMap::direction<PinOut, Pin::D3>();

    App::println("Driver enabled");
}

int16_t Driver::rs = 300;