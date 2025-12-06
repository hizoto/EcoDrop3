#pragma once

void initSensors();
void initMux();
void initTofFront();
void initTofBack();
int readTofFront();
int readTofBack();
int readTofFrontUnfiltered();
int readTofBackUnfiltered();

