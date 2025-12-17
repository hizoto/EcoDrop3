#include <Arduino.h>
#include <Wire.h>
#include "sensors.h"
#include <Adafruit_VL53L0X.h>
#include <TCA9548A-SOLDERED.h>
#include "communication.h"
                             // multiplexer
Adafruit_VL53L0X loxFrontLeft = Adafruit_VL53L0X();   // TOF
Adafruit_VL53L0X loxBackLeft = Adafruit_VL53L0X();    // TOF
Adafruit_VL53L0X loxFrontRight = Adafruit_VL53L0X();   // TOF
Adafruit_VL53L0X loxBackRight = Adafruit_VL53L0X();    // TOF

TCA9548A I2CMUX; 

// Multiplexer Channels
int tofFrontRightChannel = 5;
int tofBackRightChannel = 0;
int tofFrontLeftChannel = 1;
int tofBackLeftChannel = 2;


int tofFrontRightOffset = -46; //-33;
int tofBackRightOffset = -13;
int tofFrontLeftOffset = 0;
int tofBackLeftOffset = 0;

void initSensors(){
  Wire.begin();
  initMux();
  initTofBackRight();
  initTofFrontRight();
  initTofBackLeft();
  initTofFrontLeft();
  delay(100);
}

void initMux(){
  Serial.println("mux start");
  I2CMUX.begin();
  delay(20);
  Serial.println("mux begin gut");
  I2CMUX.closeAll();
  Serial.println("mux start gut");
}

void initTofFrontRight(){
  I2CMUX.openChannel(tofFrontRightChannel);
  if(!loxFrontRight.begin()){
    logMessage("Failed to boot Front Right TOF");   
    I2CMUX.closeAll();
    return;
  }
  I2CMUX.closeAll();
}

void initTofBackRight(){
  I2CMUX.openChannel(tofBackRightChannel);
  if(!loxBackRight.begin()){
    logMessage("Failed to boot Back Right TOF");   
    I2CMUX.closeAll(); 
    return;
  }
  I2CMUX.closeAll();
}

void initTofBackLeft(){
  I2CMUX.openChannel(tofBackLeftChannel);
  if(!loxBackLeft.begin()){
    logMessage("Failed to boot Back Left TOF");   
    I2CMUX.closeAll(); 
    return;
  }
  I2CMUX.closeAll();
}

void initTofFrontLeft(){
  I2CMUX.openChannel(tofFrontLeftChannel);
  if(!loxFrontLeft.begin()){
    logMessage("Failed to boot Front Left TOF");   
    I2CMUX.closeAll(); 
    return;
  }
  I2CMUX.closeAll();
}

int readTofFrontRight() {
  const int offset = tofFrontRightOffset;
  static int filteredDistance = 0;                // letzter geglätteter Wert
  static unsigned long lastRead = 0;
  const float alpha = 0.3;                        // Glättungsfaktor (0.1 = sehr weich, 0.5 = schneller)
  
  if(millis() - lastRead > 500){
    filteredDistance = 0;
  }

  VL53L0X_RangingMeasurementData_t measure;

  I2CMUX.openChannel(tofFrontRightChannel);
  loxFrontRight.rangingTest(&measure, false);
  I2CMUX.closeAll();

  if (measure.RangeStatus == 4) {
    // Ungültige Messung, alten Wert zurückgeben
    return filteredDistance;
  }

  uint16_t newDistance = measure.RangeMilliMeter;

  // Wenn es der erste Durchlauf ist (noch kein Filterwert vorhanden)
  if (filteredDistance == 0) filteredDistance = newDistance + offset;

  // Gleitende Mittelung
  filteredDistance = (int)(alpha * (newDistance + offset) + (1.0 - alpha) * filteredDistance);

  lastRead = millis();

  return filteredDistance;
}

int readTofFrontLeft() {
  const int offset = tofFrontLeftOffset;
  static int filteredDistance = 0;                // letzter geglätteter Wert
  static unsigned long lastRead = 0;
  const float alpha = 0.3;                        // Glättungsfaktor (0.1 = sehr weich, 0.5 = schneller)
  
  if(millis() - lastRead > 500){
    filteredDistance = 0;
  }

  VL53L0X_RangingMeasurementData_t measure;

  I2CMUX.openChannel(tofFrontLeftChannel);
  loxFrontLeft.rangingTest(&measure, false);
  I2CMUX.closeAll();

  if (measure.RangeStatus == 4) {
    // Ungültige Messung, alten Wert zurückgeben
    return filteredDistance;
  }

  uint16_t newDistance = measure.RangeMilliMeter;

  // Wenn es der erste Durchlauf ist (noch kein Filterwert vorhanden)
  if (filteredDistance == 0) filteredDistance = newDistance + offset;

  // Gleitende Mittelung
  filteredDistance = (int)(alpha * (newDistance + offset) + (1.0 - alpha) * filteredDistance);

  lastRead = millis();

  return filteredDistance;
}

