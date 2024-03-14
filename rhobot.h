#ifndef RHOBOT_H
#define RHOBOT_H

#include <vector>
#include <JetsonGPIO.h>

// Rhoabot class for hardware communication (API)

class RhoBot
{

    public:

        // Starts the communication with the robot.
        void start();

        // Stops the communication with the Alphabot
        void stop();

        // Deconstructor
        ~RhoBot()
        {
            stop();
        }

        // Set the left wheel speed between -1 and +1
        void setLeftWheelSpeed(float speed);

        // Set the right wheel speed between -1 and +1
        void setRightWheelSpeed(float speed);

        // Move forward or backward a certain number of meters
        void moveInline(float distInMeters);

        // Turn left or right by a certain number of degrees
        void changeHeading(float angleInDegrees);

        // set LiDAR motor speed between -1 and +1
        void setLiDarMotorSpeed(float speed);

        // Get LiDAR data

        // Get camera data

    private:

        // Some attributes to track things
        static const int GPIO_MOTORL = 32;
        static const int GPIO_MOTORR = 33;
        static const int GPIO_LiDARMOTOR = 12; // note that these are BOARD numbering scheme

        static constexpr float pwmFrequency = 60.0; //Hz

        std::unique_ptr<GPIO::PWM> pwmLeftPointer;
        std::unique_ptr<GPIO::PWM> pwmRightPointer;

        float leftWheelSpeed = 0.0;
        float rightWheelSpeed = 0.0;

        bool running = false;

        // Find the necessary duty cyle for motors based on the speed (-1 to +1)
        float getDutyCycleFromSpeed(float speed);

        // Initialise both hardware pwm pins
        void initPWM();

        // Set up the software PWM
        void initSoftPWM();

};

#endif