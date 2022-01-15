#include "DistanceSensor.hpp"
#include "DeltaTimer.hpp"
#include "util/delay.h"

using namespace AvrLib;


const DPin trig = DPin::D12;
const DPin echo = DPin::D11;

void DistanceSensor::enable() {
    // запускаем таймер с точностью до микросекунды
    deltaTimer4.enable(Precision::Micros);
    // задаём направления пинов
    pinDirection<PinOut, trig>();
    pinDirection<PinIn, echo>();
}

uint16_t DistanceSensor::distanceMm() {
    // убедимся, что на пине 0
    pinWrite<PinLow, trig>();
    _delay_us(2);
    // 10 микросекунд подаём high, чтобы подготовиться к сканированию
    pinWrite<PinHigh, trig>();
    _delay_us(10);
    // отправляем ультразвуковой сигнал
    pinWrite<PinLow, trig>();

    // несколько микросекунд датчик думает, прежде чем установить эхо в 0
    while(!pinRead<echo>()) {}
    // начинаем замер времени
    deltaTimer4.pass();
    // пока не дождёмся возвращения сигнала
    while(pinRead<echo>()) {}
    // запомнили время
    uint16_t const time = deltaTimer4.current();
    // переводим в миллиметры при скорости звука в 340 м/с, расстояние удваиваем
    // пол сантиметра добавляем, чтобы считать от начала решетки
    return time / 5.8f + 5;
}

DistanceSensor distanceSensor = {};
