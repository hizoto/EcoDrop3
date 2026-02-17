#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <TCA9548A-SOLDERED.h>

// ------------------- Klasse -------------------
class TofMuxSensor
{
public:
  // Konstruktor mit linearem Kalibriermodell
  TofMuxSensor(TCA9548A& mux,
               uint8_t channel,
               float scale = 1.0f,
               float bias  = 0.0f);

  bool begin();

  // Kalibrierung setzen / lesen
  void setCalibration(float scale, float bias);
  float getScale() const;
  float getBias() const;

  // Messfunktionen
  int readRaw();   // kalibrierter Wert in mm
  int readFiltered(float alpha = 0.3f,
                   uint32_t resetAfterMs = 500);   // EMA

private:
  bool readMeasurement(VL53L0X_RangingMeasurementData_t& out);
  int applyCalibration(int raw);

  TCA9548A& _mux;
  uint8_t _channel;

  float _scale;
  float _bias;

  int _lastRaw;
  int _filtered;
  uint32_t _lastRead;

  Adafruit_VL53L0X _lox;
};

void initSensors();
void initMux();

void logTofs(bool vl, bool vr, bool hl, bool hr);

// optional
void i2cScan();

TofMuxSensor& tofFR();
TofMuxSensor& tofBR();
TofMuxSensor& tofFL();
TofMuxSensor& tofBL();
