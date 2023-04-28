
#ifndef Lidar_h
#define Lidar_h

#include <Wire.h>

// used to request data from the lidar
uint8_t request_data[5] = {0x5A, 0X05, 0X00, 0X01, 0X60};

// stores data pulled from lidar
uint8_t data_frame[9];

struct LidarData {
  uint16_t Distance;
  uint16_t Strength; 
  uint16_t Temperature;
};

class Lidar {

  public:

    // save the i2c address of the lidar
    void Initialize(uint8_t address){
      i2c_address = address;
    }

    // ask lidar for data
    LidarData Fetch() {

      Wire.beginTransmission(i2c_address);
      Wire.write(request_data, (uint8_t)5);
      Wire.endTransmission();
      delay(100);

      Wire.requestFrom(i2c_address, (uint8_t)9);

      while(Wire.available()) {
        Wire.readBytes(data_frame, (uint8_t)9);
      }

      return interpretData();
    }

    // return distance in cm
    uint16_t Distance() {
      return distance;
    }

    // return strength of signal, higher is better
    uint16_t Strength() {
      return strength;
    }

    // return temperature
    uint16_t Temperature() {
      return (temperature / 8) - 256;
    }

  private:

    uint16_t distance;
    uint16_t strength;
    uint16_t temperature;
    uint8_t i2c_address;

    // read data packet into variables
    LidarData interpretData() {

      distance = data_frame[3] << 8 | data_frame[2];
      strength = data_frame[5] << 8 | data_frame[4];
      temperature = data_frame[7] << 8 | data_frame[6];

      LidarData data = { distance, strength, temperature };
      return data;
    }
};

#endif