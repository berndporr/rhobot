
#include <iostream>

// for delay function.
#include <chrono>
#include <thread>

// for signal handling
#include <signal.h>

#include <JetsonGPIO.h>

using namespace std;

static bool end_this_program = false;

inline void delay(int s) { this_thread::sleep_for(chrono::seconds(s)); }

void signalHandler(int s) { end_this_program = true; }



int main() {
    // When CTRL+C pressed, signalHandler will be called
    signal(SIGINT, signalHandler);

    // Pin Definitions
    int input_pin = 18; // BOARD pin 12, BCM pin 18

    // Pin Setup.
    GPIO::setmode(GPIO::BCM);
    // set pin as an input pin
    GPIO::setup(input_pin, GPIO::IN);

    while (!end_this_program)
    {
        // first a simple test for GPIO in
        
        // read value of pin and print onto screen every second
        int curr_value = GPIO::input(input_pin);
        cout << "Reading " << curr_value << " at pin \n";
        delay(1);

        // for rotary encoder
        // read from pin and print on rising edges
        // calculate speed and print to screen
        // do for both individually and then together
    }

    GPIO::cleanup();

    return 0;
}