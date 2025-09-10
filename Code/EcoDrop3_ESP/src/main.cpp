#include <Arduino.h>
#include "communication.h"
#include "webinterface.h"

int lastFinishedStep = 0;
int currentStep = 0;


void setup() {
    startComm();
    startWebinterface();
}

void loop() {
    getComm(currentStep, lastFinishedStep);
    if (lastFinishedStep == currentStep && lastFinishedStep != 0){

    }
}