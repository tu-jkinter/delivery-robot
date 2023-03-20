#include <Wire.h>
#include "JY901.h"

// serial baud rate must be 115200
void InitAccelerometer(){
  Serial2.begin(115200);
}

// collect acceleration data
float xAcc(){
  return (float)JY901.stcAcc.a[0]/32768*16;
}

float yAcc(){
  return (float)JY901.stcAcc.a[1]/32768*16;
}

float zAcc(){
  return (float)JY901.stcAcc.a[2]/32768*16;
}

// collect gyroscope acceleration data
float xGyro(){
  return (float)JY901.stcGyro.w[0]/32768*2000;
}

float yGyro(){
  return (float)JY901.stcGyro.w[1]/32768*2000;
}

float zGyro(){
  return (float)JY901.stcGyro.w[2]/32768*2000;
}

// collect angle data
float xAngle(){
  return (float)JY901.stcAngle.Angle[0]/32768*180;
}

float yAngle(){
  return (float)JY901.stcAngle.Angle[1]/32768*180;
}

float zAngle(){
  return (float)JY901.stcAngle.Angle[2]/32768*180;
}
