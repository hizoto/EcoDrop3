#include <Arduino.h>
#include "dc_motor.h"
#include "ultraschall.h"

const bool isMecanumWheel = true;
const int speedFast = 255;
const int speedSlow = 127;
const int toleranceToWallmm = 15;
const int toleranceWheelsmm = 1;
const int containerDepth = 20;

unsigned long timeToTurn360MecanumMilliseconds = 5700;       
unsigned long timeToMove1000mmSidewaysMilliseconds = 10000;   //TODO Ausmessen wie lange eine Umdrehung dauert
unsigned long timeToTurn3602WheelMilliseconds = 5700;        //TODO Ausmessen wie lange eine Umdrehung dauert
unsigned long timeToMove1000mmMecanum = 5720;
unsigned long timeToMove1000mm2Wheel = 5720;                  //TODO


float distancePerSecond2Wheel = 1000 / (timeToMove1000mm2Wheel / 1000.0);                         //TODO Ausmessen welche Strecke in mm gefahren wird in einer Sekunde
float distancePerSecondMecanum = 1000 / (timeToMove1000mmMecanum / 1000.0);                       //TODO Ausmessen welche Strecke in mm gefahren wird in einer Sekunde


const int incrementDistance = 1;
const int inkrementGrad = 1;
const int minMoveTimeMs = 20;

// testest

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
const int B1_IN1 = 6;
const int B1_IN2 = 7;
const int B1_ENA = 2;

// Motor 2
const int B1_IN3 = 8;
const int B1_IN4 = 9;
const int B1_ENB = 3;

// Motor 3
const int B2_IN1 = 10;
const int B2_IN2 = 11;
const int B2_ENA = 4;

// Motor 4
const int B2_IN3 = 12;
const int B2_IN4 = 13;
const int B2_ENB = 5;

// Ultraschallsensor 1 Vorn
const int U1_trig = 23;
const int U1_echo = 25;

// Ultraschallsensor 2 Hinten
const int U2_trig = 22;
const int U2_echo = 24;

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
void moveToRightWall(int distanceToWall);
void moveToRightWallMecanum(int distanceToWall);
void moveToRightWall2Wheel(int distanceToWall);
void goParallel();


DC_Motor M1(B1_IN1, B1_IN2, B1_ENA); //Motor rechts
DC_Motor M2(B1_IN3, B1_IN4, B1_ENB); //Motor links
DC_Motor M3(B2_IN1, B2_IN2, B2_ENA);
DC_Motor M4(B2_IN3, B2_IN4, B2_ENB);
ultraschallsensor U1(U1_echo, U1_trig);
ultraschallsensor U2(U2_echo, U2_trig);

void setup() {
        // TODO
  delay(2000);
}



void loop() {
  // TODO
  schlange();
}

void schlange(){
  // hinweg
  turnLeft(90);
  moveForward(200);
  turnRight(90);
  moveForward(200);
  turnRight(90);
  moveForward(200);
  turnLeft(90);
  turnLeft(180);

  // rückweg
  
  turnLeft(90);
  moveForward(200);
  turnRight(90);
  moveForward(200);
  turnRight(90);
  moveForward(200);
  turnLeft(90);
  turnRight(180);
}

// Fährt vorwärts, Distanz in mm
void moveForward(int distancemm){
    if(isMecanumWheel){
        moveForwardMecanum(distancemm);
    }
    else{
        moveForward2Wheel(distancemm);
    }
}

void moveForward2Wheel(int distancemm){
  unsigned long timeToDrive = (distancemm / distancePerSecond2Wheel) * 1000;
  M1.forward(speedFast);
  M2.forward(speedFast);
  if (timeToDrive < minMoveTimeMs){
    delay(minMoveTimeMs - timeToDrive);
  }
  delay(timeToDrive);
  M1.brake();
  M2.brake();
}

