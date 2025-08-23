#include <Arduino.h>
#include "ultraschall.h"

ultraschallsensor::ultraschallsensor(int _echo, int _trig){
    echo = _echo;
    trig = _trig;  
    pinMode(echo, INPUT);
    pinMode(trig, OUTPUT);
}

unsigned long ultraschallsensor::getDistance(){
    long distancemm, duration;
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    duration = pulseIn(echo, HIGH);
    distancemm = (duration / 2) / 291;

    return distancemm;
}