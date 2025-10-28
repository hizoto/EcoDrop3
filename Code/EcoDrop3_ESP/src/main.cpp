#include <Arduino.h>
#include "communication.h"
#include "webinterface.h"
#include "datenerfassung.h"
#include "testfunctions.h"
#include <esp_partition.h>

void currentTest();


int timeToUpdateSensorData = 5000; // Millisekunden

// Variablen um Zyklische Prozesse abzuarbeiten
unsigned long lastSensorDataUpdate = 0;
unsigned long lastSerialStatusUpdate = 0;

void setup() {
    startComm();
    startWebinterface();
    updateSensorData();
}

void loop() {
    if (millis() - lastSensorDataUpdate > 2000){
        updateSensorData();
    }

    if (ArduinoSlave.available()) getComm();
    /*
    getComm(currentStep, lastFinishedStep);
    if (lastFinishedStep == currentStep && lastFinishedStep != 0){

    }
    if (millis() - lastSensorDataUpdate >= timeToUpdateSensorData){
        updateSensorData();
        refreshWebinterface();
        lastSensorDataUpdate = millis();
    } */
}

void currentTest(){
    testLittleFS();
    //listFS();
    /*auto it = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, nullptr);
    while (it) {
        const esp_partition_t* p = esp_partition_get(it);
        Serial.printf("Label=%s  type=%d  subtype=0x%02X  addr=0x%06X  size=%u\n",
                    p->label, p->type, p->subtype, p->address, p->size);
        it = esp_partition_next(it);
    }
    esp_partition_iterator_release(it);*/
}