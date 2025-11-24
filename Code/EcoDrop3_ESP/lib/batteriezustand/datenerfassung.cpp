#include <Arduino.h>
#include "datenerfassung.h"
#include "energieauswertung.h"

float temp = 0.0;
float voltage = 0.0;
float current = 0.0;
float chargingCurrent = 0.0;
float chargingVoltage = 0.0;

void updateSensorData(){
    temp = 20.0; // TODO
    voltage = 3.7 * 3; //getVoltage(1);
    current = 0.500; //getCurrent(1);
    //chargingCurrent = getCurrent(2);
    //chargingVoltage = getVoltage(2);
}