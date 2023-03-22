#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor {

  public:

    //Motor Pins
    int SpeedPinRightFront = 12;
    int SpeedPinLeftFront = 11;
    int SpeedPinRightBack = 10;
    int SpeedPinLeftBack = 9;

    int RightMotorDirPin1 = 7;
    int RightMotorDirPin2 = 8;

    int LeftMotorDirPin1 = 5;
    int LeftMotorDirPin2 = 6;

    int RightMotorDirPin1B = 26;
    int RightMotorDirPin2B = 28;

    int LeftMotorDirPin1B = 22;
    int LeftMotorDirPin2B = 24;

    int MAX_SPEED = 255;
    int MIN_SPEED = 80;

  private:
};


#endif