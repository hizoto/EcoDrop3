#include "communication.h"

#define EMERGENCYSTOP_PIN 2

bool signalIsRunning = false;



void getComm(){
    while (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        if (input.length() > 0) {
            handleCommand(input);
        }
    }
}

void logMessage(const char* msg){
  Serial.print("[LOG]");
  Serial.println(msg);
}

void startComm(){
    Serial.begin(115200);
    pinMode(EMERGENCYSTOP_PIN, INPUT_PULLUP);
    attachInterrupt(EMERGENCYSTOP_PIN, emergencystop, RISING);
    logMessage("Kommunikation von Arduino erfolgreich gestartet.");
}

void handleCommand(const String& cmd) {
    if (cmd.startsWith("[START]")) {
        signalIsRunning = true;
    }

    else if (cmd.startsWith("[STOP]")) {
        signalIsRunning = false;
    }

    else {
        logMessage("[WARNUNG] unbekannter Befehl!!");
    }
}

void sendStepFinished(int step){
    Serial.print("[FINISHED]");
    Serial.println(step);
}

void sendStepStarted(int step){
    Serial.print("[STARTED]");
    Serial.println(step);
}

bool updateStatus(){
    return signalIsRunning;
}

void emergencystop(){
    signalIsRunning = false;
}