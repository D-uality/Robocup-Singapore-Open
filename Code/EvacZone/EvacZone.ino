#include <VL53L0X.h>
#define COM Serial1

const int TouchPins[2] = {33, 35};
int TouchValues[2] = {0, 0};

VL53L0X Laser;
uint16_t Distance;
uint16_t LaserDistance;

// -------------------------------

int Mode = 0;

// -------------------------------

void setup() {
	Serial.begin(115200);
	COM.begin(115200);

	Laser.init();
	Laser.setTimeout(500);
	Laser.setMeasurementTimingBudget(33000);

	pinMode(TouchPins[0], INPUT_PULLUP);
	pinMode(TouchPins[1], INPUT_PULLUP);
}

String GetValue(String Data, char Separator, int Index) {
	int ModeFound = 0;
	int StrIndex[] = {0, -1};
	int MaxIndex = Data.length() - 1;

	for (int i = 0; i <= MaxIndex && ModeFound <= Index; i++) {
		if (Data.charAt(i) == Separator || i == MaxIndex) {
		ModeFound++;
		StrIndex[0] = StrIndex[1] + 1;
		StrIndex[1] = (i == MaxIndex) ? i + 1 : i;
		}
	}
	
	return ModeFound > Index ? Data.substring(StrIndex[0], StrIndex[1]) : "";
}

void loop() {

	if(Mode == 0) {
		// Wait for RPI to start linefollow

		if(COM.available() > 0) {
			Serial.print("Data received:    ");
			Mode = COM.read();
			Serial.println(Mode);
		}

		Serial.println("Waiting for RPI");
		delay(1000);
	}
	
	else if(Mode == 1) {
		// Linefollow

		COM.write(Mode);
		Mode = 2;

		Serial.println("Linefollowing");
		delay(1000);
	}
	
	else if(Mode == 2) {
		// Waiting for instructions
    
		if(COM.available() > 0) {
			String Data = COM.readString();
			Serial.print("Data received:    ");
			Serial.println(Data);

			Data.remove(0, 1);
  			Data.remove(Data.length() - 1, 1);

			Mode     = GetValue(Data, ',', 0).toInt();
			int v1   = GetValue(Data, ',', 1).toInt();
			int v2   = GetValue(Data, ',', 2).toInt();

			char Buffer[100];
			sprintf(Buffer, "                  Mode: %d    v1: %d    v2: %d", Mode, v1, v2);
			Serial.println(Buffer);

            if(Laser.readRangeNoBlocking(LaserDistance)) Distance = LaserDistance;
            TouchValues[0] = digitalRead(TouchPins[0]);
            TouchValues[1] = digitalRead(TouchPins[1]);

            sprintf(Buffer, "%d %d %d", LaserDistance, TouchValues[0], TouchValues[1]);
            COM.println(Buffer);
		}
	} 

	else {
		Mode = 0;
	}
}