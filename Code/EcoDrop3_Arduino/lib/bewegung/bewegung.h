#ifndef BEWEGUNG_H
#define BEWEGUNG_H

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


void turnRight();

#endif

