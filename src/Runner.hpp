#pragma once

#include "PositionTrack.hpp"
#include "DistanceSensor.hpp"
#include "Driver.hpp"

class Runner {
public:
    void enable();

    void scanObstacle(float rotation, int& side, float& distance);

    void run();

private:
    bool finish = false;
    float const sensorLen = 400;
    float const checkAngle = 25.f; // угол сканирования препятствий
    float const fieldStartY = 250.f; // координата начала
    float const fieldRunLen = 3000.f; // расстояние поля с препятствиями
    float const fieldEndY = fieldStartY + fieldRunLen + 250.f + 20.f; // Координата конца
    uint8_t const checkCount = 3; // количество проверкой при сканировании препятствий
};

extern Runner runner;