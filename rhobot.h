#ifndef RHOBOT_H
#define RHOBOT_H

#include <vector>
#include <thread>
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
        void moveInline(float distInMeters, bool forward);

        // Turn left or right by a certain number of degrees
        void changeHeading(float angleInDegrees, bool clockwise);

        // set LiDAR motor speed between 0 and 1
        void setLiDarMotorSpeed(float speed);

        // Get LiDAR data

        // Get camera data

    private:

        // Motor pins
        // note that these are BOARD numbering scheme
        static const int GPIO_MOTORL = 32;
        static const int GPIO_MOTORR = 33;
        static const int GPIO_LiDARMOTOR = 12;

        // Pointers to PWM objects
        std::unique_ptr<GPIO::PWM> pwmLeftPointer;
        std::unique_ptr<GPIO::PWM> pwmRightPointer;
        
        // PWM parameters
        static constexpr float pwmFrequency = 60.0; // Hz
        float leftWheelSpeed = 0.0; // -1.0 to 1.0
        float rightWheelSpeed = 0.0; // -1.0 to 1.0

        // soft PWM paramteres
        static const int softPwmFrequency = 50; // Hz
        float lidarMotorSpeed = 0.0; // 0 to 1.0
        float lidarMotorOnTime = 0.0;
        float lidarMotorOffTime = 0.0;
        std::thread softPwmWorker;

        // Physcial measurements for open loop dynamics in meters and degrees
        float r_wheel = 0.065;
        float r_turn = 0.06;
        // Max angular speed of wheel in degrees/s
        float wheel_speed = 150;

        bool running = false;

        // Find the necessary duty cyle for motors based on the speed (-1 to +1)
        float getDutyCycleFromSpeed(float speed);

        // Initialise both hardware pwm pins
        void initPWM();

        // Set up the software PWM
        void initSoftPWM();

        // Do the actual pin control for the soft PWM on another thread
        void softPwmPinControl();

};

#endif
