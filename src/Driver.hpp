#pragma once

#include "AVRLib/HardwareTimer.hpp"
#include "PositionTrack.hpp"
#include "Util/PIDRegulator.hpp"

using namespace AvrLib;

const double encMps = (10.0 / wheelLen) * avgEnc;

class Driver {
public:
    void enable();

    void speedL(float mmps)
    {
        mSpeedL = mmps / 1000.f;
    }

    void speedR(float mmps)
    {
        mSpeedR = mmps / 1000.f;
    }

    void correct();

    void rotate(float degrees);

    void fillL(float v) {
        return engineSpeed<DPin::D51>(v, [](auto v) { timerPWM3.fillFactor3(v); });
    }

    void fillR(float v) {
        return engineSpeed<DPin::D49>(v, [](auto v) { timerPWM3.fillFactor2(v); });
    }

    void stop()
    {
        fillL(0);
        fillR(0);
        mFactSumL = 0;
        mFactSumR = 0;
        mSumL = 0;
        mSumR = 0;
    }

    void move(float len)
    {
        mSumL = len;
        mSumR = len;
        speedL(0);
        speedR(0);
        while(!near(mSumL, 1.f, mFactSumL) && !near(mSumL, 1.f, mFactSumL))
        {
            correct();
        }
        stop();
    }

private:
    PIDRegulator mPidL = {};
    PIDRegulator mPidR = {};
    PIDRegulator mPidAngle = {};
    PIDRegulator mPidDistance = {};

    float mSpeedL = 0;
    float mSpeedR = 0;
    float mSumL = 0;
    float mSumR = 0;
    float mFactSumL = 0;
    float mFactSumR = 0;
    float mDt = 0;


    template<DPin control, typename FF>
    void engineSpeed(float speed, FF fillFactor) {
        if (speed > 0) {
            // крутим вперёд
            fillFactor(speed);
            pinWrite<PinLow, control>();
        } else {
            // нужно инвертировать значение, если крутим колесом назад
            fillFactor(100.0f + speed);
            pinWrite<PinHigh, control>();
        }
    }
};

extern Driver driver;