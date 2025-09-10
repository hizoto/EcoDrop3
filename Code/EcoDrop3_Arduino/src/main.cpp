#include <Arduino.h>
#include "communication.h"
#include "sensors.h"

// Steps:
// 0 = Idle
// 1 = xxxx

int currentStep = 0;


void setup() {
    void startComm();
}

void loop() {
    getComm(&currentStep);
    switch(currentStep){
        // idle
        case 0:
            break;
        // kann noch genau definiert werden oder von Flowchart übernommen werden. Kommunikation ist notwendig.
        case 1:
            break;
    }
}
