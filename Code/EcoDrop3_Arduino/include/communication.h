#pragma once
#include <Arduino.h>

extern bool isRunning;

// Pointer zur Step-Variabel muss mitgegeben werden!
void getComm();

void logMessage(const char* msg);

void startComm();

void handleCommand(const String& cmd);

void sendStepFinished(int step);

bool updateStatus();

void emergencystop();

void logSuccess();
