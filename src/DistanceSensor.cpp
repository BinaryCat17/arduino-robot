#include "DistanceSensor.hpp"
#include "DeltaTimer.hpp"
#include "util/delay.h"

using namespace AvrLib;

const DPin trig = DPin::D12;
const DPin echo = DPin::D11;

void DistanceSensor::enable() {
    deltaTimer4.enable(Precision::Micros);
    pinDirection<PinOut, trig>();
    pinDirection<PinIn, echo>();
}

uint16_t DistanceSensor::distanceMm() {
    pinWrite<PinLow, trig>();
    _delay_us(2);
    pinWrite<PinHigh, trig>();
    _delay_us(10);

    pinWrite<PinLow, trig>();

    while(!pinRead<echo>()) {}
    deltaTimer4.pass();
    while(pinRead<echo>()) {}
    uint16_t const time = deltaTimer4.current();
    // переводим в миллиметры при скорости звука в 340 м/с, расстояние удваиваем
    // пол сантиметра добавляем, чтобы считать от начала решетки
    return time / 5.8f + 5;
}

DistanceSensor distanceSensor = {};
