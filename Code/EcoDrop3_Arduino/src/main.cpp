#include "communication.h"
#include "sensors.h"

// Steps:
// 0 = Idle
// 1 = xxxx

unsigned long lastLogMessage = 0;

int currentStep = 0;

void setup() {
    startComm();
    //initSensors();
    pinMode(12, OUTPUT);
    digitalWrite(12, LOW);
}

void loop() {
    if(Serial1.available()){
        getComm();
    }
    if(isRunning){
        switch(currentStep){
            // idle
            case 0:
                currentStep = 1;
                break;
            // kann noch genau definiert werden oder von Flowchart übernommen werden. Kommunikation ist notwendig.
            case 1:
                digitalWrite(12, HIGH);
                logMessage("EcoDrop on.");
                currentStep++;
                break;
            // fertig
            case 900:
                currentStep = 0;
                break;
        }
    }
    else {
        currentStep = 0;
        digitalWrite(12, LOW);
        if(millis() - lastLogMessage > 5000){
        logMessage("EcoDrop is now idle.");
        lastLogMessage = millis();
        }
    }
    
}

