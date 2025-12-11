#include <Arduino.h>
#include "bewegung.h"
#include "Pixy2Cam.h"
#include <Servo.h>

const int speedFast = 200; // max 255
const int speedSlow = 100;
// Toleranz Wandabstand
const int toleranceToWallmm = 15;
// Toleranz für parallelität
const int toleranceWheelsmm = 10;
const int containerDepth = 40;

unsigned long timeToTurn360Milliseconds = 5700;
unsigned long timeToMove1000mmSidewaysMilliseconds = 10000;   //TODO Ausmessen wie lange eine Umdrehung dauert
unsigned long timeToMove1000mm = 5720;              //TODO 
unsigned long bewegungsZeitLinear = 5000; // TODO in ms
int dockLength = 300; // TODO
int posKlappeOffen = 0; // TODO
int posKlappeGeschlossen = 90; // TODO
int zeitEntleeren = 2000;

float distancePerSecond = 1000 / (timeToMove1000mm / 1000.0);                      


const int incrementDistance = 1;
const float incrementGrad = 0.1;
const int minMoveTimeMs = 1;

// PINS
int endschalterHinten = 50; // TODO pseudopin
int endschalterUnten = 40;
int servoPin = 13;

// Motor 1 Vorne links
const int B1_IN1 = 46;
const int B1_IN2 = 47;
const int B1_ENA = 8;

// Motor 2 Vorne rechts
const int B1_IN3 = 52;
const int B1_IN4 = 53;
const int B1_ENB = 11;

// Motor 3 Hinten links
const int B2_IN1 = 48; 
const int B2_IN2 = 49; 
const int B2_ENA = 9;

// Motor 4 Hinten rechts
const int B2_IN3 = 50; 
const int B2_IN4 = 51; 
const int B2_ENB = 10; 

// Linearantrieb
const int B3_IN1 = 44; 
const int B3_IN2 = 45; 
const int B3_ENA = 12; 


/*  Definition der Motoren Mecanum Räder:

  [M1]------[M2] 


  [M3]------[M4]

*/

DC_Motor M1(B1_IN1, B1_IN2, B1_ENA); // vorne links
DC_Motor M2(B1_IN3, B1_IN4, B1_ENB); // vorne rechts
DC_Motor M3(B2_IN1, B2_IN2, B2_ENA); // hinten links
DC_Motor M4(B2_IN3, B2_IN4, B2_ENB); // hinten rechts
DC_Motor LinearAntrieb(B3_IN1, B3_IN2, B3_ENA);
Servo klappe;

DC_Motor::DC_Motor(int _IN1, int _IN2, int _ENA){ // TODO wird ausgeführt wenn deklariert??
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
}


void moveBackward(int distancemm){
  unsigned long timeToDrive = (distancemm / distancePerSecond) * 1000;
  M1.backward(speedFast);
  M2.backward(speedFast);
  M3.backward(speedFast);
  M4.backward(speedFast);
  if (timeToDrive < minMoveTimeMs){
    delay(minMoveTimeMs - timeToDrive);
  }
  delay(timeToDrive);
}

void turnLeft(float deg){
  unsigned long timeToTurn = (deg / 360.0) * timeToTurn360Milliseconds;
  M1.backward(speedFast);
  M3.backward(speedFast);
  M2.forward(speedFast);
  M4.forward(speedFast);
  if (timeToTurn < minMoveTimeMs){
    delay(minMoveTimeMs - timeToTurn);
  }
  delay(timeToTurn);
}

void turnLeftSlow(float deg){
  unsigned long timeToTurn = (deg / 360.0) * timeToTurn360Milliseconds;
  M1.backward(speedSlow);
  M3.backward(speedSlow);
  M2.forward(speedSlow);
  M4.forward(speedSlow);
  if (timeToTurn < minMoveTimeMs){
    delay(minMoveTimeMs - timeToTurn);
  }
  delay(timeToTurn);
}

void turnRight(float deg){
  unsigned long timeToTurn = (deg / 360.0) * timeToTurn360Milliseconds;
  Serial.print("Time To Turn: ");
  Serial.println(timeToTurn);
  M2.backward(speedFast);
  M4.backward(speedFast);
  M1.forward(speedFast);
  M3.forward(speedFast);
  if (timeToTurn < minMoveTimeMs){
    timeToTurn = minMoveTimeMs;
  }
  delay(timeToTurn);
}

void turnRightSlow(float deg){
  unsigned long timeToTurn = (deg / 360.0) * timeToTurn360Milliseconds;
  Serial.print("Time To Turn: ");
  Serial.println(timeToTurn);
  M2.backward(speedSlow);
  M4.backward(speedSlow);
  M1.forward(speedSlow);
  M3.forward(speedSlow);
  if (timeToTurn < minMoveTimeMs){
    timeToTurn = minMoveTimeMs;
  }
  delay(timeToTurn);
}

