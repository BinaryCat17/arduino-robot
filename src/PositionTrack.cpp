#include "PositionTrack.hpp"

volatile int32_t implEncCntL = 0;
volatile int32_t implEncCntR = 0;

volatile uint8_t implEncMask1 = 0;
volatile uint8_t implEncMask2 = 0;

PositionTrack location = {};

void PositionTrack::enable() {
    pinInputHandler.enable<ISRPin::D18, PinHandleMode::Change>();
    pinInputHandler.enable<ISRPin::D19, PinHandleMode::Change>();
    pinInputHandler.enable<ISRPin::D20, PinHandleMode::Change>();
    pinInputHandler.enable<ISRPin::D21, PinHandleMode::Change>();
}

void PositionTrack::track() {
    int32_t encCntL, encCntR;

    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        encCntL = implEncCntL;
        encCntR = implEncCntR;
    }

    // считываем координаты робота x, y и расстояние которое проехало каждое колесо
    mRightMovement = (float) (encCntR - mPrevEncCntR) * wheelLen / avgEncR;
    mLeftMovement = (float) (encCntL - mPrevEncCntL) * wheelLen / avgEncL;

    double const dif = mRightMovement - mLeftMovement;

    // угол поворота робота с момента предыдущего измерения
    double const angle = dif / wheelbase;
    double const sinA = sin(angle);
    double const cosA = cos(angle);

    // синус и косинус угла робота, которы был до нового поворота
    double const sinR = sin(mRotationRadians);
    double const cosR = cos(mRotationRadians);

    double const mx = (-baseRadius * sinA);
    double const my = baseRadius * (1.0 / cosA - 1.0) * cosA;

    // поворот совершается в сторону колеса с меньшей скоростью.
    // меньшая скорость используется для движения по вектору направления робота,
    // разность скоростей определяет угол поворота
    if(fabs(mRightMovement) > fabs(mLeftMovement))
    {
        mPosY -= (cosR * mx - sinR * my) + mLeftMovement * cosR;
        mPosX += (sinR * mx - cosR * my) + mLeftMovement * sinR;
    } else
    {
        mPosY += (cosR * mx - sinR * my) + mRightMovement * cosR;
        mPosX -= (sinR * mx - cosR * my) + mRightMovement * sinR;
    }

    mRotationRadians += angle;
    mPrevEncCntL = encCntL;
    mPrevEncCntR = encCntR;
}
