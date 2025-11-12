#include <Arduino.h>
#include "bewegung.h"

const int speedFast = 255;
const int speedSlow = 127;
// Toleranz Wandabstand
const int toleranceToWallmm = 15;
// Toleranz für parallelität
const int toleranceWheelsmm = 5;
const int containerDepth = 20;

unsigned long timeToTurn360Milliseconds = 5700;       
unsigned long timeToMove1000mmSidewaysMilliseconds = 10000;   //TODO Ausmessen wie lange eine Umdrehung dauert
unsigned long timeToMove1000mm = 5720;                //TODO

               //TODO Ausmessen welche Strecke in mm gefahren wird in einer Sekunde
float distancePerSecond = 1000 / (timeToMove1000mm / 1000.0);                       //TODO Ausmessen welche Strecke in mm gefahren wird in einer Sekunde


const int incrementDistance = 1;
const int incrementGrad = 1;
const int minMoveTimeMs = 5;


// Motor 1 Vorne links
const int B1_IN1 = 6;
const int B1_IN2 = 7;
const int B1_ENA = 2;

// Motor 2 Vorne rechts
const int B1_IN3 = 8;
const int B1_IN4 = 9;
const int B1_ENB = 3;

// Motor 3 Hinten links
const int B2_IN1 = 10; 
const int B2_IN2 = 11; 
const int B2_ENA = 4; 

// Motor 4 Hinten rechts
const int B2_IN3 = 30; 
const int B2_IN4 = 32; 
const int B2_ENB = 12; 

/*  Definition der Motoren Mecanum Räder:

  [M1]------[M2] 


  [M3]------[M4]

*/

DC_Motor M1(B1_IN1, B1_IN2, B1_ENA); 
DC_Motor M2(B1_IN3, B1_IN4, B1_ENB); 
DC_Motor M3(B2_IN1, B2_IN2, B2_ENA);
DC_Motor M4(B2_IN3, B2_IN4, B2_ENB);


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


void moveForward(int distancemm){ 
  unsigned long timeToDrive = (distancemm / distancePerSecond) * 1000;
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
  unsigned long timeToTurn = (deg / 360.0) * timeToTurn360Milliseconds;
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

void turnRight(int deg){
  unsigned long timeToTurn = (deg / 360.0) * timeToTurn360Milliseconds;
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
  unsigned long moveTime = (distancemm * 1000) / distancePerSecond; 
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

void turnSlowLeft(int distancemm){ 
  unsigned long moveTime = (distancemm * 1000) / distancePerSecond; 
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

// nach rechts bewegen bis in gewünschtem Abstand zur Wand
void moveToRightWall(int distanceToWall){  // nach rechts bewegen bis in gewünschtem Abstand zur Wand
  uint16_t distanceFront = readTofFront();

  if (distanceFront > distanceToWall){
    while (distanceFront > distanceToWall){
      moveRight(1);
      distanceFront = readTofFront();
    }
  }

  else if (distanceFront < distanceToWall){
    while (distanceFront < distanceToWall){
      moveLeft(1);
      distanceFront = readTofFront();
    }
  }
}

void goParallel(){
    logMessage("Parallelität zur Wand wird hergestellt...");
    uint16_t distanceFront = readTofFront();
    uint16_t distanceBack = readTofBack();

    while (abs((int)distanceFront - (int)distanceBack) > toleranceWheelsmm){
        if (distanceFront > distanceBack){
        turnRight(incrementGrad);
        }
        else {
        turnLeft(incrementGrad);
        }

        distanceFront = readTofFront();
        distanceBack = readTofBack();
    }
    logMessage("Success!");
}


void moveForwardParallelUntilContainer(int distanceToWall){
  // abstandserfassung
  uint16_t distanceFront = readTofFront();
  uint16_t distanceBack = readTofBack();
  
  // INITIAL PARALLELIZATION
  goParallel();

  // Move to wall distance
  moveToRightWall(distanceToWall);

  goParallel();
  distanceFront = readTofFront();
  distanceBack = readTofBack();

  // Main Logic of the function
  logMessage("Suche Containter...");
  while (distanceBack - distanceFront < containerDepth){
    if (distanceFront - distanceBack < toleranceWheelsmm || distanceBack - distanceFront < toleranceWheelsmm){
      moveForward(incrementDistance);
      distanceFront = readTofFront();
      distanceBack = readTofBack();
      
    }
    else if (distanceBack - distanceFront > toleranceWheelsmm){
      if (distanceBack - distanceFront > toleranceWheelsmm){
        turnSlowLeft(incrementDistance);
        distanceFront = readTofFront();
        distanceBack = readTofBack();
      }
    }

    else {
      turnSlowRight(incrementDistance);
      distanceFront = readTofFront();
      distanceBack = readTofBack();
    }
  }
  logMessage("Container gefunden!");
}