/*
  This sketch is designed to control the movement of the robot and abstract
  away controls into a simple class. 
*/

#include "Controller.h"
#include "Accelerometer.h"
#include "Encoder.h"
#include "Bluetooth.h"
#include "Lidar.h"
#include <Arduino.h>

//Class instances
Bluetooth Bluetooth;
Controller Control;
Lidar FrontLidar;
Lidar BackLidar;

//Global properties
bool isManualMode = true;
int manualSpeed = 75;

struct RotateData {
  float Angle; 
  int Distance;
};

void setup() {
    
    Serial.begin(115200); //Baud rate for IMU
    Serial1.begin(9600); //Baud rate for bluetooth module

    //Initialize robot control
    Control.Initialize();
    Control.Stop();

    //Initialize LiDAR
    Wire.begin();
    FrontLidar.Initialize(0x10);
    BackLidar.Initialize(0x09);
}


void loop() {

  //Update front lidar data
  LidarData frontData = FrontLidar.Fetch();
  LidarData backData = BackLidar.Fetch();

  //Update bluetooh data
  char data = Bluetooth.Update();

  //If START is pressed (Z) on remote, switch manual trigger.
  if (data == 'Z') {
    isManualMode = !isManualMode;
  }

  if (isManualMode) {

    if (data == 'X') {
      UpdateSpeed(manualSpeed + 5);
    }
    else if (data == 'Y') {
      UpdateSpeed(manualSpeed - 5);
    }

    SetManualControl(data, frontData, backData); 
  }
  else {
    if (frontData.Distance <= 30) {
      Control.Reverse(50, 30 - frontData.Distance);
      RotateData target = FindNewDirection();

      float difference = target.Angle - Control.Accelerometer.GetAngle().Z;

      if (difference > 0) {
        Control.Rotate(60, true, difference);
      }
      else {
        Control.Rotate(60, false, abs(difference));
      }
    }
    else {
      Control.Forward(50);
    }
  }
}

void UpdateSpeed(int speed) {

  if (speed < 50) {
    speed = 50;
  }

  if (speed > 150) {
    speed = 150;
  }

  manualSpeed = speed;
}

RotateData FindNewDirection() {

  RotateData data[36];

  for (int i = 0; i < 18; i++)
  {
    Control.Rotate(50, true, 10);

    //Capture lidar data
    LidarData frontData = FrontLidar.Fetch();
    LidarData backData = BackLidar.Fetch();

    //Obtain distance for front and back lidars
    data[i].Distance = frontData.Distance;
    data[i+18].Distance = backData.Distance;

    //Obtain angle for front and back lidars
    data[i].Angle = Control.Accelerometer.GetAngle().Z;
    data[i+18].Angle = data[i].Angle + 180;
  }

  RotateData target = {0, 0};

  for (int i = 0; i < 36; i++) {
    if (data[i].Distance > target.Distance) {
      target = data[i];
    }
  }

  return target;
}

//Serial event to read in Accelerometer data
void serialEvent2() {

  while (Serial2.available()) {
    Control.Accelerometer.Connection.CopeSerialData(Serial2.read()); //Call JY901 data cope function
  }
}

//Use the bluetooth data to process manual robot movement
void SetManualControl(char data, LidarData frontData, LidarData backData) {

  switch (data) {

    case 'A': 
    {
      if (frontData.Distance <= 30) {
        Control.Reverse(75);
      }
      else if (frontData.Distance > 30 && frontData.Distance <= 100) {
        Control.Forward(50);
      }
      else {
        Control.Forward(manualSpeed);
      }
      break;
    }

    case 'B': 

      if (backData.Distance <= 30) {
        Control.Forward(75);
      }
      else if (backData.Distance > 30 && backData.Distance <= 100) {
        Control.Reverse(50);
      }
      else {
        Control.Reverse(manualSpeed);
      }
      break;

    case 'C': 
      Control.Shift(manualSpeed + 75, 2);
      break;

    case 'D': 
      Control.Shift(manualSpeed + 75, 6);
      break;

    case 'E': 
      Control.Shift(manualSpeed + 75, 1);
      break;

    case 'F': 
      Control.Shift(manualSpeed + 75, 7);
      break;

    case 'G': 
      Control.Shift(manualSpeed + 75, 3);
      break;

    case 'H': 
      Control.Shift(manualSpeed + 75, 5);
      break;

    case 'I': 
      Control.Rotate(manualSpeed + 25, true);
      break;

    case 'J': 
      Control.Rotate(manualSpeed + 25, false);
      break;

    case 'K': 
      Control.Arc(manualSpeed + 25, true);
      break;

    case 'L': 
      Control.Arc(manualSpeed + 25, false);
      break;
    
    case 'M': 
      Control.Stop();
      break;

    case 'N': 
      Control.Stop();
      break;

    case 'O':
      if (frontData.Distance <= 30) {
        Control.Reverse(75);
      }
      else {
         Control.Turn(true, true, manualSpeed);
      }
      break;
    
    case 'P':
      if (frontData.Distance <= 30) {
        Control.Reverse(75);
      }
      else {
         Control.Turn(true, false, manualSpeed);
      }
      break;

    case 'Q':
      if (backData.Distance <= 30) {
        Control.Reverse(75);
      }
      else {
         Control.Turn(false, true, manualSpeed);
      }
      break;

    case 'R':
      if (backData.Distance <= 30) {
        Control.Reverse(75);
      }
      else {
         Control.Turn(false, false, manualSpeed);
      }
      break;

    default: 
      break;
  }
}



