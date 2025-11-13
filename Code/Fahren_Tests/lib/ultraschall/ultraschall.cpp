#include <Arduino.h>
#include "ultraschall.h"

ultraschallsensor::ultraschallsensor(int _echo, int _trig){
    echo = _echo;
    trig = _trig;  
    pinMode(echo, INPUT);
    pinMode(trig, OUTPUT);
}

// Distanz in mm
unsigned long ultraschallsensor::getDistance() {
    unsigned long duration;
    unsigned long distancemm;

    // Trigger-Puls
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    // Echo-Zeit messen (µs)
    duration = pulseIn(echo, HIGH);

    // Umrechnen: 1 µs ≈ 0.343 mm Hin- und Rückweg, daher /2
    distancemm = (duration * 343) / 2000;  // = duration * 0.343 / 2

    return distancemm;
}
// Git Tests....