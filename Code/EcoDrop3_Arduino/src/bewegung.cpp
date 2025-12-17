#include <Arduino.h>
#include "bewegung.h"
#include "Pixy2Cam.h"
#include <Servo.h>

const int speedFast = 100; // max 255
const int speedSlow = 75;
// Toleranz Wandabstand
const int toleranceToWallmm = 5;
// Toleranz für parallelität
const int toleranceWheelsmm = 5;
const int containerDepth = 40;

unsigned long timeToTurn360Milliseconds = 7750;
unsigned long timeToMove1000mmSidewaysMilliseconds = 10000;   
unsigned long timeToMove1000mm = 6500;              //TODO 
unsigned long bewegungsZeitLinear = 7000; // in ms
int dockLength = 350; // TODO
int posKlappeOffen = 0; // TODO
int posKlappeGeschlossen = 90; // TODO
int zeitEntleeren = 2000;

float distancePerSecond = 1000 / (timeToMove1000mm / 1000.0);                      


const int incrementDistance = 1;
const float incrementGrad = 1;
const int minMoveTimeMs = 50;

// PINS
int endschalterHinten = 42; 
int endschalterUnten = 40;
int servoPin = 13;

// Motor 1 Vorne links
const int B1_IN1 = 39;  // ehemals 53
const int B1_IN2 = 38;  // ehemals 52
const int B1_ENA = 11;

// Motor 2 Vorne rechts
const int B1_IN3 = 48;
const int B1_IN4 = 49;
const int B1_ENB = 9;

// Motor 3 Hinten links
const int B2_IN1 = 47; 
const int B2_IN2 = 46; 
const int B2_ENA = 8;

// Motor 4 Hinten rechts
const int B2_IN3 = 36;  // ehemals 50
const int B2_IN4 = 37;  // ehemals 51
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


void driveForwardWithWheelCorrection(int baseSpeed, int correction, unsigned long durationMs){
  int leftSpeed = constrain(baseSpeed - correction, 0, 255);
  int rightSpeed = constrain(baseSpeed + correction, 0, 255);

  M1.forward(leftSpeed);
  M3.forward(leftSpeed);
  M2.forward(rightSpeed);
  M4.forward(rightSpeed);

  if (durationMs < minMoveTimeMs){
    durationMs = minMoveTimeMs;
  }
  delay(durationMs);
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
  M2.backward(speedSlow);
  M4.backward(speedSlow);
  M1.forward(speedSlow);
  M3.forward(speedSlow);
  if (timeToTurn < minMoveTimeMs){
    timeToTurn = minMoveTimeMs;
  }
  delay(timeToTurn);
}

void moveLeft(int distancemm){
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

void moveRight(int distancemm){
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
  pinMode(endschalterHinten, INPUT_PULLUP);
  pinMode(endschalterUnten, INPUT_PULLUP);
  klappe.attach(servoPin);
}

// nach rechts bewegen bis in gewünschtem Abstand zur Wand
void moveToRightWall(uint16_t distanceToWall) {  // nach rechts bewegen bis in gewünschtem Abstand zur Wand
  uint16_t distanceFront = readTofFrontUnfiltered();
  uint16_t distanceBack  = readTofBackUnfiltered();

  // Falls bereits innerhalb Toleranz: nichts tun
  if (abs((int)distanceFront - (int)distanceToWall) <= toleranceToWallmm) {
    String message = "Abstand zur Wand ist OK: " + String(distanceFront) + "mm (Ziel " +
                     String(distanceToWall) + "mm ±" + String(toleranceToWallmm) + "mm). Überspringe.";
    logMessage(message.c_str());
    stopMotors();
    return;
  }

  String message = "Abstand zur Wand von " + String(distanceToWall) + "mm ±" +
                   String(toleranceToWallmm) + "mm wird hergestellt.";
  logMessage(message.c_str());

  // Zu weit weg -> nach rechts (bis innerhalb Toleranz)
  if (distanceFront > distanceToWall) {
    while (distanceFront > (distanceToWall + toleranceToWallmm)) {
      moveRight(1);
      distanceFront = readTofFrontUnfiltered();
      distanceBack  = readTofBackUnfiltered();
      if (abs((int)distanceBack - (int)distanceFront) > (toleranceWheelsmm + 5)) {
        goParallel();
      }
    }
  }
  // Zu nah -> nach links (bis innerhalb Toleranz)
  else { // distanceFront < distanceToWall
    while (distanceFront < (distanceToWall - toleranceToWallmm)) {
      moveLeft(1);
      distanceFront = readTofFrontUnfiltered();
      distanceBack  = readTofBackUnfiltered();
      if (abs((int)distanceBack - (int)distanceFront) > (toleranceWheelsmm + 5)) {
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
        stopMotors();
        }
        else {
        turnLeftSlow(incrementGrad);
        stopMotors();
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

  const float kp = 2.0f;
  const int baseSpeed = speedSlow;
  const unsigned long baseDurationMs = (incrementDistance / distancePerSecond) * 1000;
  

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
      distanceFront = readTofFrontUnfiltered();
      distanceBack = readTofBackUnfiltered();/*
      int16_t error = static_cast<int16_t>(distanceBack) - static_cast<int16_t>(distanceFront);
      int correction = static_cast<int>(kp * error);
      driveForwardWithWheelCorrection(baseSpeed, correction, baseDurationMs);*/
      moveForward(1);
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
  moveLeft(50);
  turnRight(90);
  moveLeft(150);
  pixyLampeOn();
  pixyMoveForwardUntilObject();
  pixyMoveMiddle(255);
  pixyMoveForward();
  pixyLampeOff();
  containerAufladen();
}

void abladen(){
  klappe.write(posKlappeOffen);
  delay(zeitEntleeren);
  klappe.write(posKlappeGeschlossen);
}

void containerAufladen(){
  bool gabelIstUnten = digitalRead(endschalterUnten);
  LinearAntrieb.forward(speedFast);
  delay(bewegungsZeitLinear);
  LinearAntrieb.backward(speedFast);
  gabelIstUnten = digitalRead(endschalterUnten);
  
  while(!gabelIstUnten){
    delay(1);
    gabelIstUnten = digitalRead(endschalterUnten);
  }
  delay(25);
  LinearAntrieb.brake();
}

void rueckwaertsBisAnschlag(){
  while(!digitalRead(endschalterHinten)){
    moveBackward(1);
  }
  stopMotors();
}
