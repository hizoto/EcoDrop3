#include <Arduino.h>
#include "communication.h"
#include "webinterface.h"
#include "datenerfassung.h"
#include "testfunctions.h"

void currentTest();


int lastFinishedStep = 0;
int currentStep = 0;

int timeToUpdateSensorData = 5000; // Millisekunden

// Variablen um Zyklische Prozesse abzuarbeiten
unsigned long lastSensorDataUpdate = 0;
unsigned long lastSerialStatusUpdate = 0;

void setup() {
    startComm();
    //startFilesystem();
    //startWebinterface();
}

void loop() {
    if (millis() - lastSerialStatusUpdate >= 500){ 
        Serial.println("Serial is working");
        // currentTest();
    }
    /*
    getComm(currentStep, lastFinishedStep);
    if (lastFinishedStep == currentStep && lastFinishedStep != 0){

    }
    if (millis() - lastSensorDataUpdate >= timeToUpdateSensorData){
        updateSensorData();
        refreshWebinterface();
        lastSensorDataUpdate = millis();
    }
        */
}

void currentTest(){
    testLittleFS();
    //listFS();
}