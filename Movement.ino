/*
  This sketch is designed to control the movement of the robot and abstract
  away controls into a simple class. 
*/

#include "Controller.h"
#include "Accelerometer.h"
#include "Convert.h"
#include "Encoder.h"
#include "Bluetooth.h"
#include <Arduino.h>

Bluetooth bluetooth;
Controller Control;
Convert convert;

void setup() {
    
    Serial.begin(115200); //Baud rate for IMU
    Serial1.begin(9600); //Baud rate for bluetooth module

    //Initialize robot control
    Control.Initialize();
    Control.Stop();
}


void loop() {
  char data = bluetooth.Update();
  SetManualControl(data);
  delay(30);
}


//Serial event to read in Accelerometer data
void serialEvent2() {

  while (Serial2.available()) {
    Control.Accelerometer.Connection.CopeSerialData(Serial2.read()); //Call JY901 data cope function
  }
}

//Use the bluetooth data to process manual robot movement
void SetManualControl(char data) {

  switch (data) {

    case 'A': 
      Control.Forward(100);
      break;

    case 'B': 
      Control.Reverse(100);
      break;

    case 'C': 
      Control.Shift(130, 2);
      break;

    case 'D': 
      Control.Shift(130, 6);
      break;

    case 'E': 
      Control.Shift(130, 1);
      break;

    case 'F': 
      Control.Shift(130, 7);
      break;

    case 'G': 
      Control.Shift(130, 3);
      break;

    case 'H': 
      Control.Shift(130, 5);
      break;

    case 'I': 
      Control.Rotate(100, true);
      break;

    case 'J': 
      Control.Rotate(100, false);
      break;

    case 'K': 
      Control.Arc(125, true);
      break;

    case 'L': 
      Control.Arc(125, false);
      break;
    
    case 'M': 
      Control.Stop();
      break;

    case 'N': 
      Control.Stop();
      break;

    default: 
      break;
  }
}



