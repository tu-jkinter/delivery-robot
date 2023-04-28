
#ifndef Bluetooth_h
#define Bluetooth_h

#include "Arduino.h"

#define MAX_PACKETSIZE 32    //Serial receive buffer

class Bluetooth {

  private: 
   
    char buffUART[MAX_PACKETSIZE];
    unsigned int buffUARTIndex = 0; 
    unsigned long preUARTTick = 0;

  public: 
    
    //Acquire the most recently received bluetooth module data as a single character.
    char Update() {
      
      delay(30); //IMPORTANT - DONT DELETE THIS
      char UARTData = 0;

      if (Serial1.available()) {

        size_t length = Serial1.available();
        uint8_t serialBuffer[length + 1];
        serialBuffer[length] = 0x00;
        Serial1.readBytes(serialBuffer, length);

        //Ensure the serial port can read the entire frame of data
        memcpy(buffUART + buffUARTIndex, serialBuffer, length);
        buffUARTIndex += length;
        preUARTTick = millis();

        if (buffUARTIndex >= MAX_PACKETSIZE - 1) {
          buffUARTIndex = MAX_PACKETSIZE - 2;
          preUARTTick = preUARTTick - 200;
        }

        //Check that the data is ready to be read
        if (buffUARTIndex > 0 && (millis() - preUARTTick >= 100))
        { 
          buffUART[buffUARTIndex] = 0x00;
          UARTData = buffUART[0];
          buffUARTIndex = 0;
        }

        return UARTData;
      }

      else {
        Serial1.flush();
        Serial1.end();
        Serial1.begin(9600);
        return 'N';
      }
    }
};

#endif