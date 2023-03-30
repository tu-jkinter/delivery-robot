#include <Wire.h>

uint8_t data_frame[9];

struct Data{
	uint16_t distance;
	uint16_t strength;
	uint16_t temperature;
};

class Lidar{
public:
void initializeLidar(){
	Wire.begin();
}

void fetchData(uint8_t address){
	Wire.beginTransmission(address);
	Wire.write(request_data, 5);
	Wire.endTransmission();
	delay(100);
	Wire.requestFrom(address, 9);
	while(Wire.available()){
		Wire.readBytes(data_frame, 9);
	}
	interpretData();
}

void printData(){
	Serial.print("distance: ");    Serial.print(distance,DEC);    Serial.print("        ");
	Serial.print("strength: ");    Serial.print(strength,DEC);    // Serial.print("        ");
//	Serial.print("temperature: "); Serial.print(temperature,DEC);
	Serial.println();
}


private:
uint8_t checksum(uint8_t data[9]){
	uint8_t sum;
	for(uint8_t i = 0; i < 9; i++){
		sum += data_frame[i];
	}
	return sum;
}

Data interpretData(){
	distance = data_frame[3] << 8 | data_frame[2];
	strength = data_frame[5] << 8 | data_frame[4];
//	temperature = data_frame[7] << 8 | data_frame[6]; temperature = (temperature/8) - 256;
}
}
