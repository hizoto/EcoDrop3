class ultraschallsensor{
  private:
    int echo;
    int trig;

  public:
    ultraschallsensor(int _echo, int _trig);

    unsigned long getDistance();
};
