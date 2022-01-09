#include "App.hpp"
#include "util/delay.h"

const DPin trig = DPin::D12;
const DPin echo = DPin::D11;

void Main::setup() {
    location.enable();

    pinDirection<PinOut, trig>();
    pinDirection<PinIn, echo>();

    deltaTimer.enable();
}

void Main::loop() {
    Position pos = location.pos();
    monitor.print("L ", pos.x);
    monitor.println(" R ", pos.y, " mm");

    pinWrite<PinHigh, trig>();
    _delay_us(10);
    pinWrite<PinLow, trig>();

    deltaTimer.pass();
    while(!pinRead<echo>()) {}
    uint16_t const time = deltaTimer.currentMicros();
    uint16_t const distanceMm = time / 5.8f;
    monitor.println("Distance ", distanceMm, " Time ", time);
}

