#include <Arduino.h>
#include "datenerfassung.h"
#include "energieauswertung.h"

float temp = 0.0;
float voltage = 0.0;
float current = 0.0;
float chargingCurrent = 0.0;

void updateSensorData(){
    temp = 20.0; // TODO
    voltage = getVoltage(1);
    current = getCurrent(1);
    chargingCurrent = getCurrent(2);
}