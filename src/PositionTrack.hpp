#pragma once

#include "AVRLib/PinInputHandler.hpp"

using namespace AvrLib;

class Main;

extern volatile uint8_t encMask1;
extern volatile uint8_t encMask2;
extern volatile int32_t encCntL;
extern volatile int32_t encCntR;


// в миллиметрах
struct Position {
    int32_t x;
    int32_t y;
};


const int32_t wheelLen = 213;
const int32_t avgEnc = 2437;

class PositionTrack {
public:
    void enable();

    Position pos() {
        return Position{
                encCntL * wheelLen / avgEnc,
                encCntR * wheelLen / avgEnc
        };
    }
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
            encoderChange<DPin::D18, DPin::D19>(encCntR, encMask1);
        }

        static void handle19() {
            encoderChange<DPin::D18, DPin::D19>(encCntR, encMask1);
        }

        static void handle20() {
            encoderChange<DPin::D20, DPin::D21>(encCntL, encMask2);
        }

        static void handle21() {
            encoderChange<DPin::D20, DPin::D21>(encCntL, encMask2);
        }

    };
}
