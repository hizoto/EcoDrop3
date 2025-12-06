#include <Arduino.h>
#include <Wire.h>
#include "sensors.h"
#include <Adafruit_VL53L0X.h>
#include <TCA9548A-SOLDERED.h>
#include "communication.h"
                             // multiplexer
Adafruit_VL53L0X loxFront = Adafruit_VL53L0X();   // TOF
Adafruit_VL53L0X loxBack = Adafruit_VL53L0X();    // TOF

#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

// Multiplexer Channels
int tofFrontChannel = 0;
int tofBackChannel = 1;
int tofFrontXShut = 23;
int tofBackXShut = 22;

void initSensors(){
  TOFsetID();
  delay(100);
}

void TOFsetID() {
  // all reset
  digitalWrite(tofFrontXShut, LOW);    
  digitalWrite(tofBackXShut, LOW);
  delay(10);
  // all unreset
  digitalWrite(tofFrontXShut, HIGH);
  digitalWrite(tofBackXShut, HIGH);
  delay(10);

  // activating LOX1 and resetting LOX2
  digitalWrite(tofFrontXShut, HIGH);
  digitalWrite(tofBackXShut, LOW);

  // initing LOX1
  if(!loxFront.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot front VL53L0X"));
    while(1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(tofBackXShut, HIGH);
  delay(10);

  //initing LOX2
  if(!loxBack.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot back VL53L0X"));
    while(1);
  }
}

int readTofFront() {
  static int filteredDistance = 0;                // letzter geglätteter Wert
  static unsigned long lastRead = 0;
  const float alpha = 0.3;                        // Glättungsfaktor (0.1 = sehr weich, 0.5 = schneller)
  
  if(millis() - lastRead > 500){
    filteredDistance = 0;
  }

  VL53L0X_RangingMeasurementData_t measure;
  loxFront.rangingTest(&measure, false);

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
  loxFront.rangingTest(&measure, false);

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
  loxBack.rangingTest(&measure, false);

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
