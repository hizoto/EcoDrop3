#include <Arduino.h>
#include "dc_motor.h"
#include "ultraschall.h"

const bool isMecanumWheel = false;
const int speedFast = 255;
const int speedSlow = 127;
const int toleranceToWallmm = 15;
const int toleranceWheelsmm = 1;
const int containerDepth = 20;

int runtimeForward = 1000;
int braketime = 1000;
unsigned long timeToTurn360MecanumMilliseconds = 25000;       //TODO Ausmessen wie lange eine Umdrehung dauert 
unsigned long timeToMove1000mmSidewaysMilliseconds = 25000;   //TODO Ausmessen wie lange eine Umdrehung dauert
unsigned long timeToTurn3602WheelMilliseconds = 25000;        //TODO Ausmessen wie lange eine Umdrehung dauert
unsigned long distancePerSecond2Wheel = 15;                   //TODO Ausmessen welche Strecke in mm gefahren wird in einer Sekunde
unsigned long distancePerSecondMecanum = 15;



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


/*  Definition der Motoren Mecanum Räder:

  [M1]------[M2] 


  [M3]------[M4]
*/


// Motor 1
const int B1_IN1 = 5;
const int B1_IN2 = 4;
const int B1_ENA = 6;

// Motor 2
const int B1_IN3 = 8;
const int B1_IN4 = 7;
const int B1_ENB = 9;

// Motor 3
const int B2_IN1 = 10;
const int B2_IN2 = 11;
const int B2_ENA = 12;

// Motor 4
const int B2_IN3 = 13;
const int B2_IN4 = 14;
const int B2_ENB = 15;

// Ultraschallsensor 1
const int U1_trig = 16;
const int U1_echo = 17;

// Ultraschallsensor 2
const int U2_trig = 18;
const int U2_echo = 19;

// Funktionsdefinitionen
void schlange();
void moveForward(int distancemm);
void moveForwardMecanum(int distancemm);
void moveForward2Wheel(int distancemm);
void turnLeft(int deg);
void turnLeftMecanum(int deg);
void turnLeft2Wheel(int deg);
void turnRight(int deg);
void turnRightMecanum(int deg);
void turnRight2Wheel(int deg);
void moveLeft(int distancemm);
void moveRight(int distancemm);
void moveForwardParallelUntilContainer(int distanceToWall);
void turnSlowRight(int distancemm);
void turnSlowRightMecanum(int distancemm);
void turnSlowRight2Wheel(int distancemm);
void turnSlowLeft(int distancemm);
void turnSlowLeftMecanum(int distancemm);
void turnSlowLeft2Wheel(int distancemm);


/*  Definition der Motoren Mecanum Räder:

  [M1]------[M2] 


  [M3]------[M4]
*/

DC_Motor M1(B1_IN1, B1_IN2, B1_ENA); //Motor rechts
DC_Motor M2(B1_IN3, B1_IN4, B1_ENB); //Motor links
DC_Motor M3(B2_IN1, B2_IN2, B2_ENA);
DC_Motor M4(B2_IN3, B2_IN4, B2_ENB);
ultraschallsensor U1(U1_echo, U1_trig);
ultraschallsensor U2(U2_echo, U2_trig);

void setup() {
        
}



void loop() {
  // put your main code here, to run repeatedly:
  schlange();
}

void schlange(){
  turnLeft(90);
  moveForward(50);
  turnRight(90);
  moveForward(50);
  turnRight(90);
  moveForward(50);
  turnLeft(90);
}

void moveForward(int distancemm){
    if(isMecanumWheel){
        moveForwardMecanum(distancemm);
    }
    else{
        moveForward2Wheel(distancemm);
    }
}

void moveForward2Wheel(int distancemm){
  unsigned long timeToDrive = (distancemm / distancePerSecond2Wheel);
  M1.forward(speedFast);
  M2.forward(speedFast);
  delay(timeToDrive);
  M1.brake();
  M2.brake();
  delay(braketime);
}

void moveForwardMecanum(int distancemm){ 
  unsigned long timeToDrive = (distancemm / distancePerSecondMecanum);
  M1.forward(speedFast);
  M2.forward(speedFast);
  M3.forward(speedFast);
  M4.forward(speedFast);
  delay(timeToDrive);
  M1.brake();
  M2.brake();
  M3.brake();
  M4.brake();
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
  unsigned long timeToTurn2Wheel = (deg / 360.0) * timeToTurn3602WheelMilliseconds;
  M1.forward(speedSlow);
  M2.backward(speedSlow);
  delay(timeToTurn2Wheel);
  M1.brake();
  M2.brake();
  delay(braketime);
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
  unsigned long timeToTurn2Wheel = (deg / 360.0) * timeToTurn3602WheelMilliseconds;
  M1.backward(speedSlow);
  M2.forward(speedSlow);
  delay(timeToTurn2Wheel);
  M1.brake();
  M2.brake();
  delay(braketime);
}

