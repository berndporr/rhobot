
#include <iostream>

// for delay function.
#include <chrono>
#include <map>
#include <string>
#include <thread>

#include "rhobot.h"

// delay function
inline void delay(double s) { std::this_thread::sleep_for(std::chrono::duration<double>(s)); }

int main()
{
    
    std::cout << "Motor test running." << std::endl;

    RhoBot rhobot;

    rhobot.start();

    float speed = 0.0;
    float incr = 0.1;

    // Start from 0.0 speed, go up to 1.0, then down to -1.0, then back to 0.0
    for (int i = 0; i < 40; i++) {

        delay(0.25);

        if (speed >= 1.0 || speed <= -1.0) {
            incr = -incr;
        }

        speed += incr;

        rhobot.setLeftWheelSpeed(speed);
        rhobot.setRightWheelSpeed(speed);
    }

    delay(0.25);

    speed = 0.0;
    incr = 0.1;

    // spin both directions
    for (int i = 0; i < 40; i++) {

        delay(0.25);

        if (speed >= 1.0 || speed <= -1.0) {
            incr = -incr;
        }

        speed += incr;

        rhobot.setLeftWheelSpeed(speed);
        rhobot.setRightWheelSpeed(-speed);
    }
    
    rhobot.stop();

    std::cout << "Test complete." << std::endl;
    
    return 0;
}