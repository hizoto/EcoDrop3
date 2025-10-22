#pragma once
#include <HardwareSerial.h>

void startComm();
void getComm(int& currentStep, int& lastFinishedStep);
extern HardwareSerial& ArduinoSlave;
void sendEmergencyStop();
void sendStartSignal();
void sendStopSignal();
void saveSensorDataToWebinterface();