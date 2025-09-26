#include <Arduino.h>
#include "communication.h"
#include "webinterface.h"


// UART für Arduino
#define RX_PIN D8   // an Arduino TX
#define TX_PIN D9   // an Arduino RX

HardwareSerial& Arduino = Serial2;

void startComm(){
    Serial.begin(19200);
    Arduino.begin(19200, SERIAL_8N1, RX_PIN, TX_PIN);
    logToWebinterface("Serial communication ready.");
}

void getComm(int& currentStep, int& lastFinishedStep){
    if (Arduino.available()) {
        String msg = Serial2.readStringUntil('\n');
        msg.trim();
        if (msg.startsWith("[FINISHED]")){
            lastFinishedStep = msg.substring(10).toInt();
        }
        
        else if (msg.startsWith("[STARTED]")){
            currentStep = msg.substring(9).toInt();
        }

        else if (msg.startsWith("[LOG]")){
            String logmsg = msg.substring(5);
            logToWebinterface(logmsg);
        }

        else {
            logToWebinterface(" ");
            logToWebinterface("****************************************");
            logToWebinterface("!!! CRITICAL ERROR !!!");
            logToWebinterface("RECEIVED UNKNOWN MESSAGE FROM ARDUINO:");
            logToWebinterface(msg);
            Serial.print("unknown communication received : ");
            Serial.println(msg);
            logToWebinterface("****************************************");
            logToWebinterface(" ");
        }
    }
}