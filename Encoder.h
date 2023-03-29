
#ifndef Encoder_h
#define Encoder_h

#include "Arduino.h"

class Encoder {

  public:
  
    int Count = 0;
    int State;
    int LastState; 

    void Initialize(int pinA, int pinB) {

      PinA = pinA;
      PinB = pinB;

      pinMode (PinA, INPUT);
      pinMode (PinB, INPUT);
      LastState = digitalRead(PinA); 
    }

    void Update() {
      
      // Reads the "current" state of the PinA
      State = digitalRead(PinA); 

      // If the previous and the current state of the outputA are different, that means a Pulse has occured
      if (State != LastState) {   

        // If the PinB state is different to the PinA state, that means the encoder is rotating clockwise
        if (digitalRead(PinB) != State) { 
          Count += 1;
        } else {
          Count -= 1;
        }
      } 

      // Updates the previous state of the outputA with the current state
      LastState = State; 
    }

    void Reset() {
      Count = 0;
    }
  
  private: 

    int PinA;
    int PinB;
};


#endif