#include <Arduino.h>
#include "bewegung.h"
#include "Pixy2Cam.h"
#include <Servo.h>

const int speedFast = 150; // max 255
const int speedSlow = 50;
// Toleranz Wandabstand
const int toleranceToWallmm = 5;
// Toleranz für parallelität
const int toleranceWheelsmm = 5;
const int containerDepth = 40;

unsigned long timeToTurn360Milliseconds = 7750;
unsigned long timeToMove1000mmSidewaysMilliseconds = 10000;   
unsigned long timeToMove1000mm = 6500;              
unsigned long bewegungsZeitLinear = 7000; // in ms
int dockLength = 400; 
int posKlappeOffen = 0; 
int posKlappeGeschlossen = 100; 
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

void DC_Motor::idle(){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, 0);
}


void moveForward(int distancemm, int speed){
  unsigned long timeToDrive = (distancemm / distancePerSecond) * 1000;
  M1.forward(speed);
  M2.forward(speed);
  M3.forward(speed);
  M4.forward(speed);
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

void idleMotors(){
  M1.idle();
  M2.idle();
  M3.idle();
  M4.idle();
}

void moveBackward(int distancemm, int speed){
  unsigned long timeToDrive = (distancemm / distancePerSecond) * 1000;
  M1.backward(speed);
  M2.backward(speed);
  M3.backward(speed);
  M4.backward(speed);
  if (timeToDrive < minMoveTimeMs){
    delay(minMoveTimeMs - timeToDrive);
  }
  delay(timeToDrive);
}

void turnLeft(float deg, int speed){
  unsigned long timeToTurn = (deg / 360.0) * timeToTurn360Milliseconds;
  M1.backward(speed);
  M3.backward(speed);
  M2.forward(speed);
  M4.forward(speed);
  if (timeToTurn < minMoveTimeMs){
    delay(minMoveTimeMs - timeToTurn);
  }
  delay(timeToTurn);
}

void turnRight(float deg, int speed){
  unsigned long timeToTurn = (deg / 360.0) * timeToTurn360Milliseconds;
  M2.backward(speed);
  M4.backward(speed);
  M1.forward(speed);
  M3.forward(speed);
  if (timeToTurn < minMoveTimeMs){
    timeToTurn = minMoveTimeMs;
  }
  delay(timeToTurn);
}

void moveLeft(int distancemm, int speed){
  unsigned long timeToMove = (distancemm / 1000.0) * timeToMove1000mmSidewaysMilliseconds;
 
  M1.backward(speed);
  M2.forward(speed);
  M3.forward(speed);
  M4.backward(speed);
  
  if (timeToMove < minMoveTimeMs){
    timeToMove = minMoveTimeMs;
  }
  delay(timeToMove);
}

void moveRight(int distancemm, int speed){
  unsigned long timeToMove = (distancemm / 1000.0) * timeToMove1000mmSidewaysMilliseconds;
  M1.forward(speed);
  M2.backward(speed);
  M3.backward(speed);
  M4.forward(speed);
  if (timeToMove < minMoveTimeMs){
    timeToMove = minMoveTimeMs;
  }
  delay(timeToMove);
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
  klappe.write(posKlappeGeschlossen);
}

// nach rechts bewegen bis in gewünschtem Abstand zur Wand
void moveToRightWall(uint16_t distanceToWall) {  // nach rechts bewegen bis in gewünschtem Abstand zur Wand
  uint16_t distanceFront = tofFR().readFiltered();
  uint16_t distanceBack  = tofBR().readFiltered();

  // Falls bereits innerhalb Toleranz: nichts tun
  if (abs((int)distanceFront - (int)distanceToWall) <= toleranceToWallmm) {
    String message = "Abstand zur Wand ist OK: " + String(distanceFront) + "mm (Ziel " +
                     String(distanceToWall) + "mm ±" + String(toleranceToWallmm) + "mm). Überspringe.";
    logMessage(message.c_str());
    stopMotors();
    return;
  }

  String message = "Abstand zur rechten Wand von " + String(distanceToWall) + "mm ±" +
                   String(toleranceToWallmm) + "mm wird hergestellt.";
  logMessage(message.c_str());

  // Zu weit weg -> nach rechts (bis innerhalb Toleranz)
  if (distanceFront > distanceToWall) {
    while (distanceFront > (distanceToWall + toleranceToWallmm)) {
      moveRight(1);
      distanceFront = tofFR().readRaw();
      distanceBack  = tofBR().readRaw();
      if (abs((int)distanceBack - (int)distanceFront) > (toleranceWheelsmm + 5)) {
        goParallelRight();
      }
    }
  }
  // Zu nah -> nach links (bis innerhalb Toleranz)
  else { // distanceFront < distanceToWall
    while (distanceFront < (distanceToWall - toleranceToWallmm)) {
      moveLeft(1);
      distanceFront = tofFR().readRaw();
      distanceBack  = tofBR().readRaw();
      if (abs((int)distanceBack - (int)distanceFront) > (toleranceWheelsmm + 5)) {
        goParallelRight();
      }
    }
  }

  stopMotors();
}

void moveToLeftWall(uint16_t distanceToWall) {  // nach links bewegen bis in gewünschtem Abstand zur Wand
  uint16_t distanceFront = tofFL().readFiltered();
  uint16_t distanceBack  = tofBL().readFiltered();

  // Falls bereits innerhalb Toleranz: nichts tun
  if (abs((int)distanceFront - (int)distanceToWall) <= toleranceToWallmm) {
    String message = "Abstand zur Wand ist OK: " + String(distanceFront) + "mm (Ziel " +
                     String(distanceToWall) + "mm ±" + String(toleranceToWallmm) + "mm). Überspringe.";
    logMessage(message.c_str());
    stopMotors();
    return;
  }

  String message = "Abstand zur linken Wand von " + String(distanceToWall) + "mm ±" +
                   String(toleranceToWallmm) + "mm wird hergestellt.";
  logMessage(message.c_str());

  // Zu weit weg -> nach links (bis innerhalb Toleranz)
  if (distanceFront > distanceToWall) {
    while (distanceFront > (distanceToWall + toleranceToWallmm)) {
      moveLeft(1);
      distanceFront = tofFL().readRaw();
      distanceBack  = tofBL().readRaw();
      if (abs((int)distanceBack - (int)distanceFront) > (toleranceWheelsmm + 5)) {
        goParallelLeft();
      }
    }
  }
  // Zu nah -> nach rechts (bis innerhalb Toleranz)
  else { // distanceFront < distanceToWall
    while (distanceFront < (distanceToWall - toleranceToWallmm)) {
      moveRight(1);
      distanceFront = tofFL().readRaw();
      distanceBack  = tofBL().readRaw();
      if (abs((int)distanceBack - (int)distanceFront) > (toleranceWheelsmm + 5)) {
        goParallelLeft();
      }
    }
  }

  stopMotors();
}

void goParallelRight(){ 
    logMessage("Parallelität zur rechten Wand wird hergestellt...");
    uint16_t distanceFront = tofFR().readRaw();
    uint16_t distanceBack = tofBR().readRaw();
    while (abs((int)distanceFront - (int)distanceBack) > toleranceWheelsmm){
        if (distanceFront > distanceBack){
        turnRight(incrementGrad, speedSlow);
        stopMotors();
        }
        else {
        turnLeft(incrementGrad, speedSlow);
        stopMotors();
        }
      logTofs();
      distanceFront = tofFR().readRaw();
      distanceBack = tofBR().readRaw();
    }
    stopMotors();
}

void goParallelLeft(){
    logMessage("Parallelität zur linken Wand wird hergestellt...");
    uint16_t distanceFront = tofFL().readFiltered();
    uint16_t distanceBack = tofBL().readFiltered();
    while (abs((int)distanceFront - (int)distanceBack) > toleranceWheelsmm){
        if (distanceFront < distanceBack){
        turnRight(incrementGrad, speedSlow);
        stopMotors();
        }
        else {
        turnLeft(incrementGrad, speedSlow);
        stopMotors();
        }
        logTofs();
      distanceFront = tofFL().readFiltered();
      distanceBack = tofBL().readFiltered();
    }
    stopMotors();
}

void moveForwardParallelUntilContainer(uint16_t distanceToWall){
  // abstandserfassung
  uint16_t distanceFront = tofFR().readFiltered();
  uint16_t distanceBack = tofBR().readFiltered();

  const float kp = 2.0f;
  const int baseSpeed = speedSlow;
  const unsigned long baseDurationMs = (incrementDistance / distancePerSecond) * 1000;
  

  // INITIAL PARALLELIZATION
  goParallelRight();

  // Move to wall distance
  moveToRightWall(distanceToWall);

  goParallelRight();
  distanceFront = tofFR().readFiltered();
  distanceBack = tofBR().readFiltered();

  // Main Logic of the function
  logMessage("Suche Containter...");
  while (abs(tofBR().readRaw() - tofFR().readRaw()) < containerDepth - 3){
      distanceFront = tofFR().readRaw();
      distanceBack = tofBR().readRaw();/*
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
  pixyLampeOn();
  moveLeft(50);
  pixyMoveForwardUntilObject();
  pixyMoveMiddle(150); //166
  delay(500);
  pixyMoveForward();
  pixyLampeOff();
  containerAufladen();
  moveBackward(130);
}

void abladen(){
  klappe.write(posKlappeOffen);
  delay(zeitEntleeren);
  klappe.write(posKlappeGeschlossen);
}

void oeffnen(){
  klappe.write(posKlappeOffen);
}

void schliessen(){
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

void staplerOben(){
  LinearAntrieb.forward(speedFast);
}

void staplerUnten(){
  LinearAntrieb.backward(speedFast);
}