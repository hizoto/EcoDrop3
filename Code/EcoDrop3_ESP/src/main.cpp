#include <Arduino.h>
#include "communication.h"

int lastFinishedStep = 0;
int currentStep = 0;


void setup() {

}

void loop() {
    getComm(currentStep, lastFinishedStep);
    if (lastFinishedStep == currentStep && lastFinishedStep != 0){

    }
}