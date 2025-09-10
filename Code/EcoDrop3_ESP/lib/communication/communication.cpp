#include <Arduino.h>
#include "communication.h"


// UART für Arduino
#define RX_PIN 16   // an Arduino TX
#define TX_PIN 17   // an Arduino RX

void startComm(){
    Serial.begin(19200);
    Serial2.begin(19200, SERIAL_8N1, RX_PIN, TX_PIN);
}

void getComm(int& currentStep, int& lastFinishedStep){
    if (Serial2.available()) {
        String msg = Serial2.readStringUntil('\n');
        msg.trim();
        if (msg.startsWith("[FINISHED]")){
            lastFinishedStep = msg.substring(10).toInt();
        }
        else if (msg.startsWith("[STARTED]")){
            currentStep = msg.substring(9).toInt();
        }
    }
}