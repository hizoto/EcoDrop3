#include "sensors.h"
#include "communication.h"   // logMessage()

// ------------------- MUX + Channels -------------------
TCA9548A I2CMUX;

static constexpr uint8_t CH_FRONT_RIGHT = 0;
static constexpr uint8_t CH_BACK_RIGHT  = 5;
static constexpr uint8_t CH_FRONT_LEFT  = 1;
static constexpr uint8_t CH_BACK_LEFT   = 4;

// Offsets
static constexpr int16_t OFF_FRONT_RIGHT = 0;
static constexpr int16_t OFF_BACK_RIGHT  = 0;
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
  _lox.rangingTest(&out, false);

  // RangeStatus==4: ungültig/out of range
  return (out.RangeStatus != 4);
}

int TofMuxSensor::readRaw() {
  VL53L0X_RangingMeasurementData_t m;
  _mux.openChannel(_channel);
  readMeasurement(m);
  _mux.closeAll();
  return (int)m.RangeMilliMeter + _offset;
}

int TofMuxSensor::readFiltered(float alpha, uint32_t resetAfterMs) {
  _mux.openChannel(_channel);
  if (millis() - _lastRead > resetAfterMs) _filtered = 0;

  VL53L0X_RangingMeasurementData_t m;
  readMeasurement(m);

  int newVal = (int)m.RangeMilliMeter + _offset;

  if (_filtered == 0) _filtered = newVal;
  _filtered = (int)(alpha * newVal + (1.0f - alpha) * _filtered);

  _lastRead = millis();
  _mux.closeAll();
  return _filtered;
}

// ------------------- init -------------------
void initMux() {
  logMessage("mux start");
  I2CMUX.begin();
  delay(20);
  logMessage("mux begin gut");
  I2CMUX.closeAll();
  logMessage("mux close gut");
}

void initSensors() {
  Wire.begin();
  initMux();
  logMessage("ready to init TOFs");
  if (!tofBackRight.begin())  logMessage("Failed to boot Back Right TOF");
  else logMessage("TOF BR initiated");
  if (!tofFrontRight.begin()) logMessage("Failed to boot Front Right TOF");
  else logMessage("TOF FR initiated");
  if (!tofBackLeft.begin())   logMessage("Failed to boot Back Left TOF");
  else logMessage("TOF BL initiated");
  if (!tofFrontLeft.begin())  logMessage("Failed to boot Front Left TOF");
  else logMessage("TOF FL initiated");

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

void setOffsetsRight() {
  int br = tofBackRight.readRaw();
  int fr = tofFrontRight.readRaw();
  if (br < 0 || fr < 0) return;

  int soll = (br + fr) / 2;
  tofBackRight.setOffset(soll - br);
  tofFrontRight.setOffset(soll - fr);
}


void setOffsetsLeft(){
  int bl = tofBackLeft.readRaw();
  int fl = tofFrontLeft.readRaw();
  if (bl < 0 || fl < 0) return;

  int soll = (bl + fl) / 2;
  tofBackLeft.setOffset(soll - bl);
  tofFrontLeft.setOffset(soll - fl);
}

void logTofs(bool vl, bool vr, bool hl, bool hr){
  static unsigned long lastTofLog = 0;

  if (millis() - lastTofLog > 2000){
    String abstandFL = "TOF FL: " + String(tofFL().readRaw());
    String abstandFR = "TOF FR: " + String(tofFR().readRaw());
    String abstandBL = "TOF BL: " + String(tofBL().readRaw());
    String abstandBR = "TOF BR: " + String(tofBR().readRaw());
    if (vl) logMessage(abstandFL.c_str());
    if (vr) logMessage(abstandFR.c_str());
    if (hl) logMessage(abstandBL.c_str());
    if (hr) logMessage(abstandBR.c_str());
    lastTofLog = millis();
  }
}