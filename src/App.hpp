#pragma once
#include "stdint.h"
#include "LineDriver.hpp"

class Main
{
public:
    void setup() {
        deltaTimer.enable();

        lineRobot.enable(5.0f, 0.0f, 0.0f);
        lineRobot.speed(20.f);
    }

    void loop() {
        deltaTimer.loopPassed();
        lineRobot.drive();
    }
};


//volatile int32_t enc_cntL = 0;
//volatile int32_t enc_cntR = 0;
//
//volatile int32_t prev_enc_cntL = 0;
//volatile int32_t prev_enc_cntR = 0;
//
//volatile uint8_t mask1 = 0;
//volatile uint8_t mask2 = 0;
//
//volatile float speed1 = 0;
//
//const int encAPin1 = 18;
//const int encBPin1 = 19;
//
//const int encAPin2 = 20;
//const int encBPin2 = 21;
//
//const float wheelLen = 21.3f;
//const float avgEnc = 21.3f;
//
//
//
//template<>
//class UserApp<Custom> {
//public:
//    static void init() {
//        // настраиваем прерывания на режим CHANGE
//        cli();
//        InterGen::enable<Pin::D18, InterruptHandleMode::Change>();
//        InterGen::enable<Pin::D19, InterruptHandleMode::Change>();
//
//        unsigned long ctc_match_overflow = ((16000000 * 1) / 1024) - 1;
//
//        // CTC counter mode
//        TCCR4A &= ~(1 << WGM40);
//        TCCR4A &= ~(1 << WGM41);
//        TCCR4B |= (1 << WGM42);
//
//        // clear on compare
//        // TCCR4A |= (1 << COM4A1);
//        // TCCR4A &= ~(1 << COM4A0);
//
//        // enable interruptions
//        TIMSK4 |= (1 << OCIE4A);
//
//        // 1024 prescaling
//        TCCR4B |= (1 << CS42) | (1 << CS40);
//
//        TCNT4 = 0u;
//        OCR4A = ctc_match_overflow;
//
//        sei();
//    }
//
//    static void loop() {
//        //App::print("L ", enc_cntL);
//        //App::println(" R ", enc_cntR);
//
//        App::print("L ", enc_cntL * wheelLen / avgEnc);
//        App::print(" R ", enc_cntR * wheelLen / avgEnc, "cm");
//        App::println(" S ", speed1);
//    }
//
//    static void encInter1()
//    {
//        encInter(enc_cntR, mask1, encAPin1, encBPin1);
//    }
//
//    static void encInter2()
//    {
//        encInter(enc_cntL, mask2, encAPin2, encBPin2);
//    }
//
//    static void encInter(volatile int32_t& cnt, volatile uint8_t& mask, int pin1, int pin2) {
//        // первые два бита - значения a и b текущие показания энкодера соответственно
//        // последние два бита - предыдущие значения a и b соответственно
//
//        // 6.79 см, 21,3 см, 2437 делений
//        // 0100 0010 1011 1101 по часовой стрелке
//        // 0111 1110 1000 0001 против часовой стрелки
//
//        auto const a = PinMap::read<Pin::D18>();
//        auto const b = PinMap::read<Pin::D19>();
//
//        mask <<= 2;
//        PinMap::setBit(mask, 0, b);
//        PinMap::setBit(mask, 1, a);
//        mask &= 0b00001111;
//
//        if(mask == 0b0100 || mask == 0b0010 || mask == 0b1011 || mask == 0b1101)
//        {
//            ++cnt;
//        } else if (mask == 0b0111 || mask == 0b1110 || mask == 0b1000 || mask == 0b0001)
//        {
//            --cnt;
//        }
//    }
//};

//ISR(TIMER4_COMPA_vect) {
//        speed1 = (enc_cntL - prev_enc_cntL) * wheelLen / avgEnc;
//
//        prev_enc_cntL = enc_cntL;
//        prev_enc_cntR = enc_cntR;
//}
