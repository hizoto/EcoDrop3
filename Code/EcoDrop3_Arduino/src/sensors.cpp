#include "sensors.h"
#include "communication.h"   // logMessage()

// ------------------- MUX + Channels -------------------
TCA9548A I2CMUX;

static constexpr uint8_t CH_FRONT_RIGHT = 5;
static constexpr uint8_t CH_BACK_RIGHT  = 0;
static constexpr uint8_t CH_FRONT_LEFT  = 1;
static constexpr uint8_t CH_BACK_LEFT   = 2;

// Offsets
static constexpr int16_t OFF_FRONT_RIGHT = -46;
static constexpr int16_t OFF_BACK_RIGHT  = -13;
static constexpr int16_t OFF_FRONT_LEFT  = 0;
static constexpr int16_t OFF_BACK_LEFT   = 0;

// Sensor-Instanzen
static TofMuxSensor tofFrontRight(I2CMUX, CH_FRONT_RIGHT, OFF_FRONT_RIGHT);
static TofMuxSensor tofBackRight (I2CMUX, CH_BACK_RIGHT,  OFF_BACK_RIGHT);
static TofMuxSensor tofFrontLeft (I2CMUX, CH_FRONT_LEFT,  OFF_FRONT_LEFT);
static TofMuxSensor tofBackLeft  (I2CMUX, CH_BACK_LEFT,   OFF_BACK_LEFT);

// ------------------- Klasse: Implementation -------------------
TofMuxSensor::TofMuxSensor(TCA9548A& mux, uint8_t channel, int16_t offsetMm)
: _mux(mux), _channel(channel), _offset(offsetMm), _filtered(0), _lastRead(0) {}

bool TofMuxSensor::begin() {
  _mux.openChannel(_channel);
  bool ok = _lox.begin();
  _mux.closeAll();
  return ok;
}

void TofMuxSensor::setOffset(int16_t offsetMm) { _offset = offsetMm; }
int16_t TofMuxSensor::getOffset() const { return _offset; }

bool TofMuxSensor::readMeasurement(VL53L0X_RangingMeasurementData_t& out) {
  _mux.openChannel(_channel);
  _lox.rangingTest(&out, false);
  _mux.closeAll();

  // RangeStatus==4: ungültig/out of range
  return (out.RangeStatus != 4);
}

int TofMuxSensor::readRaw() {
  VL53L0X_RangingMeasurementData_t m;
  if (!readMeasurement(m)) return _filtered; // fallback
  return (int)m.RangeMilliMeter + _offset;
}

int TofMuxSensor::readFiltered(float alpha, uint32_t resetAfterMs) {
  if (millis() - _lastRead > resetAfterMs) _filtered = 0;

  VL53L0X_RangingMeasurementData_t m;
  if (!readMeasurement(m)) return _filtered; // ungültig -> alten Wert

  int newVal = (int)m.RangeMilliMeter + _offset;

  if (_filtered == 0) _filtered = newVal;
  _filtered = (int)(alpha * newVal + (1.0f - alpha) * _filtered);

  _lastRead = millis();
  return _filtered;
}

// ------------------- init -------------------
void initMux() {
  Serial.println("mux start");
  I2CMUX.begin();
  delay(20);
  Serial.println("mux begin gut");
  I2CMUX.closeAll();
  Serial.println("mux start gut");
}

void initSensors() {
  Wire.begin();
  initMux();

  if (!tofBackRight.begin())  logMessage("Failed to boot Back Right TOF");
  if (!tofFrontRight.begin()) logMessage("Failed to boot Front Right TOF");
  if (!tofBackLeft.begin())   logMessage("Failed to boot Back Left TOF");
  if (!tofFrontLeft.begin())  logMessage("Failed to boot Front Left TOF");

  delay(100);
}

void i2cScan() {
  Serial.println("scan...");
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    byte err = Wire.endTransmission();
    if (err == 0) {
      Serial.print("found 0x");
      Serial.println(addr, HEX);
    }
  }
  Serial.println("done");
}

TofMuxSensor& tofFR() { return tofFrontRight; }
TofMuxSensor& tofBR() { return tofBackRight; }
TofMuxSensor& tofFL() { return tofFrontLeft; }
TofMuxSensor& tofBL() { return tofBackLeft; }
