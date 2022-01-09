#include "PositionTrack.hpp"

volatile int32_t implEncCntL = 0;
volatile int32_t implEncCntR = 0;

volatile uint8_t implEncMask1 = 0;
volatile uint8_t implEncMask2 = 0;

PositionTrack location = {};

void PositionTrack::enable() {
    deltaTimer.enable();
    pinInputHandler.enable<ISRPin::D18, PinHandleMode::Change>();
    pinInputHandler.enable<ISRPin::D19, PinHandleMode::Change>();
    pinInputHandler.enable<ISRPin::D20, PinHandleMode::Change>();
    pinInputHandler.enable<ISRPin::D21, PinHandleMode::Change>();
}
