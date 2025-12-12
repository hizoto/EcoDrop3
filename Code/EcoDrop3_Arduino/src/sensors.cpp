#include <Arduino.h>
#include <Wire.h>
#include "sensors.h"
#include <Adafruit_VL53L0X.h>
#include <TCA9548A-SOLDERED.h>
#include "communication.h"
                             // multiplexer
Adafruit_VL53L0X loxFront = Adafruit_VL53L0X();   // TOF
Adafruit_VL53L0X loxBack = Adafruit_VL53L0X();    // TOF

TCA9548A I2CMUX; 

// Multiplexer Channels
int tofFrontChannel = 0;
int tofBackChannel = 5;

void initSensors(){
  initMux();
  initTofBack();
  initTofFront();
  delay(100);
}

void initMux(){
  I2CMUX.begin();
  I2CMUX.closeAll();
}

void initTofFront(){
  I2CMUX.openChannel(tofFrontChannel);
  if(!loxFront.begin()){
    logMessage("Failed to boot Front TOF");   
    return;
  }
  I2CMUX.closeAll();
}

void initTofBack(){
  I2CMUX.openChannel(tofBackChannel);
  if(!loxBack.begin()){
    logMessage("Failed to boot Back TOF");    
    return;
  }
  I2CMUX.closeAll();
}


int readTofFront() {
  static int filteredDistance = 0;                // letzter geglätteter Wert
  static unsigned long lastRead = 0;
  const float alpha = 0.3;                        // Glättungsfaktor (0.1 = sehr weich, 0.5 = schneller)
  
  if(millis() - lastRead > 500){
    filteredDistance = 0;
  }

  VL53L0X_RangingMeasurementData_t measure;

  I2CMUX.openChannel(tofFrontChannel);
  loxFront.rangingTest(&measure, false);
  I2CMUX.closeAll();

  if (measure.RangeStatus == 4) {
    // Ungültige Messung, alten Wert zurückgeben
    return filteredDistance;
  }

  uint16_t newDistance = measure.RangeMilliMeter;

  // Wenn es der erste Durchlauf ist (noch kein Filterwert vorhanden)
  if (filteredDistance == 0) filteredDistance = newDistance;

  // Gleitende Mittelung
  filteredDistance = (int)(alpha * newDistance + (1.0 - alpha) * filteredDistance);

  return filteredDistance;
}

int readTofFrontUnfiltered() {
  VL53L0X_RangingMeasurementData_t measure;
  loxFront.rangingTest(&measure, false);

  return measure.RangeMilliMeter;
}

int readTofBackUnfiltered() {
  VL53L0X_RangingMeasurementData_t measure;
  I2CMUX.openChannel(tofBackChannel);
  loxBack.rangingTest(&measure, false);
  I2CMUX.closeAll();

  return measure.RangeMilliMeter;
}

int readTofBack() {
  static int filteredDistance = 0;   // letzter geglätteter Wert (bleibt zwischen Aufrufen erhalten)
  static unsigned long lastRead = 0;
  const float alpha = 0.3;           // Glättungsfaktor (0.1 = sehr weich, 0.5 = schneller)
  
  if(millis() - lastRead > 500){
    filteredDistance = 0;
  }

  VL53L0X_RangingMeasurementData_t measure;
  I2CMUX.openChannel(tofBackChannel);
  loxBack.rangingTest(&measure, false);
  I2CMUX.closeAll();

  if (measure.RangeStatus == 4) {
    // Ungültige Messung, alten Wert zurückgeben
    return filteredDistance;
  }

  uint16_t newDistance = measure.RangeMilliMeter;

  // Wenn es der erste Durchlauf ist (noch kein Filterwert vorhanden)
  if (filteredDistance == 0) filteredDistance = newDistance;

  // Gleitende Mittelung
  filteredDistance = (int)(alpha * newDistance + (1.0 - alpha) * filteredDistance);

  return filteredDistance;
}
