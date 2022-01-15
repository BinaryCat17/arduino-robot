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

const double wheelLen = 210.04;
const double wheelbase = 236;
const double baseRadius = wheelbase / 2;

//const float avgEncL = 2437;
//const float avgEncR = 2437;
const float avgEncL = 1500;// Робот 1
const float avgEncR = 1500; // Робот 1
//const float avgEncL = 1490; // Робот 3
//const float avgEncR = 1510; // Робот 3

class PositionTrack {
public:
    void enable();

    void track();

    float leftMovement() const
    {
        return mLeftMovement;
    }

    float rightMovement() const
    {
        return mRightMovement;
    }

    float rotationRadians() const {
        return (float) mRotationRadians;
    }

    float rotationDegrees() const
    {
        return rotationRadians() / 0.0174533f;
    }

    Position pos() const {
        return Position{
                (int32_t) mPosX,
                (int32_t) mPosY,
        };
    }

private:
    double mRotationRadians = 0;
    double mPosX = 0;
    double mPosY = 0;
    double mLeftMovement = 0;
    double mRightMovement = 0;
    int32_t mPrevEncCntL = 0;
    int32_t mPrevEncCntR = 0;
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
            encoderChange<DPin::D21, DPin::D20>(implEncCntL, implEncMask2);
        }

        static void handle21() {
            encoderChange<DPin::D21, DPin::D20>(implEncCntL, implEncMask2);
        }
    };
}
