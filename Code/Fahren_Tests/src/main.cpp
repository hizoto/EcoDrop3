#include <Arduino.h>

const bool isMecanumWheel = false;
const int speedFast = 255;
const int speedSlow = 127;


int currentSpeed1 = 0;
int currentSpeed2 = 0;


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

void setup() {
  DC_Motor M1(B1_IN1, B1_IN2, B1_ENA);
  DC_Motor M2(B1_IN3, B1_IN4, B1_ENB);
  DC_Motor M3(B2_IN1, B2_IN2, B2_ENA);
  DC_Motor M4(B2_IN3, B2_IN4, B2_ENB);
}



void loop() {
  // put your main code here, to run repeatedly:
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