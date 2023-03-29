
#ifndef Convert_h
#define Convert_h

#include <Arduino.h>

class Convert {

    public: 

      float FeetToInches(float feet) {
        return feet * 12;
      }
};


#endif