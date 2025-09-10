#include <Arduino.h>

void getComm(int* stp){
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        if (input.length() > 0) {
            handleCommand(input, stp);
        }
    }
}

void logMessage(const char* msg){
  Serial.print("[LOG] ");
  Serial.println(msg);
}

void startComm(){
    Serial.begin(19200);
    logMessage("Kommunikation erfolgreich gestartet.");
}

void handleCommand(const String& cmd, int* stp) {
    if (cmd.startsWith("[STEP]")) {
        String numPart = cmd.substring(6);
        numPart.trim();

        *stp = numPart.toInt();
        
        // hier evtl. Acknowledgement senden
        sendStepStarted(*stp);
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