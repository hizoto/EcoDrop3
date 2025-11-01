#include <Arduino.h>
#include <Wire.h>
#include "sensors.h"
#include <Adafruit_VL53L0X.h>
#include <TCA9548A-SOLDERED.h>
#include "communication.h"

TCA9548A I2CMUX;
Adafruit_VL53L0X loxFront = Adafruit_VL53L0X();
Adafruit_VL53L0X loxBack = Adafruit_VL53L0X();

int tofFrontChannel = 0;
int tofBackChannel = 1;

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
    Serial.println("Failed to boot Front TOF"); // TODO
    return;
  }
  //delay(10);
  I2CMUX.closeAll();
}

void initTofBack(){
  I2CMUX.openChannel(tofBackChannel);
  if(!loxBack.begin()){
    Serial.println("Failed to boot Back TOF"); // TODO
    return;
  }
  //delay(10);
  I2CMUX.closeAll();
}

uint16_t readTofFront(){
  I2CMUX.openChannel(tofFrontChannel);
  //delay(10);
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
  //delay(10);
  VL53L0X_RangingMeasurementData_t measure;
  loxBack.rangingTest(&measure, false);
  I2CMUX.closeAll();
  if (measure.RangeStatus != 4) {
    uint16_t distance = measure.RangeMilliMeter;
    return distance;
  }
}