int readTofBackRight() {
  const int offset = tofBackRightOffset;
  static int filteredDistance = 0;   // letzter geglätteter Wert (bleibt zwischen Aufrufen erhalten)
  static unsigned long lastRead = 0;
  const float alpha = 0.3;           // Glättungsfaktor (0.1 = sehr weich, 0.5 = schneller)
  
  if(millis() - lastRead > 500){
    filteredDistance = 0;
  }

  VL53L0X_RangingMeasurementData_t measure;
  I2CMUX.openChannel(tofBackRightChannel);
  loxBackRight.rangingTest(&measure, false);
  I2CMUX.closeAll();

  if (measure.RangeStatus == 4) {
    // Ungültige Messung, alten Wert zurückgeben
    return filteredDistance;
  }

  uint16_t newDistance = measure.RangeMilliMeter;

  // Wenn es der erste Durchlauf ist (noch kein Filterwert vorhanden)
  if (filteredDistance == 0) filteredDistance = newDistance + offset;

  // Gleitende Mittelung
  filteredDistance = (int)(alpha * (newDistance + offset) + (1.0 - alpha) * filteredDistance);
  lastRead = millis();
  return filteredDistance;
}

int readTofBackLeft() {
  const int offset = tofBackRightOffset;
  static int filteredDistance = 0;   // letzter geglätteter Wert (bleibt zwischen Aufrufen erhalten)
  static unsigned long lastRead = 0;
  const float alpha = 0.3;           // Glättungsfaktor (0.1 = sehr weich, 0.5 = schneller)
  
  if(millis() - lastRead > 500){
    filteredDistance = 0;
  }

  VL53L0X_RangingMeasurementData_t measure;
  I2CMUX.openChannel(tofBackLeftChannel);
  loxBackLeft.rangingTest(&measure, false);
  I2CMUX.closeAll();

  if (measure.RangeStatus == 4) {
    // Ungültige Messung, alten Wert zurückgeben
    return filteredDistance;
  }

  uint16_t newDistance = measure.RangeMilliMeter;

  // Wenn es der erste Durchlauf ist (noch kein Filterwert vorhanden)
  if (filteredDistance == 0) filteredDistance = newDistance + offset;

  // Gleitende Mittelung
  filteredDistance = (int)(alpha * (newDistance + offset) + (1.0 - alpha) * filteredDistance);
  lastRead = millis();
  return filteredDistance;
}

int readTofFrontRightUnfiltered() {
  VL53L0X_RangingMeasurementData_t measure;

  I2CMUX.openChannel(tofFrontRightChannel);
  loxFrontRight.rangingTest(&measure, false);
  I2CMUX.closeAll();

  return measure.RangeMilliMeter + tofFrontRightOffset;
}


int readTofBackRightUnfiltered() {
  VL53L0X_RangingMeasurementData_t measure;

  I2CMUX.openChannel(tofBackRightChannel);
  loxBackRight.rangingTest(&measure, false);
  I2CMUX.closeAll();

  return measure.RangeMilliMeter + tofBackRightOffset;
}

int readTofBackLeftUnfiltered() {
  VL53L0X_RangingMeasurementData_t measure;

  I2CMUX.openChannel(tofBackRightChannel);
  loxBackLeft.rangingTest(&measure, false);
  I2CMUX.closeAll();

  return measure.RangeMilliMeter + tofBackRightOffset;
}

int readTofFrontLeftUnfiltered() {
  VL53L0X_RangingMeasurementData_t measure;

  I2CMUX.openChannel(tofBackRightChannel);
  loxFrontLeft.rangingTest(&measure, false);
  I2CMUX.closeAll();

  return measure.RangeMilliMeter + tofBackRightOffset;
}

void i2cScan(){
  Serial.println("scan...");
  for (byte addr=1; addr<127; addr++) {
    Wire.beginTransmission(addr);
    byte err = Wire.endTransmission();
    if (err == 0) {
      Serial.print("found 0x");
      Serial.println(addr, HEX);
    }
  }
  Serial.println("done");
}