#include <Arduino.h>

const bool isMecanumWheel = false;
const int speedFast = 255;
const int speedSlow = 127;


int runtimeForward = 1000;
int runtimeLeft = 500;
int runtimeRight = 500;
int braketime = 1000;


/* Pindefinitionen Motorentreiber:

        9 - 24V 	Power Supply, +
        PGND 	Power Supply, GND
        OUT1 	Motor1_+
        OUT2 	Motor1_-
        OUT3 	Motor2_+
        OUT4 	Motor2_-
        ENA 	Motor1 PWM
        IN1 	Motor1 control signal
        IN2 	Motor1 control signal
        ENB 	Motor2 PWM
        IN3 	Motor2 control signal
        IN4 	Motor2 control signal
        +5V 	Voltage Reference Input, +5V OR 3.3V

*/


// Motor 1
const int B1_IN1=5;
const int B1_IN2=4;
const int B1_ENA=6;

// Motor 2
const int B1_IN3=8;
const int B1_IN4=7;
const int B1_ENB=9;

// Motor 3
const int B2_IN1=10;
const int B2_IN2=11;
const int B2_ENA=12;

// Motor 4
const int B2_IN3=13;
const int B2_IN4=14;
const int B2_ENB=15;

class DC_Motor;
class ultraschallsensor;


// Funktionsdefinitionen
void schlange();
void driveForward(int distancemm);
void driveForwardMecanum(int distancemm);
void driveForward2Wheel(int distancemm);
void turnLeft(int deg);
void turnLeftMecanum(int deg);
void turnLeft2Wheel(int deg);
void turnRight(int deg);
void turnRightMecanum(int deg);
void turnRight2Wheel(int deg);

DC_Motor M1(B1_IN1, B1_IN2, B1_ENA); //Motor rechts
DC_Motor M2(B1_IN3, B1_IN4, B1_ENB); //Motor links
DC_Motor M3(B2_IN1, B2_IN2, B2_ENA);
DC_Motor M4(B2_IN3, B2_IN4, B2_ENB);


void setup() {
        
}



void loop() {
  // put your main code here, to run repeatedly:
  schlange();
}

void schlange(){
  turnLeft(90);
  driveForward(50);
  turnRight(90);
  driveForward(50);
  turnRight(90);
  driveForward(50);
  turnLeft(90);
}

void driveForward(int distancemm){
    if(isMecanumWheel){
        driveForwardMecanum(distancemm);
    }
    else{
        driveForward2Wheel(distancemm);
    }
}

void driveForward2Wheel(int distancemm){
  M1.forward(speedFast);
  M2.forward(speedFast);
  delay(runtimeForward);
  M1.brake();
  M2.brake();
  delay(braketime);
}

void driveForwardMecanum(int distancemm){

}

void turnLeft(int deg){
    if(isMecanumWheel){
        turnLeftMecanum(deg);
    }
    else{
        turnLeft2Wheel(deg);
    }
}

void turnLeft2Wheel(int deg){
  M1.forward(speedSlow);
  M2.backward(speedSlow);
  delay(runtimeLeft);
  M1.brake();
  M2.brake();
  delay(braketime);
}

void turnLeftMecanum(int deg){

}

void turnRight(int deg){
    if(isMecanumWheel){
        turnRightMecanum(deg);
    }
    else{
        turnRight2Wheel(deg);
    }
}

void turnRight2Wheel(int deg){
  M1.backward(speedSlow);
  M2.forward(speedSlow);
  delay(runtimeRight);
  M1.brake();
  M2.brake();
  delay(braketime);
}

void turnRightMecanum(int deg){

}

class DC_Motor{
  private:
    int IN1;
    int IN2;
    int ENA;
  public:
    DC_Motor(int _IN1, int _IN2, int _ENA){
      IN1 = _IN1;
      IN2 = _IN2;
      ENA = _ENA;
      pinMode(IN1, OUTPUT);
      pinMode(IN2, OUTPUT);
      pinMode(ENA, OUTPUT);
    }

    void brake(){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
    }

    void forward(int speed){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);

    }

    void backward(int speed){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENA, speed);
    }
};



class ultraschallsensor{
  private:
    int echo;
    int trig;

  public:
    ultraschallsensor(int _echo, int _trig){
      echo = _echo;
      trig = _trig;  
    }

    unsigned long getDistance(){

    }
};