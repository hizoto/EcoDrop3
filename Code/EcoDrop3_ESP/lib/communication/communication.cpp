#include <Arduino.h>
#include "communication.h"
#include "webinterface.h"


// UART für Arduino
#define RX_PIN D8   // an Arduino TX
#define TX_PIN D9   // an Arduino RX
#define SEND_EMERGENCY_STOP_PIN D2


HardwareSerial& ArduinoSlave = Serial2;

void startComm(){
    Serial.begin(115200);
    ArduinoSlave.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
    pinMode(SEND_EMERGENCY_STOP_PIN, OUTPUT);
    digitalWrite(SEND_EMERGENCY_STOP_PIN, LOW);    
    delay(3000);
    logToWebinterface("Serial communication ready.");
    Serial.println("Serial communication ready.");
}

void getComm(){
    while (ArduinoSlave.available()) {
        String msg = Serial2.readStringUntil('\n');
        msg.trim();

        if (msg.startsWith("[LOG]")){
            String logmsg = msg.substring(5);
            logToWebinterface(logmsg);
        }

        else if (msg.startsWith("[WARNUNG]")){
            String warnung = msg.substring(9);
            logToWebinterface("****************************************");
            logToWebinterface("!!! WARNUNG !!!");
            logToWebinterface(warnung);
            logToWebinterface("****************************************");
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

void sendEmergencyStop(){
    digitalWrite(SEND_EMERGENCY_STOP_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(SEND_EMERGENCY_STOP_PIN, LOW);
    logToWebinterface("EMERGENCY STOP");
}


void sendStartSignal(){
    ArduinoSlave.write("[START]");
}


void sendStopSignal(){
    ArduinoSlave.write("[STOP]");
}


void saveSensorDataToWebinterface(){
    
}