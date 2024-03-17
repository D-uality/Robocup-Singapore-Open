#include <Wire.h>

const int ArduinoAddress = 0x08;
int Mode = 0;
int EvacZoneTrigger = 1;
int ServoControls[4] = {0, 0, 0, 0};

#include <Servo.h>

const int ServoPins[4] = {10, 11, 9, 12};
const int ClawPin = 4;
Servo Servos[4];
Servo Claw;

const int TouchSensors[2] = {33, 35};

#include <VL53L0X.h>
VL53L0X Laser;
int Distance;
uint16_t LaserDistance;
uint8_t HighByte, LowByte;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Wire.begin(ArduinoAddress);
    Wire.onReceive(ReceiveEvent);
    Wire.onRequest(RequestEvent);

    // 0 - Arduino LineFollowing
    // 1 - RPI Evacuation

    for(int i=0; i<4; i++) {
        Servos[i].attach(ServoPins[i]);
		Servos[i].write(90);
    }

	Claw.attach(ClawPin);
	Claw.write(150);

    Laser.init();
    Laser.setTimeout(500);
    Laser.setMeasurementTimingBudget(33000);
}

void ReceiveEvent(int Length) {
    int Buffer = Wire.read();

    if(Length ==  1) {
        return;
    }

	Mode = Wire.read();
	Serial.print("Reading RPI: ");
	Serial.print(Mode);
    
    if(Mode == 1) {
        ServoControls[0] = Wire.read();
        ServoControls[1] = Wire.read();
        ServoControls[2] = Wire.read();
        ServoControls[3] = Wire.read();

        char Buffer[40];
        sprintf(Buffer, "    %d %d %d %d", ServoControls[0], ServoControls[1], ServoControls[2], ServoControls[3]);
		Serial.print(Buffer);
    }

	Serial.println();
}

void RequestEvent() {
    Serial.print("Sending RPI: ");

    if(Mode == 0) {
        Wire.write(EvacZoneTrigger);
        Serial.println(EvacZoneTrigger);
    }

    else if(Mode == 1) {
        byte Data[2] = {11, 12};
        Wire.write(Data, 2);
    }
}

void Run(int v1, int v2, int d=2) {
    Servos[0].write(v1);
    Servos[1].write(v1);
    Servos[2].write(v2);
    Servos[3].write(v2);
    
    if(d > 0) {
		delay(d * 1000);

		Servos[0].write(90);
		Servos[1].write(90);
		Servos[2].write(90);
		Servos[3].write(90);
    }
}

void Mech(int pos) {
	Claw.write(pos);
}

void Split16ToBytes(uint16_t Num, uint8_t &HighByte, uint8_t &LowByte) {
    HighByte = (Num >> 8) & 0xFF;
    LowByte = Num & 0xFF;
}

void loop() {
    Mode = 1;
    if(Mode == 1) {
        Run(ServoControls[0], ServoControls[1], ServoControls[2]);
		Mech(ServoControls[3]);


        if(Laser.readRangeNoBlocking(LaserDistance)) Distance = LaserDistance;
        // Serial.print(Distance);
        // Serial.print("   ");
        // Split16ToBytes(Distance, HighByte, LowByte);
        // Serial.print(HighByte);
        // Serial.print(" ");
        // Serial.println(LowByte);
    }
}