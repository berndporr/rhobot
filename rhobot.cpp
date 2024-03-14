#include "rhobot.h"
#include <iostream>

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

    GPIO::setmode(GPIO::BOARD);

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
    // use a callback function from jetsongpio to turn on and off at correct frequency and duty cycle
}

void RhoBot::start() {

    // this variable is used to ensure methods only execute after start has been called and things have been set up
    running = true;

    initPWM();
    initSoftPWM();
}

void RhoBot::stop() {

    if (!running) return;

    setLeftWheelSpeed(0.0);
    setRightWheelSpeed(0.0);
    // setLiDarMotorSpeed(0.0);

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
    
    std::cout << "changed left speed to " << std::to_string(leftWheelSpeed) << std::endl;
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

void RhoBot::moveInline(float distInMeters) {

}

void RhoBot::changeHeading(float angleInDegrees) {

}

void RhoBot::setLiDarMotorSpeed(float speed) {

}