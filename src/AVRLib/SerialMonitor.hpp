#pragma once

#include "stdint.h"

#if defined(ENABLE_DEBUG)
#include "Arduino.h"
#include "avr8-stub.h"
#endif

//#define ENABLE_SERIAL
#if defined(ENABLE_SERIAL)

#include <HardwareSerial.h>

#endif

namespace AvrLib {
    class SerialMonitor {
    public:
        template<typename T>
        void print(T v) {
#if not defined(ENABLE_DEBUG) and defined(ENABLE_SERIAL)
            Serial.print(v);
#endif
        }

        template<typename T, typename... Types>
        void print(T v, Types... args) {
            print(v);
            print(args...);
        }

        template<typename T>
        void println(T v) {
#if not defined(ENABLE_DEBUG) and defined(ENABLE_SERIAL)
            Serial.println(v);
#endif
        }

        template<typename T, typename... Types>
        void println(T v, Types... args) {
            print(v);
            println(args...);
        }
    };

    extern SerialMonitor monitor;
}