void moveForwardMecanum(int distancemm){ 
  unsigned long timeToDrive = (distancemm / distancePerSecondMecanum) * 1000;
  M1.forward(speedFast);
  M2.forward(speedFast);
  M3.forward(speedFast);
  M4.forward(speedFast);
  if (timeToDrive < minMoveTimeMs){
    delay(minMoveTimeMs - timeToDrive);
  }
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

void turnLeftMecanum(int deg){
  unsigned long timeToTurn = (deg / 360.0) * timeToTurn360MecanumMilliseconds;
  M1.backward(speedFast);
  M3.backward(speedFast);
  M2.forward(speedFast);
  M4.forward(speedFast);
  if (timeToTurn < minMoveTimeMs){
    delay(minMoveTimeMs - timeToTurn);
  }
  delay(timeToTurn);
  M1.brake();
  M2.brake();
  M3.brake();
  M4.brake();
}

void turnLeft2Wheel(int deg){
  unsigned long timeToTurn = (deg / 360.0) * timeToTurn3602WheelMilliseconds;
  M1.forward(speedSlow);
  M2.backward(speedSlow);
  if (timeToTurn < minMoveTimeMs){
    delay(minMoveTimeMs - timeToTurn);
  }
  delay(timeToTurn);
  M1.brake();
  M2.brake();
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
  unsigned long timeToTurn = (deg / 360.0) * timeToTurn3602WheelMilliseconds;
  M1.backward(speedSlow);
  M2.forward(speedSlow);
  if (timeToTurn < minMoveTimeMs){
    delay(minMoveTimeMs - timeToTurn);
  }
  delay(timeToTurn);
  M1.brake();
  M2.brake();
}

void turnRightMecanum(int deg){
  unsigned long timeToTurn = (deg / 360.0) * timeToTurn360MecanumMilliseconds;
  M2.backward(speedFast);
  M4.backward(speedFast);
  M1.forward(speedFast);
  M3.forward(speedFast);
  if (timeToTurn < minMoveTimeMs){
    delay(minMoveTimeMs - timeToTurn);
  }
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
  if (timeToMove < minMoveTimeMs){
    delay(minMoveTimeMs - timeToMove);
  }
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
  if (timeToMove < minMoveTimeMs){
    delay(minMoveTimeMs - timeToMove);
  }
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
  if (moveTime < minMoveTimeMs){
    delay(minMoveTimeMs - moveTime);
  }
  delay(moveTime);
  M1.brake();
  M2.brake();
  M3.brake();
  M4.brake();
}

void turnSlowRight2Wheel(int distancemm){ 
  unsigned long moveTime = (distancemm * 1000) / distancePerSecondMecanum; 
  M1.forward(speedFast);
  M2.forward(speedSlow);
  if (moveTime < minMoveTimeMs){
    delay(minMoveTimeMs - moveTime);
  }
  delay(moveTime);
  M1.brake();
  M2.brake();
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
  if (moveTime < minMoveTimeMs){
    delay(minMoveTimeMs - moveTime);
  }
  delay(moveTime);
  M1.brake();
  M2.brake();
  M3.brake();
  M4.brake();
}

void turnSlowLeft2Wheel(int distancemm){  
  unsigned long moveTime = (distancemm * 1000) / distancePerSecondMecanum; 
  M1.forward(speedSlow);
  M2.forward(speedFast);
  if (moveTime < minMoveTimeMs){
    delay(minMoveTimeMs - moveTime);
  }
  delay(moveTime);
  M1.brake();
  M2.brake();
}

// nach rechts bewegen bis in gewünschtem Abstand zur Wand
void moveToRightWall(int distanceToWall){   // nach rechts bewegen bis in gewünschtem Abstand zur Wand
  long distanceFront = U1.getDistance();
  long distanceBack = U2.getDistance();

  if (isMecanumWheel){
    moveToRightWallMecanum(distanceToWall);
  }

  else {
    moveToRightWall2Wheel(distanceToWall);
  }

}

// nach rechts bewegen bis in gewünschtem Abstand zur Wand
void moveToRightWallMecanum(int distanceToWall){  // nach rechts bewegen bis in gewünschtem Abstand zur Wand
  long distanceFront = U1.getDistance();
  long distanceBack = U2.getDistance();

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
}

// nach rechts bewegen bis in gewünschtem Abstand zur Wand
void moveToRightWall2Wheel(int distanceToWall){  
  long distanceToMove = U1.getDistance() - distanceToWall;
  if (distanceToMove > 0){
    turnRight(90);
    moveForward(distanceToMove);
    turnLeft(90);
  }
  else if (distanceToMove < 0){
    distanceToMove *= -1;
    turnLeft(90);
    moveForward(distanceToMove);
    turnRight(90);
  }
}

void goParallel(){
  long distanceFront = U1.getDistance();
  long distanceBack = U2.getDistance();

  if (distanceFront > distanceBack){
    while (distanceFront > distanceBack){
      turnRight(inkrementGrad);
      distanceFront = U1.getDistance();
      distanceBack = U2.getDistance();
    }
  }

  else if (distanceFront < distanceBack){
    while (distanceFront < distanceBack){
      turnLeft(inkrementGrad);
      distanceFront = U1.getDistance();
      distanceBack = U2.getDistance();
    }
  }
}

void moveForwardParallelUntilContainer(int distanceToWall){
  // abstandserfassung
  long distanceFront = U1.getDistance();
  long distanceBack = U2.getDistance();
  
  // INITIAL PARALLELIZATION
  goParallel();

  // Move to wall distance
  moveToRightWall(distanceToWall);

  goParallel();
  distanceFront = U1.getDistance();
  distanceBack = U2.getDistance();

  // Main Logic of the function
  while (distanceBack - distanceFront < containerDepth){
    if (distanceFront > distanceBack){
      if(distanceFront - distanceBack < toleranceWheelsmm && distanceBack - distanceFront < toleranceWheelsmm){
        moveForward(incrementDistance);
        distanceFront = U1.getDistance();
        distanceBack = U2.getDistance();
      }
    }
    else if (distanceBack > distanceFront){
      if (distanceBack - distanceFront > toleranceWheelsmm){
        turnSlowLeft(incrementDistance);
        distanceFront = U1.getDistance();
        distanceBack = U2.getDistance();
      }
    }

    else {
      turnSlowRight(incrementDistance);
      distanceFront = U1.getDistance();
      distanceBack = U2.getDistance();
    }
  }
}


