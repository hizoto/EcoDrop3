#include <Arduino.h>
#include "communication.h"
#include "webinterface.h"
#include "datenerfassung.h"

int lastFinishedStep = 0;
int currentStep = 0;

int timeToUpdateSensorData = 5000; // Millisekunden

// Variablen um Zyklische Prozesse abzuarbeiten
unsigned long lastSensorDataUpdate = 0;

void setup() {
    startComm();
    startWebinterface();
}

void loop() {
    getComm(currentStep, lastFinishedStep);
    if (lastFinishedStep == currentStep && lastFinishedStep != 0){

    }
    if (millis() - lastSensorDataUpdate >= timeToUpdateSensorData){
        updateSensorData();
        refreshWebinterface();
        lastSensorDataUpdate = millis();
    }
}