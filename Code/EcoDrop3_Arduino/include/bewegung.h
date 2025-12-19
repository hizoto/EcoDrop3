#pragma once
#include "sensors.h"
#include "communication.h"

class DC_Motor{
  private:
    int IN1;
    int IN2;
    int ENA;
  public:
    DC_Motor(int _IN1, int _IN2, int _ENA);

    void brake();
    void forward(int speed);
    void backward(int speed);
};

constexpr int speedNormal = 100;

void startMotors();
void moveForward(int distancemm, int speed = speedNormal);
void moveBackward(int distancemm, int speed = speedNormal);
void turnLeft(float deg);
void turnRight(float deg);
void turnLeftSlow(float deg);
void turnRightSlow(float deg);
void moveLeft(int distancemm, int speed = speedNormal);
void moveRight(int distancemm, int speed = speedNormal);
void moveForwardParallelUntilContainer(uint16_t distanceToWall);
void driveForwardWithWheelCorrection(int baseSpeed, int correction, unsigned long durationMs);
void moveToRightWall(uint16_t distanceToWall);
void moveToLefttWall(uint16_t distanceToWall);
void stopMotors();
void goParallelLeft();
void goParallelRight();
void parkieren();
void moveOutOfDock();
void pickUpContainer();
void abladen();
void rueckwaertsBisAnschlag();
void containerAufladen();


