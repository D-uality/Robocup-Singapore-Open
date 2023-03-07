#include "ModeSelector.h"
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver motor = Adafruit_PWMServoDriver(0x40);

void setup() {
  Serial.begin(115200);
  motor.begin();
  motor.setPWMFreq(MotorFrequency);
}

void Mode() {
  String IncomingString = "";

  if(Serial.available() > 0) {
    Serial.print("Reading Input");
    IncomingString = Serial.readString();

    if(IncomingString == "s") mode = 0;
    if(IncomingString == "g") mode = 1;
    if(IncomingString == "t") mode = 2;
    if(IncomingString == "c") mode = 3;
    if(IncomingString == "r") mode = 4;
  }
}

void Run(int v1, int v2, int d) {
  motor.setPin(DriveMotors[0], MID+v1);
  motor.setPin(DriveMotors[1], MID-v2-StraightWeight);
  motor.setPin(DriveMotors[2], MID+v1);
  motor.setPin(DriveMotors[3], MID-v2-StraightWeight);

  delay(d);

  if(RunOutput == 1) {
		char Buffer[100];
		sprintf(Buffer, "\t\t %d %d", MID+v1, MID-v1);
		Serial.print(Buffer);
	}
}

void ReadColourSensors() {
  for(int SensorNum=0; SensorNum<6; SensorNum++) {
    ColourSensorValues[SensorNum] = analogRead(ColourSensors[SensorNum]);
    if(SensorNum==0 || SensorNum==1 || SensorNum==2) ColourSensorValues[SensorNum] = map(ColourSensorValues[SensorNum], 0, 1023, 1023, 0);
  }

  if(ReadColourSensorsOutput == 1) {
    char Buffer[100];
		sprintf(Buffer, "\t\tTL:%d\tCL:%d\tBL:%d\tTR:%d\tCR:%d\tBR:%d", ColourSensorValues[0], ColourSensorValues[1], ColourSensorValues[2], ColourSensorValues[3], ColourSensorValues[4], ColourSensorValues[5]);
		Serial.print(Buffer);
  }
}