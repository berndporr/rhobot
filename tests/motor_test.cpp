// from stationary, smooth to full speed clockwise, smooth to full speed counterclockwise
// full speed clockwise, then full speed counterclockwise

#include <iostream>
// for delay function.
#include <chrono>
#include <map>
#include <string>
#include <thread>

// for signal handling
#include <signal.h>

#include <JetsonGPIO.h>

using namespace std;

int right_motor_pin = 32;
int left_motor_pin = 33;

int frequency = 60; // in Hz for a period of 16.67 ms, motor data sheet wants 20 ms between pulses but this seems to work!

inline void delay(double s) { this_thread::sleep_for(std::chrono::duration<double>(s)); }

static bool running = false;

void signalHandler(int s) { running = true; }

float getDutyCycleFromSpeed(float speed)
{
    // take in speed from -1 to 1 (anticlockwise to clockwise) and return the calibrated pwm for the motors
    // motors want an on time of 1.5 ms for stationary, 1.7 ms for max anticlockwise and 1.3 for max clockwise
    
    // map speed from -1 -> 1 to 0.0013 -> 0.0017
    float onTime = ( (speed + 1.0) / 2.0 ) * 0.0004 + 0.0013; // first goes from -1 -> 1 to 0 -> 1, then 0 -> 0.004 and finally 0.0013 -> 0.0017

    float period = 1.0f / (float)frequency;

    float dutyCycle = onTime / period * 100.0;
    
    return dutyCycle;
}

int main()
{

    cout << "Motor test running. Press CTRL+C to exit." << endl;

    // Pin Setup.
    // Board pin-numbering scheme
    GPIO::setmode(GPIO::BOARD);

    // set pins as output pins with optional initial state of HIGH
    GPIO::setup(left_motor_pin, GPIO::OUT, GPIO::HIGH);
    GPIO::PWM pwmLeft(left_motor_pin, 50);
    GPIO::setup(right_motor_pin, GPIO::OUT, GPIO::HIGH);
    GPIO::PWM pwmRight(right_motor_pin, 50);

    // set frequency
    pwmLeft.ChangeFrequency(frequency);
    pwmRight.ChangeFrequency(frequency);

    // initsalize with zero speed
    float speed = 0.0;
    float dutyCycle = getDutyCycleFromSpeed(speed);
    pwmLeft.start(dutyCycle);
    pwmRight.start(dutyCycle);

    float incr = 0.1;
    int i = 0;
    cout << "Loop " + to_string(++i) << endl;

    signal(SIGINT, signalHandler); // When CTRL+C pressed, signalHandler will be called

    while (!running)
    {
        delay(0.1);

        if (speed >= 1.0){
            incr = -incr;
        }
        if (speed <= -1.0){
            incr = -incr;
            cout << "Loop " + to_string(++i) << endl;
        }

        speed += incr;

        pwmLeft.ChangeDutyCycle(getDutyCycleFromSpeed(speed));
        pwmRight.ChangeDutyCycle(getDutyCycleFromSpeed(-speed));

    }

    pwmLeft.stop();
    pwmRight.stop();
    GPIO::cleanup();

    return 0;
}