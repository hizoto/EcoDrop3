#pragma once
#include <Arduino.h>

void current_measure_init();
float getCurrent(int channel);
float getVoltage(int channel);