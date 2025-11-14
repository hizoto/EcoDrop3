#include <Arduino.h>
#include <Wire.h>
#include "sensors.h"
#include <Adafruit_VL53L0X.h>
#include <TCA9548A-SOLDERED.h>
#include "communication.h"
#include "pins.h"
#include "config.h"

TCA9548A I2CMUX;
Adafruit_VL53L0X loxFront = Adafruit_VL53L0X();
Adafruit_VL53L0X loxBack = Adafruit_VL53L0X();

int samples = 5;


int tofFrontChannel = 0;
int tofBackChannel = 1;

void initSensors(){
  initMux();
  initTofBack();
  initTofFront();
  delay(100);
}

void initMux(){
  I2CMUX.begin();-
  I2CMUX.closeAll();
}

void initTofFront(){
  I2CMUX.openChannel(tofFrontChannel);
  if(!loxFront.begin()){
    Serial.println("Failed to boot Front TOF"); // TODO
    return;
  }
  I2CMUX.closeAll();
}

void initTofBack(){
  I2CMUX.openChannel(tofBackChannel);
  if(!loxBack.begin()){
    Serial.println("Failed to boot Back TOF"); // TODO
    return;
  }
  I2CMUX.closeAll();
}

uint16_t readTofFront(){
  I2CMUX.openChannel(tofFrontChannel);
  VL53L0X_RangingMeasurementData_t measure;
  loxFront.rangingTest(&measure, false);
  I2CMUX.closeAll();
  if (measure.RangeStatus != 4) {
    uint16_t distance = measure.RangeMilliMeter;
    return distance;
  }
}

uint16_t readTofBack(){
  I2CMUX.openChannel(tofBackChannel);
  VL53L0X_RangingMeasurementData_t measure;
  loxBack.rangingTest(&measure, false);
  I2CMUX.closeAll();
  if (measure.RangeStatus != 4) {
    uint16_t distance = measure.RangeMilliMeter;
    return distance;
  }
}

/*int averageTofFront(){
  int sum = 0;
  I2CMUX.openChannel(tofFrontChannel);
  for (int i = 0; i < samples; i++){
    VL53L0X_RangingMeasurementData_t measure;
    loxFront.rangingTest(&measure, false);
      if (measure.RangeStatus != 4) {
    uint16_t distance = measure.RangeMilliMeter;
    sum += distance;
    }
  }
  I2CMUX.closeAll();
  sum = sum / 5;
  return sum;
} 

int averageTofBack(){
  int sum = 0;
  I2CMUX.openChannel(tofBackChannel);
  for (int i = 0; i < samples; i++){
    VL53L0X_RangingMeasurementData_t measure;
    loxFront.rangingTest(&measure, false);
      if (measure.RangeStatus != 4) {
    uint16_t distance = measure.RangeMilliMeter;
    sum += distance;
    }
  }
  I2CMUX.closeAll();
  sum = sum / 5;
  return sum;
}

*/

int averageTofFront() {
  static int filteredDistance = 0;   // letzter geglätteter Wert (bleibt zwischen Aufrufen erhalten)
  const float alpha = 0.3;           // Glättungsfaktor (0.1 = sehr weich, 0.5 = schneller)
  
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

int averageTofBack() {
  static int filteredDistance = 0;   // letzter geglätteter Wert (bleibt zwischen Aufrufen erhalten)
  const float alpha = 0.3;           // Glättungsfaktor (0.1 = sehr weich, 0.5 = schneller)
  
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
