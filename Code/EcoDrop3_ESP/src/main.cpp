#include <Arduino.h>
#include "communication.h"
#include "webinterface.h"
#include "datenerfassung.h"
#include "testfunctions.h"
#include <esp_partition.h>
#include "energieauswertung.h"

void currentTest();


int timeToUpdateSensorData = 5000; // Millisekunden

// Variablen um Zyklische Prozesse abzuarbeiten
unsigned long lastSensorDataUpdate = 0;
unsigned long lastSerialStatusUpdate = 0;

void setup() {
    Serial.begin(115200);
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