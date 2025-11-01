#include "communication.h"
#include "sensors.h"

// Steps:
// 0 = Idle
// 1 = xxxx

int currentStep = 0;
bool isRunning = false;

void setup() {
    startComm();
    initSensors();
}

void loop() {
    /*
    getComm(&currentStep);
    isRunning = updateStatus();
    while(isRunning){
        switch(currentStep){
            // idle
            case 0:
                break;
            // kann noch genau definiert werden oder von Flowchart übernommen werden. Kommunikation ist notwendig.
            case 1:
                break;
            // fertig
            case 900:
                currentStep = 0;
                break;
        }
        sendStepFinished(currentStep);
    } */
    Serial.print("Distanz hinten: ");
    Serial.println(readTofBack());
    delay(1000);
    Serial.print("Distanz vorne: ");
    Serial.println(readTofFront());
    delay(1000);
}

