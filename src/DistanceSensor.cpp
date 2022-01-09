#include "DistanceSensor.hpp"
#include "DeltaTimer.hpp"
#include "util/delay.h"

using namespace AvrLib;

const DPin trig = DPin::D12;
const DPin echo = DPin::D11;

void DistanceSensor::enable() {
    deltaTimer.enable();
    pinDirection<PinOut, trig>();
    pinDirection<PinIn, echo>();
}

uint16_t DistanceSensor::distanceMm() {
    pinWrite<PinLow, trig>();
    _delay_us(2);
    pinWrite<PinHigh, trig>();
    _delay_us(10);
    pinWrite<PinLow, trig>();

    // начинаем отсчет времени
    deltaTimer.pass();
    // зачем мучаться и пытаться ждать на прерывании изменения состояния ножки, когда при
    // расстоянии в 30см придётся ждать 1740 микросекунд, что не существенно, если робот обычно
    // выполняет не более 30 проверок в секунду
    while(pinRead<echo>()) {}
    uint16_t const time = deltaTimer.currentMicros();
    // переводим в миллиметры при скорости звука в 340 м/с, расстояние удваиваем
    return time / 5.8f;
}

DistanceSensor distanceSensor = {};
