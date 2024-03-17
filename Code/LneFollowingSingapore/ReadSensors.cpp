#include "Arduino.h"
#include "Functions.h"

VL53L0X laser;
const byte irReceiverPins[6] = {A10, A8, A15, A9, A13, A11};
const byte irDiodePins[6] = {43, 45, 37, 47, 39, 41};
uint16_t previousDistance;
uint16_t distance;
uint16_t laserDistance;
const byte touchPins[4] = {33, 35};
bool touchValues[4] = {1, 1, 1, 1};
int irValues[6];
byte irValuesMapped[6];
int calibratedMin[6] = {217, 268, 185, 196, 259, 267};
int calibratedMax[6] = {885, 893, 950, 862, 908, 958};

void updateCalibration() {
  readSensors(1, 0);
  for (byte j = 0; j < 6; j++) {
    calibratedMin[j] = min(calibratedMin[j], irValues[j]);
    calibratedMax[j] = max(calibratedMax[j], irValues[j]);
  }
}

void calibrateSensors(bool spin) {
  if (spin) {
    for (byte i = 0; i < 6; i++) calibratedMin[i] = 1200;
    for (byte i = 0; i < 6; i++) calibratedMax[i] = 0;
    runMotors(maxMotorFrequency-200, maxMotorFrequency-200, 250);
    for (byte spinCycle = 0; spinCycle < 200; spinCycle++) updateCalibration();
    runMotors(stopMotorFrequency, stopMotorFrequency, 500);
    runMotors(minMotorFrequency+200, minMotorFrequency+200, 250);
    for (byte spinCycle = 0; spinCycle < 200; spinCycle++) updateCalibration();
  }
  Serial.print(F("int calibratedMin[6] = {"));
  for (byte i = 0; i < 6; i++) {
    Serial.print(calibratedMin[i]);
    Serial.print(i < 5 ? F(", ") : F("};\nint calibratedMax[6] = {"));
  }
  for (byte i = 0; i < 6; i++) {
    Serial.print(calibratedMax[i]);
    Serial.print(i < 5 ? F(", ") : F("};\n"));
  }
  mode = 's';
}

void readSensors(bool print, bool readLasers) {
  for (byte i = 0; i < 6; i++) {
    digitalWrite(irDiodePins[i], HIGH);
    delayMicroseconds(500);
    int noiseSignal = analogRead(irReceiverPins[i]);
    digitalWrite(irDiodePins[i], LOW);
    delayMicroseconds(500);
    int noise = analogRead(irReceiverPins[i]);
    irValues[i] = noiseSignal - noise;
    irValuesMapped[i] = constrain(map(irValues[i], calibratedMin[i], calibratedMax[i], 0, 200), 0, 255);
    if (print) {
      Serial.print(F("Ir "));
      Serial.print(i + 1);
      Serial.print(F(" = "));
      Serial.print(irValuesMapped[i]);
      Serial.print(F(", "));
    }
  } 
  for(byte i = 0; i < 2; i++) touchValues[i] = digitalRead(touchPins[i]);
  if(print) {
    Serial.print(F("Touch 1 = "));
    Serial.print(touchValues[0]);
    Serial.print(F(", Touch 2 = "));
    Serial.print(touchValues[2]);
    if(readLasers) {
      Serial.print(F(", Laser = "));
      Serial.println(distance);
    }
    else Serial.print(F("\n"));
  }
  if(readLasers) previousDistance = distance;
  if(readLasers && laser.readRangeNoBlocking(laserDistance)) distance = laserDistance;
}