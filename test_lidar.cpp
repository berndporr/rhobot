
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

	std::cout << "LiDAR test running." << std::endl;

    RhoBot rhobot;

    rhobot.start();

    float speed = 0.0;
    float incr = 0.1;

	// Testing the motor speed
    // Start from 0.0 speed, go up to 1.0, then back to 0.0
    for (int i = 0; i < 20; i++) {

        delay(0.5);
		
        speed += incr;

        if (speed >= 1.0 || speed <= 0) {
            incr = -incr;
        }

        rhobot.setLiDarMotorSpeed(speed);
    }

	rhobot.stop();

    std::cout << "Test complete." << std::endl;
    
    return 0;

}