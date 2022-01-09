#include "PositionTrack.hpp"

volatile int32_t encCntL = 0;
volatile int32_t encCntR = 0;

volatile uint8_t encMask1 = 0;
volatile uint8_t encMask2 = 0;

PositionTrack location = {};

void PositionTrack::enable() {
    pinInputHandler.enable<ISRPin::D18, PinHandleMode::Change>();
    pinInputHandler.enable<ISRPin::D19, PinHandleMode::Change>();
    pinInputHandler.enable<ISRPin::D20, PinHandleMode::Change>();
    pinInputHandler.enable<ISRPin::D21, PinHandleMode::Change>();
}
