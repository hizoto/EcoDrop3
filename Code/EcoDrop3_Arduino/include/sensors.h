#pragma once

void initSensors();
int readTofFrontRight();
int readTofBackRight();
int readTofFrontLeft();
int readTofBackLeft();
int readTofFrontRightUnfiltered();
int readTofBackRightUnfiltered();
int readTofFrontLeftUnfiltered();
int readTofBackLeftUnfiltered();
void initMux();
void initTofFrontRight();
void initTofBackRight();
void initTofFrontLeft();
void initTofBackLeft();
void i2cScan();