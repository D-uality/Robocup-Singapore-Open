#include "Functions.h"

VL53L0X Laser;
uint16_t Distance;
uint16_t LaserDistance;

const int TouchPins[2] = {33, 35};
int TouchValues[2] = {0, 0};

Servo Servos[4];
int ServoPins[4] = {10, 11, 9, 12};
Servo Claw;

void ReadTouch() {
	TouchValues[0] = digitalRead(TouchPins[0]);
	TouchValues[1] = digitalRead(TouchPins[1]);
}

void Run(int v1, int v2, int d=2) {
    if(v1 > 0) v1 += 5;
    if(v2 < 0) v2 -= 5;

	v1 = constrain(v1, -90, 90);
	v2 = constrain(v2, -90, 90);

	Servos[0].write(90+v1);
	Servos[1].write(90+v1);
	Servos[2].write(90-v2);
	Servos[3].write(90-v2);

    delay(d);
}

void ClawIncrement(int Position) {
    int CurrentPosition = Claw.readMicroseconds();
    Serial.println(CurrentPosition);

    if(CurrentPosition > Position) {
        for(int i=CurrentPosition; i>Position; i--) {
            Claw.writeMicroseconds(i);
            delay(2);
        }
    } else {
        for(int i=CurrentPosition; i<Position; i++) {
            Claw.writeMicroseconds(i);
            delay(2);
        }
    }
}