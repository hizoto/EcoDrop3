#include <Arduino.h>
#include "ultraschall.h"

ultraschallsensor::ultraschallsensor(int _echo, int _trig){
    echo = _echo;
    trig = _trig;  
    pinMode(echo, INPUT);
    pinMode(trig, OUTPUT);
}

unsigned long ultraschallsensor::getDistance(){
    unsigned long distancemm = 5555;
    return distancemm;
}