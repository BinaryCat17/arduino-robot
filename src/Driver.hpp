#pragma once

#include "stdint.h"
#include "math.h"
#include "Timer.hpp"

class SensorColor {
public:
    void read();

    int diverge(uint8_t sensor);

    uint32_t avg(uint8_t sensor) const {
        return data[sensor].avgSum / cnt;
    }

    uint32_t avgDif(uint8_t sensor) const {
        return data[sensor].avgDifSum / cnt;
    }

public:
    uint32_t cnt = 0;
    struct SensorData {
        uint32_t avgSum = 0;
        uint32_t avgDifSum = 0;
        int16_t divCnt = 0;
        Timer divTimer;
        bool div = false;
        bool res = false;
    } data[2] = {};
};

class Driver {
public:
    static void speed(float speed);

    static void calibrate();

    static void correct();

    static void drive();

private:
    static void enable();

    static void speed1(float v);

    static void speed2(float v);

    static Driver &instance();

    bool inited = false;

    SensorColor white;
    SensorColor gray;
    SensorColor black;
    int32_t cnt = 0;

    float m_robotSpeed = 0.3;
};
