
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

int left_motor_pin = 32;
int right_motor_pin = 33;

// in Hz for a period of 16.67 ms, motor data sheet requests 20 ms between pulses but this seems to work best
int frequency = 60;

// delay function
inline void delay(double s) { this_thread::sleep_for(std::chrono::duration<double>(s)); }

// variable to store if CTRL+C is pressed
static bool running = true;

// function to break out of loops so pwm can be cleaned up when exiting program early
void signalHandler(int s) {
    running = false;
    }

float getDutyCycleFromSpeed(float speed)
{
    // take in speed from -1 to 1 (backwards to forwards) and return the calibrated pwm for the motors
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

    // Board pin-numbering scheme
    GPIO::setmode(GPIO::BOARD);

    // setup pins as pwms
    GPIO::setup(left_motor_pin, GPIO::OUT, GPIO::HIGH);
    GPIO::PWM pwmLeft(left_motor_pin, 50);
    GPIO::setup(right_motor_pin, GPIO::OUT, GPIO::HIGH);
    GPIO::PWM pwmRight(right_motor_pin, 50);

    // set frequency of the pwms
    pwmLeft.ChangeFrequency(frequency);
    pwmRight.ChangeFrequency(frequency);

    // initsalize with zero speed
    float speed = 0.0;
    float dutyCycle = getDutyCycleFromSpeed(speed);

    pwmLeft.start(dutyCycle);
    pwmRight.start(dutyCycle);

    // increment the speed each loop iteration
    float incr = 0.1;

    // track how many loops so can break out after one
    int i = 0;

    // When CTRL+C pressed, signalHandler will break out of loops to clean up pwm
    signal(SIGINT, signalHandler);

    if (running){ cout << "Left motor from stationary, smooth to full speed forward, smooth to full speed backward." << endl; }
    // so starting from 0.0 speed, going up to 1.0, and then down to -1.0 and back to 0.0
    while (running) {

        if (speed <= 0.05 && speed >= -0.05){
            i += 1;

            // i will equal 3 after its gone to full speed in both directions and then come back to zero
            if (i == 3){
                // reset for next section
                i = 0;
                incr = 0.1;
                speed = 0.0;
                break;
            }
        }

        delay(0.25);

        if (speed >= 1.0)
            incr = -incr;
        if (speed <= -1.0)
            incr = -incr;

        speed += incr;

        pwmLeft.ChangeDutyCycle(getDutyCycleFromSpeed(speed));
        pwmRight.ChangeDutyCycle(getDutyCycleFromSpeed(0.0));
    }

    if (running){ cout << "Right motor from stationary, smooth to full speed forward, smooth to full speed backward." << endl; }
    // so starting from 0.0 speed, going up to 1.0, and then down to -1.0 and back to 0.0
    while (running) {

        if (speed <= 0.05 && speed >= -0.05){
            i += 1;

            // i will equal 3 after its gone to full speed in both directions and then come back to zero
            if (i == 3){
                // reset for next section
                i = 0;
                incr = 0.1;
                speed = 0.0;
                break;
            }
        }

        delay(0.25);

        if (speed >= 1.0)
            incr = -incr;
        if (speed <= -1.0)
            incr = -incr;

        speed += incr;
        
        pwmLeft.ChangeDutyCycle(getDutyCycleFromSpeed(0.0));
        pwmRight.ChangeDutyCycle(getDutyCycleFromSpeed(-speed));
    }

    if (running){ cout << "Both motors from stationary, smooth to full speed forward, smooth to full speed backward." << endl; }
    // so starting from 0.0 speed, going up to 1.0, and then down to -1.0 and back to 0.0
    while (running) {

        if (speed <= 0.05 && speed >= -0.05){
            i += 1;

            // i will equal 3 after its gone to full speed in both directions and then come back to zero
            if (i == 3){
                // reset for next section
                i = 0;
                incr = 0.1;
                speed = 0.0;
                break;
            }
        }

        delay(0.25);

        if (speed >= 1.0)
            incr = -incr;
        if (speed <= -1.0)
            incr = -incr;

        speed += incr;

        pwmLeft.ChangeDutyCycle(getDutyCycleFromSpeed(speed));
        pwmRight.ChangeDutyCycle(getDutyCycleFromSpeed(-speed));
    }

    if (running){
        cout << "Full speed left, then full speed right, full speed forwards, full speed backwards." << endl;
        delay(0.5);

        pwmLeft.ChangeDutyCycle(getDutyCycleFromSpeed(1.0));
        pwmRight.ChangeDutyCycle(getDutyCycleFromSpeed(1.0));
        delay(3);
    }

    if (running){
        pwmLeft.ChangeDutyCycle(getDutyCycleFromSpeed(-1.0));
        pwmRight.ChangeDutyCycle(getDutyCycleFromSpeed(-1.0));
        delay(3);
    }

    if (running){
        pwmLeft.ChangeDutyCycle(getDutyCycleFromSpeed(1.0));
        pwmRight.ChangeDutyCycle(getDutyCycleFromSpeed(-1.0));
        delay(3);
    }

    if (running){
        pwmLeft.ChangeDutyCycle(getDutyCycleFromSpeed(-1.0));
        pwmRight.ChangeDutyCycle(getDutyCycleFromSpeed(1.0));
        delay(3);
    }

    // clean up the pwm before closing
    pwmLeft.stop();
    pwmRight.stop();
    GPIO::cleanup();

    if (running){
        cout << "Test complete." << endl;
    } 
    else {
        cout << "Test terminated." << endl;
    }
    
    return 0;
}