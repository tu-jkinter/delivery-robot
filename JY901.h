#ifndef JY901_h
#define JY901_h

#include <Wire.h>

#define SAVE 			  0x00
#define CALSW 		  0x01
#define RSW 			  0x02
#define RRATE			  0x03
#define BAUD 			  0x04
#define AXOFFSET	  0x05
#define AYOFFSET	  0x06
#define AZOFFSET	  0x07
#define GXOFFSET	  0x08
#define GYOFFSET	  0x09
#define GZOFFSET	  0x0a
#define HXOFFSET	  0x0b
#define HYOFFSET	  0x0c
#define HZOFFSET	  0x0d
#define D0MODE		  0x0e
#define D1MODE		  0x0f
#define D2MODE		  0x10
#define D3MODE		  0x11
#define D0PWMH		  0x12
#define D1PWMH		  0x13
#define D2PWMH		  0x14
#define D3PWMH		  0x15
#define D0PWMT		  0x16
#define D1PWMT		  0x17
#define D2PWMT		  0x18
#define D3PWMT		  0x19
#define IICADDR		  0x1a
#define LEDOFF 		  0x1b
#define GPSBAUD		  0x1c

#define YYMM				0x30
#define DDHH				0x31
#define MMSS				0x32
#define MS					0x33
#define AX					0x34
#define AY					0x35
#define AZ					0x36
#define GX					0x37
#define GY					0x38
#define GZ					0x39
#define HX					0x3a
#define HY					0x3b
#define HZ					0x3c			
#define Roll				0x3d
#define Pitch				0x3e
#define Yaw					0x3f
#define TEMP				0x40
#define D0Status		0x41
#define D1Status		0x42
#define D2Status		0x43
#define D3Status		0x44
#define PressureL		0x45
#define PressureH		0x46
#define HeightL			0x47
#define HeightH			0x48
#define LonL				0x49
#define LonH				0x4a
#define LatL				0x4b
#define LatH				0x4c
#define GPSHeight   0x4d
#define GPSYAW      0x4e
#define GPSVL				0x4f
#define GPSVH				0x50
      
#define DIO_MODE_AIN    0
#define DIO_MODE_DIN    1
#define DIO_MODE_DOH    2
#define DIO_MODE_DOL    3
#define DIO_MODE_DOPWM  4
#define DIO_MODE_GPS    5		

struct STime {
	unsigned char ucYear;
	unsigned char ucMonth;
	unsigned char ucDay;
	unsigned char ucHour;
	unsigned char ucMinute;
	unsigned char ucSecond;
	unsigned short usMiliSecond;
};

struct SAcc {
	short a[3];
	short T;
};

struct SGyro {
	short w[3];
	short T;
};

struct SAngle {
	short Angle[3];
	short T;
};

struct SMag {
	short h[3];
	short T;
};

struct SDStatus {
	short sDStatus[4];
};

struct SPress {
	long lPressure;
	long lAltitude;
};

struct SLonLat {
	long lLon;
	long lLat;
};

struct SGPSV {
	short sGPSHeight;
	short sGPSYaw;
	long lGPSVelocity;
};

class JY901 {
  
  public: 

    struct STime		stcTime;
    struct SAcc 		stcAcc;
    struct SGyro 		stcGyro;
    struct SAngle 	stcAngle;
    struct SMag 		stcMag;
    struct SDStatus stcDStatus;
    struct SPress 	stcPress;
    struct SLonLat 	stcLonLat;
    struct SGPSV 		stcGPSV;
	 
    //Constructor
    JY901 () {
      ucDevAddr = 0x50;
    } 

    void StartIIC() {
        ucDevAddr = 0x50;
	      Wire.begin();
    }

    void StartIIC(unsigned char ucAddr) {
        ucDevAddr = ucAddr;
	      Wire.begin();
    }

    void CopeSerialData(unsigned char ucData) {

      static unsigned char ucRxBuffer[250];
      static unsigned char ucRxCnt = 0;	
      
      ucRxBuffer[ucRxCnt++] = ucData;

      if (ucRxBuffer[0] != 0x55) 
      {
        ucRxCnt = 0;
        return;
      }

      if (ucRxCnt < 11) {
        return;
      } 
      else {
        switch(ucRxBuffer[1]) {
          case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8); break;
          case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8); break;
          case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8); break;
          case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8); break;
          case 0x54:	memcpy(&stcMag,&ucRxBuffer[2],8); break;
          case 0x55:	memcpy(&stcDStatus,&ucRxBuffer[2],8); break;
          case 0x56:	memcpy(&stcPress,&ucRxBuffer[2],8); break;
          case 0x57:	memcpy(&stcLonLat,&ucRxBuffer[2],8); break;
          case 0x58:	memcpy(&stcGPSV,&ucRxBuffer[2],8); break;
        }

        ucRxCnt = 0;
      }
    }

    short ReadWord(unsigned char ucAddr) {
      short sResult;
      readRegisters(ucDevAddr, ucAddr, 2, (char *)&sResult);
      return sResult;
    }

    void WriteWord(unsigned char ucAddr,short sData) {
        writeRegister(ucDevAddr, ucAddr, 2, (char *)&sData);
    }

    void ReadData(unsigned char ucAddr,unsigned char ucLength,char chrData[]) {
        readRegisters(ucDevAddr, ucAddr, ucLength, chrData);
    }

    void GetTime() {
        readRegisters(ucDevAddr, 0x30, 8, (char*)&stcTime);
    }

    void GetAcc() {
        readRegisters(ucDevAddr, AX, 6, (char *)&stcAcc);
    }

    void GetGyro() {
        readRegisters(ucDevAddr, GX, 6, (char *)&stcGyro);
    }

    void GetAngle() {
        readRegisters(ucDevAddr, Roll, 6, (char *)&stcAngle);
    }

    void GetMag() {
        readRegisters(ucDevAddr, HX, 6, (char *)&stcMag);
    }

    void GetPress() {
        readRegisters(ucDevAddr, PressureL, 8, (char *)&stcPress);
    }

    void GetDStatus() {
        readRegisters(ucDevAddr, D0Status, 8, (char *)&stcDStatus);
    }

    void GetLonLat() {
        readRegisters(ucDevAddr, LonL, 8, (char *)&stcLonLat);
    }

    void GetGPSV() {
        readRegisters(ucDevAddr, GPSHeight, 8, (char *)&stcGPSV);
    }
	
  private: 

    unsigned char ucDevAddr; 

    void readRegisters(unsigned char deviceAddr,unsigned char addressToRead, unsigned char bytesToRead, char * dest) {

        Wire.beginTransmission(deviceAddr);
        Wire.write(addressToRead);
        Wire.endTransmission(false); //endTransmission but keep the connection active

        Wire.requestFrom(deviceAddr, bytesToRead); //Ask for bytes, once done, bus is released by default

        while(Wire.available() < bytesToRead); //Hang out until we get the # of bytes we expect

        for(int x = 0; x < bytesToRead; x++)
          dest[x] = Wire.read(); 
    }

    void writeRegister(unsigned char deviceAddr,unsigned char addressToWrite,unsigned char bytesToRead, char *dataToWrite) {

        Wire.beginTransmission(deviceAddr);
        Wire.write(addressToWrite);
        for(int i = 0; i < bytesToRead; i++)
          Wire.write(dataToWrite[i]);

        Wire.endTransmission(); //Stop transmitting
    }

};

#endif