void moveRight(int distancemm){
  unsigned long timeToMove = (distancemm / 1000.0) * timeToMove1000mmSidewaysMilliseconds;
 
  M1.backward(speedSlow);
  M2.forward(speedFast);
  M3.forward(speedSlow);
  M4.backward(speedSlow);
  
  if (timeToMove < minMoveTimeMs){
    timeToMove = minMoveTimeMs;
  }
  delay(timeToMove);
}

void moveLeft(int distancemm){
  unsigned long timeToMove = (distancemm / 1000.0) * timeToMove1000mmSidewaysMilliseconds;
  M1.forward(speedFast);
  M2.backward(speedFast);
  M3.backward(speedFast);
  M4.forward(speedFast);
  if (timeToMove < minMoveTimeMs){
    timeToMove = minMoveTimeMs;
  }
  delay(timeToMove);
}

void rechtsKurve(int distancemm){  
  unsigned long moveTime = (distancemm * 1000) / distancePerSecond; 
  M1.forward(speedFast);
  M2.forward(speedSlow);
  M3.forward(speedFast);
  M4.forward(speedSlow);
  if (moveTime < minMoveTimeMs){
    delay(minMoveTimeMs - moveTime);
  }
  delay(moveTime);
}

void linksKurve(int distancemm){ 
  unsigned long moveTime = (distancemm * 1000) / distancePerSecond; 
  M1.forward(speedSlow);
  M2.forward(speedFast);
  M3.forward(speedSlow);
  M4.forward(speedFast);
  if (moveTime < minMoveTimeMs){
    delay(minMoveTimeMs - moveTime);
  }
  delay(moveTime);
}

void stopMotors(){
  M1.brake();
  M2.brake();
  M3.brake();
  M4.brake();
}

void startMotors(){
  klappe.attach(servoPin);
}

// nach rechts bewegen bis in gewünschtem Abstand zur Wand
void moveToRightWall(uint16_t distanceToWall){  // nach rechts bewegen bis in gewünschtem Abstand zur Wand
  uint16_t distanceFront = readTofFront();
  uint16_t distanceBack;
  String message = "Abstand zur Wand von " + String(distanceToWall) + "mm mm wird hergestellt.";
  logMessage(message.c_str());
  if (distanceFront > distanceToWall){
    while (distanceFront > distanceToWall){
      moveRight(1);
      distanceFront = readTofFront();
      distanceBack = readTofBack();
      if(abs(distanceBack - distanceFront) > toleranceWheelsmm + 5){
        goParallel();
      }
    }
  }

  else if (distanceFront < distanceToWall){
    while (distanceFront < distanceToWall){
      moveLeft(1);
      distanceFront = readTofFront();
      distanceBack = readTofBack();
      if(abs(distanceBack - distanceFront) > toleranceWheelsmm + 5){
        goParallel();
      }
    }
  }
  stopMotors();
}

void goParallel(){
    logMessage("Parallelität zur Wand wird hergestellt...");
    uint16_t distanceFront = readTofFront();
    uint16_t distanceBack = readTofBack();
    while (abs((int)distanceFront - (int)distanceBack) > toleranceWheelsmm){
        if (distanceFront > distanceBack){
        turnRightSlow(incrementGrad);
        }
        else {
        turnLeftSlow(incrementGrad);
        }

      distanceFront = readTofFront();
      distanceBack = readTofBack();
    }
    stopMotors();
}


void moveForwardParallelUntilContainer(uint16_t distanceToWall){
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
  while (abs(readTofBackUnfiltered() - readTofFrontUnfiltered()) < containerDepth - 3){
      distanceFront = readTofFront();
      distanceBack = readTofBack();
    if (abs(distanceFront - distanceBack) < toleranceWheelsmm){
      moveForward(incrementDistance);
    }
    else if (distanceBack - distanceFront > toleranceWheelsmm){
      turnLeftSlow(incrementDistance);
    }

    else {
      turnRightSlow(incrementDistance);
    }
  }
  stopMotors();
  logMessage("Container gefunden!");
}

void parkieren(){
  while(!digitalRead(endschalterHinten)){
      moveBackward(incrementDistance);
  }
  stopMotors();
}

void moveOutOfDock(){
  moveForward(dockLength);
  stopMotors();
}

void pickUpContainer(){
  pixyLampeOn();
  pixyMoveBackwardUntilObject();
  pixyMoveMiddle();
  pixyMoveForward();
  pixyLampeOff();
}

void abladen(){
  klappe.write(posKlappeOffen);
  delay(zeitEntleeren);
  klappe.write(posKlappeGeschlossen);
}

void containerAufladen(){
  LinearAntrieb.forward(speedFast);
  delay(bewegungsZeitLinear);
  LinearAntrieb.brake();
  LinearAntrieb.backward(speedFast);
  while(!digitalRead(endschalterUnten)){
    delay(1);
  }
  LinearAntrieb.brake();
}

void rueckwaertsBisAnschlag(){
  while(!digitalRead(endschalterHinten)){
    moveBackward(1);
  }
  stopMotors();
}
