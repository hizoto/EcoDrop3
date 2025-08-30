#include <Arduino.h>
#include 'bewegung.h'

DC_Motor::DC_Motor(int _IN1, int _IN2, int _ENA){
    IN1 = _IN1;
    IN2 = _IN2;
    ENA = _ENA;
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);
}

void DC_Motor::brake(){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
}

void DC_Motor::forward(int speed){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speed);

}

void DC_Motor::backward(int speed){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, speed);
}


DC_Motor::DC_Motor(int _IN1, int _IN2, int _ENA){
    IN1 = _IN1;
    IN2 = _IN2;
    ENA = _ENA;
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);
}

void DC_Motor::brake(){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
}

void DC_Motor::forward(int speed){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speed);

}

void DC_Motor::backward(int speed){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, speed);
}
