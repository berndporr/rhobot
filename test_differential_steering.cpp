#include "rhobot.h"

int main(){
    
    RhoBot rhobot;

    rhobot.start();

    // move in a 15 cm octagon
    for (int i = 0; i < 8; i++) {
        rhobot.moveInline(0.15, true);
        rhobot.changeHeading(45, true);
    }

    // go backwards in a 15 cm octagon
    for (int i = 0; i < 8; i++) {
        rhobot.changeHeading(45, false);
        rhobot.moveInline(0.15, false);
    }

    rhobot.stop();

}