#include <Arduino.h>
#include "communication.h"
#include "sensors.h"

// Steps:
// 0 = Idle
// 1 = xxxx

int currentStep = 0;


void setup() {
}

void loop() {
    getComm(&currentStep);
}