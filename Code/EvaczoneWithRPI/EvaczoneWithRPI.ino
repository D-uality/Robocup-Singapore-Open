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

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    // 0 - Arduino LineFollowing
    // 1 - RPI Evacuation

    Wire.begin(ArduinoAddress);
    Wire.onReceive(ReceiveEvent);
    Wire.onRequest(RequestEvent);

    for(int i=0; i<4; i++) {
        Servos[i].attach(ServoPins[i]);
		Servos[i].write(90);
    }
	Claw.attach(ClawPin);
	Claw.write(90);
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
}

void Run(int v1, int v2, int d=2) {
	v1 = map(constrain(v1, -100, 100), -100, 100, -60, 60);
	v2 = map(constrain(v2, -100, 100), -100, 100, -60, 60);

	Servos[0].write(90+v1);
	Servos[1].write(90+v1);
	Servos[2].write(90-v2);
	Servos[3].write(90-v2);

	if(d > 0) {
		delay(d * 1000);

		Servos[0].write(90+v1);
		Servos[1].write(90+v1);
		Servos[2].write(90-v2);
		Servos[3].write(90-v2);
	}	
}

void Mech(int pos) {
	Claw.write(pos);
}

void loop() {
    if(Mode == 1) {
        Run(ServoControls[0], ServoControls[1], ServoControls[2]);
		Mech(ServoControls[3]);
    }
}