void turnRightMecanum(int deg){
  unsigned long timeToTurn = (deg / 360.0) * timeToTurn360MecanumMilliseconds;
  M2.backward(speedFast);
  M4.backward(speedFast);
  M1.forward(speedFast);
  M3.forward(speedFast);
  delay(timeToTurn);
  M1.brake();
  M2.brake();
  M3.brake();
  M4.brake();
}

void turnLeftMecanum(int deg){
  unsigned long timeToTurn = (deg / 360.0) * timeToTurn360MecanumMilliseconds;
  M1.backward(speedFast);
  M3.backward(speedFast);
  M2.forward(speedFast);
  M4.forward(speedFast);
  delay(timeToTurn);
  M1.brake();
  M2.brake();
  M3.brake();
  M4.brake();
}


void moveLeft(int distancemm){
  if (!isMecanumWheel){
    return;
  }
  unsigned long timeToMove = (distancemm / 1000.0) * timeToMove1000mmSidewaysMilliseconds;
  M1.backward(speedFast);
  M2.forward(speedFast);
  M3.forward(speedFast);
  M4.backward(speedFast);
  delay(timeToMove);
  M1.brake();
  M2.brake();
  M3.brake();
  M4.brake();
}


void moveRight(int distancemm){
  if (!isMecanumWheel){
    return;
  }
  unsigned long timeToMove = (distancemm / 1000.0) * timeToMove1000mmSidewaysMilliseconds;
  M1.forward(speedFast);
  M2.backward(speedFast);
  M3.backward(speedFast);
  M4.forward(speedFast);
  delay(timeToMove);
  M1.brake();
  M2.brake();
  M3.brake();
  M4.brake();
}


void turnSlowRight(int distancemm){
  if (isMecanumWheel){
    turnSlowRightMecanum(distancemm);
  }
  else {
    turnSlowRight2Wheel(distancemm);
  }
}

void turnSlowRightMecanum(int distancemm){  
  unsigned long moveTime = (distancemm * 1000) / distancePerSecondMecanum; 
  M1.forward(speedFast);
  M2.forward(speedSlow);
  M3.forward(speedFast);
  M4.forward(speedSlow);
  delay(moveTime);
  M1.brake();
  M2.brake();
  M3.brake();
  M4.brake();
}

void turnSlowRight2Wheel(int distancemm){ //TODO

}

void turnSlowLeft(int distancemm){ 
  if (isMecanumWheel){
    turnSlowLeftMecanum(distancemm);
  }
  else {
    turnSlowLeft2Wheel(distancemm);
  }

}

void turnSlowLeftMecanum(int distancemm){ 
  unsigned long moveTime = (distancemm * 1000) / distancePerSecondMecanum; 
  M1.forward(speedSlow);
  M2.forward(speedFast);
  M3.forward(speedSlow);
  M4.forward(speedFast);
  delay(moveTime);
  M1.brake();
  M2.brake();
  M3.brake();
  M4.brake();
}

void turnSlowLeft2Wheel(int distancemm){  //TODO

}


void moveForwardParallelUntilContainer(int distanceToWall){
  // abstandserfassung
  unsigned long distanceFront = U1.getDistance();
  unsigned long distanceBack = U2.getDistance();
  // INITIAL PARALLELIZATION
  if (distanceFront > distanceBack){
    while (distanceFront > distanceBack){
      turnRight(1);
      distanceFront = U1.getDistance();
      distanceBack = U2.getDistance();
    }
  }
  else{
    while (distanceFront < distanceBack){
      turnLeft(1);
      distanceFront = U1.getDistance();
      distanceBack = U2.getDistance();
    }
  }
  // Move to wall distance
  if (distanceFront > distanceToWall){
    while (distanceFront > distanceToWall){
      moveRight(1);
      distanceFront = U1.getDistance();
      distanceBack = U2.getDistance();
    }
  }

  else if (distanceFront < distanceToWall){
    while (distanceFront < distanceToWall){
      moveLeft(1);
      distanceFront = U1.getDistance();
      distanceBack = U2.getDistance();
    }
  }
  // Main Logic of the function
  while (distanceBack - distanceFront < containerDepth){
    if(distanceFront - distanceBack < toleranceWheelsmm && distanceBack - distanceFront < toleranceWheelsmm){
      moveForward(1);
      distanceFront = U1.getDistance();
      distanceBack = U2.getDistance();
    }
    else if (distanceFront - distanceBack > toleranceWheelsmm){
      turnSlowLeft(1);
      distanceFront = U1.getDistance();
      distanceBack = U2.getDistance();
    }

    else {
      turnSlowRight(1);
      distanceFront = U1.getDistance();
      distanceBack = U2.getDistance();
    }
  }
}



