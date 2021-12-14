#pragma once

class Driver {
public:
    static void enable();

    static void speed(float speed);

    static void correct();

    static Driver& instance();

private:
    float m_robotSpeed = 0.3;
};
