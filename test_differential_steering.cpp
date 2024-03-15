#include "rhobot.h"

int main(){
    
    RhoBot rhobot;

    rhobot.start();
    
    // move forward 30 cm
    rhobot.moveInline(0.3, true);

    // move backward 30 cm
    rhobot.moveInline(0.3, false);

    // move in a 20 cm square
    for (int i = 0; i < 4; i++) {
        rhobot.moveInline(0.2, true);
        rhobot.changeHeading(90, true);
    }

    // go backwards in a 20 cm square
    for (int i = 0; i < 4; i++) {
        rhobot.moveInline(0.2, false);
        rhobot.changeHeading(90, false);
    }

    rhobot.stop();

}