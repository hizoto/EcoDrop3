#ifndef BEWEGUNG_H
#define BEWEGUNG_H
#include "sensors.h"

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

void moveForward(int distancemm);
void turnLeft(int deg);
void turnRight(int deg);
void moveLeft(int distancemm);
void moveRight(int distancemm);
void moveForwardParallelUntilContainer(int distanceToWall);
void turnSlowRight(int distancemm);
void turnSlowLeft(int distancemm);
void moveToRightWall(int distanceToWall);
void goParallel();

#endif

