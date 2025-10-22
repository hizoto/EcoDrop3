#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include "sensors.h"


#include "Adafruit_VL53L0X.h"

#define SHUT_PIN 22


Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void tof_setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(SHUT_PIN, OUTPUT);
  

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  digitalWrite(SHUT_PIN, LOW);
  delay(500);
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin(0x30)) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 

  digitalWrite(SHUT_PIN, HIGH);
  delay(500);
}


void tof_loop() {
  VL53L0X_RangingMeasurementData_t measure1;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!

  if (measure1.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(measure1.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
  VL53L0X_RangingMeasurementData_t measure2;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!

  if (measure2.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(measure2.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
    
  delay(100);
  byte error, address;
  int nDevices = 0;

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Gerät gefunden bei Adresse 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println();
      nDevices++;
    }
  }

  if (nDevices == 0) {
    Serial.println("Keine I2C-Geräte gefunden");
  } else {
    Serial.println("Scan abgeschlossen");
  }

  delay(5000); // 5 Sekunden warten, bevor erneut gescannt wird
}