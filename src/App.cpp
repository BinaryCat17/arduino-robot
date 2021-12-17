#include "App.hpp"

//#define ENABLE_DEBUG

#if defined(ENABLE_DEBUG)
#include "Arduino.h"
#include "avr8-stub.h"
#else

#include <HardwareSerial.h>

#endif

void App::start(CustomApp *app) {
#if defined(USBCON)
    USBDevice.attach();
#endif

#if defined(ENABLE_DEBUG)
    init();
    debug_init();
#else
    Serial.begin(9600);
#endif

    app->init();

    App::println("Application started");

    while (true) {
        app->loop();
#if defined(ENABLE_DEBUG)
        if (serialEventRun) serialEventRun();
#endif
    }
}

void App::print(const char *msg) {
#if not defined(ENABLE_DEBUG)
    Serial.print(msg);
#endif
}

void App::print(float msg) {
#if not defined(ENABLE_DEBUG)
    Serial.print(msg);
#endif
}

void App::print(uint16_t msg) {
#if not defined(ENABLE_DEBUG)
    Serial.print(msg);
#endif
}

void App::print(uint32_t msg) {
#if not defined(ENABLE_DEBUG)
    Serial.print(msg);
#endif
}

void App::print(int16_t msg) {
#if not defined(ENABLE_DEBUG)
    Serial.print(msg);
#endif
}

void App::print(int32_t msg) {
#if not defined(ENABLE_DEBUG)
    Serial.print(msg);
#endif
}

void App::println(const char *msg) {
#if not defined(ENABLE_DEBUG)
    Serial.println(msg);
#endif
}

void App::println(float msg) {
#if not defined(ENABLE_DEBUG)
    Serial.println(msg);
#endif
}

void App::println(uint32_t msg) {
#if not defined(ENABLE_DEBUG)
    Serial.println(msg);
#endif
}

void App::println(uint16_t msg) {
#if not defined(ENABLE_DEBUG)
    Serial.println(msg);
#endif
}

void App::println(int32_t msg) {
#if not defined(ENABLE_DEBUG)
    Serial.println(msg);
#endif
}

void App::println(int16_t msg) {
#if not defined(ENABLE_DEBUG)
    Serial.println(msg);
#endif
}