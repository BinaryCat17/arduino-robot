#pragma once
#include "stdint.h"

class CustomApp {
public:
    virtual void init() = 0;

    virtual void loop() = 0;
};

class App
{
public:
    static void start(CustomApp* app);

    static void print(char const* msg);

    static void print(float msg);

    static void print(uint32_t msg);

    static void print(uint16_t msg);

    static void print(int32_t msg);

    static void print(int16_t msg);

    static void println(char const* msg);

    static void println(float msg);

    static void println(uint32_t msg);

    static void println(uint16_t msg);

    static void println(int32_t msg);

    static void println(int16_t msg);

private:
    bool m_debug;
};

