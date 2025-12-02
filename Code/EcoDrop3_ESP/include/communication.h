#pragma once
#include <HardwareSerial.h>

void startComm();
void getComm();
extern HardwareSerial& ArduinoSlave;
void sendEmergencyStop();
void sendStartSignal();
void sendStopSignal();
void saveSensorDataToWebinterface();