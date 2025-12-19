#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <TCA9548A-SOLDERED.h>

// ------------------- Klasse -------------------
class TofMuxSensor {
public:
  TofMuxSensor(TCA9548A& mux, uint8_t channel, int16_t offsetMm = 0);

  bool begin();
  void setOffset(int16_t offsetMm);
  int16_t getOffset() const;

  int readRaw();                                  // mm + offset
  int readFiltered(float alpha = 0.3f,
                   uint32_t resetAfterMs = 500);   // EMA + reset nach Pause

private:
  bool readMeasurement(VL53L0X_RangingMeasurementData_t& out);

  TCA9548A& _mux;
  uint8_t _channel;
  int16_t _offset;

  Adafruit_VL53L0X _lox;

  int _filtered;
  uint32_t _lastRead;
};

// ------------------- API -------------------
void initSensors();
void initMux();


// optional
void i2cScan();

TofMuxSensor& tofFR();
TofMuxSensor& tofBR();
TofMuxSensor& tofFL();
TofMuxSensor& tofBL();

