#include "ultraschall.h"

ultraschallsensor::ultraschallsensor(int _echo, int _trig){
    echo = _echo;
    trig = _trig;  
}

unsigned long ultraschallsensor::getDistance(){
    unsigned long distance = 5555;
    return distance;
}