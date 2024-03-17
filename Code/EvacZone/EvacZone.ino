#include <Wire.h>
#include 

const int ArduinoAddress = 0x08;
int Mode = 0;

void setup() {
    Serial.begin(115200);
	Wire.begin(ArduinoAddress);
	Wire.onRequest(RequestEvent);
  	Wire.onReceive(ReceiveEvents);

}

void loop() {
	if(Mode == 0) {
		delay(1);
	}

	else if(Mode == 1) {
		// Start Line Follow
    	delay(1);
	}
}

void RequestEvent() {
	Serial.print("Sending to RPI: ");

	if(Mode == 1) {
		Mode += 1;
		Serial.println(Mode);

		Wire.write(Mode);
	}

	else if(Mode == 2) {
		uint8_t Data[12] = {1, 2, 1, 2, 3, 4, 1, 2, 3, 4, 5, 6};
		
		Wire.write(Data, 12);

		for(int i=0; i<12; i++) {
			Serial.print(Data[i]);
			Serial.print(" ");
		}

		Serial.println();
	}
}

void ReceiveEvents(int Length) {
	
	if(Length == 1) { return; }
	int Buffer = Wire.read();

	Serial.print("Received from RPI: ");
	Serial.println(Mode);

	Mode = Wire.read();
}