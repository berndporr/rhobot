#include "rhobot.h"

#include <chrono>
#include <map>
#include <string>

float RhoBot::getDutyCycleFromSpeed(float speed) {
    // take in speed from -1 to 1 (backwards to forwards) and return the calibrated pwm for the motors
    // motor datasheet requests an on time of 1.5 ms for stationary, 1.7 ms for max anticlockwise and 1.3 for max clockwise
    
    // map speed from -1 -> 1 to 0.0013 -> 0.0017
    // first goes from -1 -> 1 to 0 -> 1, then 0 -> 0.004 and finally 0.0013 -> 0.0017
    float onTime = ( (speed + 1.0) / 2.0 ) * 0.0004 + 0.0013;

    float period = 1.0 / pwmFrequency;
    float dutyCycle = onTime / period * 100.0;
    
    return dutyCycle;
}

void RhoBot::initPWM() {

    // create the pwm objects and reset the pointers to point at them
    GPIO::setup(GPIO_MOTORL, GPIO::OUT, GPIO::HIGH);
    std::unique_ptr<GPIO::PWM> pwmLeft = std::make_unique<GPIO::PWM>(GPIO::PWM(GPIO_MOTORL, 50));
    pwmLeftPointer = std::move(pwmLeft);

    GPIO::setup(GPIO_MOTORR, GPIO::OUT, GPIO::HIGH);
    std::unique_ptr<GPIO::PWM> pwmRight = std::make_unique<GPIO::PWM>(GPIO::PWM(GPIO_MOTORR, 50));
    pwmRightPointer = std::move(pwmRight);

    // set frequency of the pwms
    pwmLeftPointer->ChangeFrequency(pwmFrequency);
    pwmRightPointer->ChangeFrequency(pwmFrequency);

    pwmLeftPointer->start(getDutyCycleFromSpeed(0.0));
    pwmRightPointer->start(getDutyCycleFromSpeed(0.0));

}

void RhoBot::initSoftPWM() {

    // pin set up as gpio out
    GPIO::setup(GPIO_LiDARMOTOR, GPIO::OUT, GPIO::LOW);

    // set pin control function to run in antoher thread
    softPwmWorker = std::thread(&RhoBot::softPwmPinControl, this);

}

void RhoBot::start() {

    // this variable is used to ensure methods only execute after start has been called and things have been set up
    running = true;

    GPIO::setmode(GPIO::BOARD);
    GPIO::setwarnings(false);
    initPWM();
    initSoftPWM();
}

void RhoBot::stop() {

    if (!running) return;

    setLeftWheelSpeed(0.0);
    setRightWheelSpeed(0.0);
    setLiDarMotorSpeed(0.0);
    
    running = false;

    // join this worker back
    softPwmWorker.join();

    // stop the pwm and clean up gpio
    GPIO::output(GPIO_LiDARMOTOR, GPIO::LOW);
    pwmLeftPointer->stop();
    pwmRightPointer->stop();
    GPIO::cleanup();
}

void RhoBot::setLeftWheelSpeed(float speed) {

    if (!running) return;

    // make sure speed is in the right bounds
    if (speed < -1)
                speed = -1;
    if (speed > 1)
            speed = 1;
    
    leftWheelSpeed = speed;
    pwmLeftPointer->ChangeDutyCycle(getDutyCycleFromSpeed(speed));

}

void RhoBot::setRightWheelSpeed(float speed) {

    if (!running) return;

    // make sure speed is in the right bounds
    if (speed < -1)
                speed = -1;
    if (speed > 1)
            speed = 1;
    
    // minus speed so +1 is forward for both motors
    leftWheelSpeed = -speed;
    pwmRightPointer->ChangeDutyCycle(getDutyCycleFromSpeed(-speed));
}


void RhoBot::moveInline(float distInMeters, bool forward) {

    if (!running) return;

    // rough estimation of open loop dynamics to move inline a distance in meters
    // this will be improved by the addition of speed sensors on both wheels to provide feedback

    float onTimeInSeconds = distInMeters * 360.0 / (2 * 3.1415 * r_wheel * wheel_speed);
    
    float speed = 1.0;
    if (!forward) {speed = -speed;}

    setLeftWheelSpeed(speed);
    setRightWheelSpeed(speed);

    std::this_thread::sleep_for(std::chrono::duration<double>(onTimeInSeconds));

    setLeftWheelSpeed(0.0);
    setRightWheelSpeed(0.0);

}

void RhoBot::changeHeading(float angleInDegrees, bool clockwise) {

    if (!running) return;

    // rough estimation of open loop dynamics to move inline a distance in meters
    // this will be improved by the addition of speed sensors on both wheels to provide feedback

    float onTimeInSeconds = angleInDegrees * r_turn / (r_wheel * wheel_speed);
    
    float speed = 1.0;
    if (!clockwise) {speed = -speed;}

    setLeftWheelSpeed(speed);
    setRightWheelSpeed(-speed);

    std::this_thread::sleep_for(std::chrono::duration<double>(onTimeInSeconds));

    setLeftWheelSpeed(0.0);
    setRightWheelSpeed(0.0);

}

void RhoBot::setLiDarMotorSpeed(float speed) {

    if (!running) return;

    // make sure speed is in the right bounds
    if (speed < 0.0)
            speed = 0.0;
    if (speed > 1.0)
            speed = 1.0;

    // this is effectively the duty cycle
    lidarMotorSpeed = speed;

    // calculate the on time here so don't have to repeat the computation
    lidarMotorOntime = lidarMotorSpeed * (1.0 / (float)softPwmFrequency); // in seconds

}


void RhoBot::softPwmPinControl() {

    while (running) { // from start called to stop called

        // turn high, sleep for on time and turn low
        GPIO::output(GPIO_LiDARMOTOR, GPIO::HIGH);
        std::this_thread::sleep_for(std::chrono::duration<double>(lidarMotorOntime));
        GPIO::output(GPIO_LiDARMOTOR, GPIO::LOW);
    }

}