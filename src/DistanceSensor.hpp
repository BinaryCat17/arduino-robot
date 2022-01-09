#pragma once
#include "stdint.h"

class DistanceSensor
{
public:
    void enable();

    uint16_t distanceMm();
};

extern DistanceSensor distanceSensor;