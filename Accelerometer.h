
#ifndef Accelerometer_h
#define Accelerometer_h

#include <Wire.h>
#include <Arduino.h>
#include "JY901.h"

struct Acceleration {
      float X;
      float Y;
      float Z;
};

struct Gyro {
  float X;
  float Y;
  float Z;
};

struct Angle {
  float X;
  float Y;
  float Z;
};

class Accelerometer {
  
  public: 

    JY901 conn;

    // serial baud rate must be 115200
    void Initialize() {
      Serial2.begin(115200);
    }

    Acceleration GetAcceleration() {
      float x = xAcc();
      float y = yAcc();
      float z = zAcc();
      Acceleration xAcc = { x, y, z };
      return xAcc;
    }

    Gyro GetGyro() {
      float x = xGyro();
      float y = yGyro();
      float z = zGyro();
      Gyro xGyro = { x, y, z };
      return xGyro;
    }

    Angle GetAngle() {
      float x = xAngle();
      float y = yAngle();
      float z = zAngle();
      z = ConvertZAngle(z);
      Angle xAngle = { x, y, z };
      return xAngle;
    }

    float ConvertZAngle(float angle) {

      if (angle < 0) {
        return abs(angle);
      }

      return abs(angle - 360);
    }

    void UpdateValues() {
        while (Serial2.available()) 
        {
          conn.CopeSerialData(Serial2.read()); //Call JY901 data cope function
        }
    }

    void Flush() {
      Serial2.flush();
    }

    // collect acceleration data
    float xAcc(){
      return (float)conn.stcAcc.a[0] / 32768 * 16;
    }

    float yAcc(){
      return (float)conn.stcAcc.a[1] / 32768 * 16;
    }

    float zAcc(){
      return (float)conn.stcAcc.a[2] / 32768 * 16;
    }

    // collect gyroscope acceleration data
    float xGyro(){
      return (float)conn.stcGyro.w[0] / 32768 * 2000;
    }

    float yGyro(){
      return (float)conn.stcGyro.w[1] / 32768 * 2000;
    }

    float zGyro(){
      return (float)conn.stcGyro.w[2] / 32768 * 2000;
    }

    // collect angle data
    float xAngle(){
      return (float)conn.stcAngle.Angle[0] / 32768 * 180;
    }

    float yAngle(){
      return (float)conn.stcAngle.Angle[1] / 32768 * 180;
    }

    float zAngle(){
      return (float)conn.stcAngle.Angle[2] / 32768 * 180;
    }
};

#endif