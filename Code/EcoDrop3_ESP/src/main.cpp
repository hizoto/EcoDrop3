#include <Arduino.h>
#include "communication.h"
#include "webinterface.h"
#include "datenerfassung.h"
#include <esp_partition.h>
#include "energieauswertung.h"

void currentTest();


int timeToUpdateSensorData = 500; // Millisekunden

// Variablen um Zyklische Prozesse abzuarbeiten
unsigned long lastSensorDataUpdate = 0;
unsigned long lastSerialStatusUpdate = 0;

void setup() {
    startComm();
    startWebinterface();
    current_measure_init();
    updateSensorData();
}

void loop() {

    if (ArduinoSlave.available()) getComm();
    
    if (millis() - lastSensorDataUpdate >= timeToUpdateSensorData){
        updateSensorData();
        lastSensorDataUpdate = millis();
    }
}

void currentTest(){

}