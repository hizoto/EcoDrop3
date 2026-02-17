#include "sensors.h"
#include "communication.h"   // logMessage()

// ------------------------------------------------------
//                     TOF-Parameter
// ------------------------------------------------------


// kontinuierliche Messung
const bool TofIsContinous = false;
// Timingbudget erhöht die genauigkeit (auf kosten der Intervallzeit)
uint32_t timingBudgetms = 200;

// ------------------- MUX + Channels -------------------
TCA9548A I2CMUX;

static constexpr uint8_t CH_FRONT_LEFT  = 1;
static constexpr uint8_t CH_FRONT_RIGHT = 0;
static constexpr uint8_t CH_BACK_LEFT   = 4;
static constexpr uint8_t CH_BACK_RIGHT  = 5;


// ------------------- OFFSETS TOF ----------------------
static constexpr float FLscale = 0.0;
static constexpr float FLbias = 0.0;

static constexpr float FRscale = 0.0;
static constexpr float FRbias = 0.0;

static constexpr float BLscale = 0.0;
static constexpr float BLbias = 0.0;

static constexpr float BRscale = 0.0;
static constexpr float BRbias = 0.0;


// Sensor-Instanzen
static TofMuxSensor tofFrontLeft (I2CMUX, CH_FRONT_LEFT, FLscale, FLbias);
static TofMuxSensor tofFrontRight(I2CMUX, CH_FRONT_RIGHT, FRscale, FRbias);
static TofMuxSensor tofBackLeft  (I2CMUX, CH_BACK_LEFT, BLscale, BLbias);
static TofMuxSensor tofBackRight (I2CMUX, CH_BACK_RIGHT, BRscale, BRbias);

// ------------------- Klasse: Implementation -------------------
TofMuxSensor::TofMuxSensor(TCA9548A& mux,
                           uint8_t channel,
                           float scale,
                           float bias)
: _mux(mux),
  _channel(channel),
  _scale(scale),
  _bias(bias),
  _lastRaw(0),
  _filtered(0),
  _lastRead(0)
{}


bool TofMuxSensor::begin() {
  _mux.openChannel(_channel);
  bool ok = _lox.begin();
  //_lox.setMeasurementTimingBudgetMicroSeconds(timingBudgetms * 1000);
  if(TofIsContinous){
    _lox.startRangeContinuous(0); // backtoback Messungen
  }
  _mux.closeAll();
  return ok;
}

bool TofMuxSensor::readMeasurement(VL53L0X_RangingMeasurementData_t& out) {
  _lox.rangingTest(&out, false);

  // RangeStatus==4: ungültig/out of range
  return (out.RangeStatus != 4);
}

int TofMuxSensor::readRaw() {
  if (TofIsContinous){
    _mux.openChannel(_channel);
    delay(4);

    if (!_lox.isRangeComplete()) {
        _mux.closeAll();
        return _lastRaw;
    }

    VL53L0X_RangingMeasurementData_t m;
    _lox.getRangingMeasurement(&m);

    _mux.closeAll();

    if (m.RangeStatus != 0)
        return _lastRaw;
    int raw = (int)m.RangeMilliMeter;
    int newVal = applyCalibration(raw);


    if (newVal <= 0 || newVal > 2000)
        return _lastRaw;

    if (_lastRaw != 0 && abs(newVal - _lastRaw) > 200 && _lastRaw != 0)
        return _lastRaw;

    _lastRaw = newVal;
    return _lastRaw;
  }
  else {
    VL53L0X_RangingMeasurementData_t m;
    for (int i = 0; i < 3; i++)  // max 3 Versuche
    {
        _mux.openChannel(_channel);
        delay(4);
        if(!readMeasurement(m)){
          _mux.closeAll();
          continue;
        }
        _mux.closeAll();

        int raw = (int)m.RangeMilliMeter;
        int value = applyCalibration(raw);


        if (value > 0 && value < 2000)
        {
            _lastRaw = value;
            return _lastRaw;
        }
    }
    return _lastRaw;
  }
}



int TofMuxSensor::readFiltered(float alpha, uint32_t resetAfterMs) {
  if(TofIsContinous){
    _mux.openChannel(_channel);
    delay(4);

    if (millis() - _lastRead > resetAfterMs)
      _filtered = 0;

    if (_lox.isRangeComplete()) {

    VL53L0X_RangingMeasurementData_t m;
    _lox.getRangingMeasurement(&m);

    if (m.RangeStatus != 0) {
        _mux.closeAll();
        return _filtered;
    }

    int raw = (int)m.RangeMilliMeter;
    int newVal = applyCalibration(raw);


    if (newVal <= 0 || newVal > 2000){
        _mux.closeAll();
        return _filtered;
    }

    if (_filtered != 0 && abs(newVal - _filtered) > 200){
        _mux.closeAll();
        return _filtered;
    }

    if (_filtered == 0){
        _filtered = newVal;
    }

    _filtered = (int)(alpha * newVal + (1.0f - alpha) * _filtered);

    _lastRead = millis();
    }
  _mux.closeAll();
  return _filtered;
  }
  else {
    _mux.openChannel(_channel);
    delay(4);

    VL53L0X_RangingMeasurementData_t m;

    if (!readMeasurement(m)) {
        _mux.closeAll();
        return _filtered;
    }

    int raw = (int)m.RangeMilliMeter;
    int newVal = applyCalibration(raw);


    if (newVal <= 0 || newVal > 2000) {
        _mux.closeAll();
        return _filtered;
    }

    if (_filtered != 0 && abs(newVal - _filtered) > 300) {
        _mux.closeAll();
        return _filtered;
    }

    if (_filtered == 0)
        _filtered = newVal;

    _filtered = (int)(alpha * newVal + (1.0f - alpha) * _filtered);

    _lastRead = millis();
    _mux.closeAll();
    return _filtered;
  }
}

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

void TofMuxSensor::setCalibration(float scale, float bias)
{
    _scale = scale;
    _bias = bias;
}

float TofMuxSensor::getScale() const { return _scale; }
float TofMuxSensor::getBias() const { return _bias; }

int TofMuxSensor::applyCalibration(int raw)
{
    return (int)(_scale * raw + _bias);
}