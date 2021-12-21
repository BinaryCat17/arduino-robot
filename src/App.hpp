#pragma once
#include "stdint.h"

#define ENABLE_SERIAL
#if defined(ENABLE_DEBUG)
#include "Arduino.h"
#include "avr8-stub.h"
#endif

#if defined(ENABLE_SERIAL)
#include <HardwareSerial.h>
#endif

#define ARDUINO_LIB
#if defined(ARDUINO_LIB)
#include "Arduino.h"
#endif

class App {
public:
    template<typename T>
    static void print(T v)
    {
#if not defined(ENABLE_DEBUG) and defined(ENABLE_SERIAL)
        Serial.print(v);
#endif
    }

    template<typename T, typename... Types>
    static void print(T v, Types... args)
    {
        print(v);
        print(args...);
    }

    template<typename T>
    static void println(T v)
    {
#if not defined(ENABLE_DEBUG) and defined(ENABLE_SERIAL)
        Serial.println(v);
#endif
    }

    template<typename T, typename... Types>
    static void println(T v, Types... args)
    {
        print(v);
        println(args...);
    }

    template<typename T>
    static void start() {

#if defined(ARDUINO_LIB)
        init();
#endif

#if defined(ENABLE_DEBUG)
        debug_init();
#endif

#if defined(ENABLE_SERIAL) and not defined(ENABLE_DEBUG)
        Serial.begin(9600);
#endif

        App::println("Starting...");

        T app;
        app.init();

        while (true) {
            app.loop();
#if defined(ENABLE_DEBUG)
            if (serialEventRun) serialEventRun();
#endif
        }
    }
};

