#pragma once

#include "AVRLib/PinInputHandler.hpp"
#include "math.h"
#include "DeltaTimer.hpp"

using namespace AvrLib;

class Main;

extern volatile uint8_t implEncMask1;
extern volatile uint8_t implEncMask2;
extern volatile int32_t implEncCntL;
extern volatile int32_t implEncCntR;

// в миллиметрах
struct Position {
    int32_t x;
    int32_t y;
};

const int32_t wheelLen = 213;
const double wheelbase = 200;
const double wheelRadius = wheelbase / 2;
const int32_t avgEnc = 2437;

class PositionTrack {
public:
    void enable();

    void track() {
        int32_t encCntL, encCntR;

        ATOMIC_BLOCK(ATOMIC_FORCEON) {
            encCntL = implEncCntL;
            encCntR = implEncCntR;
        }

        // для правого колеса
        int32_t const moveR = (encCntR - prevEncCntR) * wheelLen / avgEnc;
        double const rotationR = (double) moveR / wheelbase;
        posX -= wheelRadius * sin(rotationR);
        double const cosR = cos(rotationR);
        posY += wheelRadius * (1 / cosR - 1) * cosR;
        mRotationRadians += rotationR * 2;

        // для левого колеса
        int32_t const moveL = (encCntL - prevEncCntL) * wheelLen / avgEnc;
        double const rotationL = (double) moveL / wheelbase;
        posX += wheelRadius * sin(rotationL);
        double const cosL = cos(rotationL);
        posY += wheelRadius * (1 / cosL - 1) * cosL;
        mRotationRadians -= rotationL * 2;

        prevEncCntL = encCntL;
        prevEncCntR = encCntR;
    }

    float rotationRadians() const {
        return (float) mRotationRadians;
    }

    Position pos() const {
        return Position{
                (int32_t) posX,
                (int32_t) posY,
        };
    }

private:
    double mRotationRadians = 0;
    double posX = 0;
    double posY = 0;
    int32_t prevEncCntL = 0;
    int32_t prevEncCntR = 0;
};

extern PositionTrack location;

template<DPin pin1, DPin pin2>
void encoderChange(volatile int32_t &cnt, volatile uint8_t &mask) {
    // первые два бита - значения a и b текущие показания энкодера соответственно
    // последние два бита - предыдущие значения a и b соответственно

    // 6.79 см, 21,3 см, 2437 делений
    // 0100 0010 1011 1101 по часовой стрелке
    // 0111 1110 1000 0001 против часовой стрелки

    auto const a = pinRead<pin1>();
    auto const b = pinRead<pin2>();

    mask <<= 2;
    setBits<0>(mask, b);
    setBits<1>(mask, a);
    mask &= 0b00001111;

    if (mask == 0b0100 || mask == 0b0010 || mask == 0b1011 || mask == 0b1101) {
        ++cnt;
    } else if (mask == 0b0111 || mask == 0b1110 || mask == 0b1000 || mask == 0b0001) {
        --cnt;
    }
}

namespace AvrLib {
    template<>
    struct PinHandlerInterrupts<Main> {
        static void handle18() {
            encoderChange<DPin::D18, DPin::D19>(implEncCntR, implEncMask1);
        }

        static void handle19() {
            encoderChange<DPin::D18, DPin::D19>(implEncCntR, implEncMask1);
        }

        static void handle20() {
            encoderChange<DPin::D20, DPin::D21>(implEncCntL, implEncMask2);
        }

        static void handle21() {
            encoderChange<DPin::D20, DPin::D21>(implEncCntL, implEncMask2);
        }
    };
}
