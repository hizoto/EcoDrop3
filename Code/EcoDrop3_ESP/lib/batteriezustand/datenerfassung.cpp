#include <Arduino.h>
#include "datenerfassung.h"
#include "energieauswertung.h"

void updateSensorData(){
    temp = 20.0; // TODO
    voltage = getVoltage(1);
    current = getCurrent(1);
    chargingCurrent = getCurrent(2);
}