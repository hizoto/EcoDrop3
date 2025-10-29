#include <Arduino.h>
#include <Wire.h>
#include "sensors.h"
#include <vl53lx_class.h>


#define TCAADDR 0x70       // I2C-Adresse des TCA9548A
#define CHANNEL_SENSOR1 0  // Kanal 0 am Multiplexer
#define CHANNEL_SENSOR2 1  // Kanal 1 am Multiplexer

VL53LX sensor1(&Wire);
VL53LX sensor2(&Wire);

void tcaselect(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
  delay(10);  // Kurzes Delay für Stabilität
}

void tof_setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(100);

  Serial.println("VL53L3CX + TCA9548A Setup");

  // --- Sensor 1 ---
  tcaselect(CHANNEL_SENSOR1);
  if (sensor1.begin() != 0) {
    Serial.println("Sensor 1 Init Fehler!");
    while (1);
  }
  sensor1.InitSensor(0x29);          // Adresse (Multiplexer erlaubt gleiche Adresse)
  sensor1.VL53LX_StartMeasurement();
  Serial.println("Sensor 1 bereit (Kanal 0)");

  // --- Sensor 2 ---
  tcaselect(CHANNEL_SENSOR2);
  if (sensor2.begin() != 0) {
    Serial.println("Sensor 2 Init Fehler!");
    while (1);
  }
  sensor2.InitSensor(0x29);
  sensor2.VL53LX_StartMeasurement();
  Serial.println("Sensor 2 bereit (Kanal 1)");
}

void tof_loop() {
  uint8_t ready = 0;
  VL53LX_MultiRangingData_t data;

  // --- Sensor 1 ---
  tcaselect(CHANNEL_SENSOR1);
  do {
    sensor1.VL53LX_GetMeasurementDataReady(&ready);
  } while (!ready);
  sensor1.VL53LX_GetMultiRangingData(&data);
  Serial.print("S1: ");
  Serial.println(data.RangeData[0].RangeMilliMeter);

  delay(50);

  // --- Sensor 2 ---
  tcaselect(CHANNEL_SENSOR2);
  do {
    sensor2.VL53LX_GetMeasurementDataReady(&ready);
  } while (!ready);
  sensor2.VL53LX_GetMultiRangingData(&data);
  Serial.print("S2: ");
  Serial.println(data.RangeData[0].RangeMilliMeter);

  delay(200);
}