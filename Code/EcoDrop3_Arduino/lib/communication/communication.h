#pragma once
#include <Arduino.h>

// Pointer zur Step-Variabel muss mitgegeben werden!
void getComm(int* stp);

void logMessage();

void startComm();

void handleCommand(const String& cmd, int* stp);

void sendStepFinished(int step);

bool updateStatus();

void emergencystop();
