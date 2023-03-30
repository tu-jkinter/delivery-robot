#ifndef Motor_h
#define Motor_h

#include "Arduino.h"
#include "Encoder.h"

class Motor {

  public:

    //Pins 
    int SpeedPin;
    int DirectionPin1;
    int DirectionPin2;

    //Encoder
    Encoder Encoder;

    //Constants
    int MAX_SPEED = 255;
    int MIN_SPEED = 50;

    void Initialize(char encoder) {
      
      int pinA, pinB;
      
      switch (encoder) {
        case 'A':
        {
          SpeedPin = 11;
          DirectionPin1 = 5;
          DirectionPin2 = 6;
          pinA = 52;
          pinB = 53;
          break;
        }

        case 'B':
        {
          SpeedPin = 12;
          DirectionPin1 = 7;
          DirectionPin2 = 8;
          pinA = 50;
          pinB = 51;
          break;
        }

        case 'C': 
        {
          SpeedPin = 9;
          DirectionPin1 = 22;
          DirectionPin2 = 24;
          pinA = 48;
          pinB = 49;
          break;
        }

        case 'D': 
        {
          SpeedPin = 10;
          DirectionPin1 = 26;
          DirectionPin2 = 28;
          pinA = 46;
          pinB = 47;
          break;
        }
      }
      
      Encoder.Initialize(pinA, pinB);

      pinMode(SpeedPin, OUTPUT);
      pinMode(DirectionPin1, OUTPUT);
      pinMode(DirectionPin2, OUTPUT);
    }

    //Rotate this motor (or wheel) forward or in reverse.
    //true = forward, false = reverse.
    void Rotate(int speed, bool forward) {

        if (forward) {
            digitalWrite(DirectionPin1, HIGH);
            digitalWrite(DirectionPin2, LOW);
        } else {
            digitalWrite(DirectionPin1, LOW);
            digitalWrite(DirectionPin2, HIGH);
        }

        analogWrite(SpeedPin, speed);
    }
};


#endif