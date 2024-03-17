#include "Functions.h"

Servo frontLeft, backLeft, frontRight, backRight, claw, est;
const int stopMotorFrequency = 1500;
const int maxMotorFrequency = 2400;
const int minMotorFrequency = 600;
uint16_t currentDuration = 0;
bool sensorConditionMet = 0;

void runMotors(int leftSpeed, int rightSpeed, int duration) {
  frontLeft.write(constrain(leftSpeed, minMotorFrequency, maxMotorFrequency));
  backLeft.write(constrain(leftSpeed, minMotorFrequency, maxMotorFrequency));
  frontRight.write(constrain(rightSpeed, minMotorFrequency, maxMotorFrequency));
  backRight.write(constrain(rightSpeed, minMotorFrequency, maxMotorFrequency));
  delay(duration);
}

void runWhile(int leftSpeed, int rightSpeed, byte sensorToMeasure, bool moreThan, int sensorValue, uint16_t duration) {
  sensorConditionMet = 0;
  currentDuration = 0;
  bool laserLessThan = 0;
  bool laserMoreThan = 0;
  bool touchIsEqual = 0;
  bool irLessThan = 0;
  bool irMoreThan = 0;
  do {  
    if(currentDuration < 65534) currentDuration++;
    if(sensorToMeasure > 8) readSensors(1, 1);
    else readSensors(1, 0);
    laserLessThan = (sensorToMeasure > 8 && !moreThan && distance < sensorValue);
    laserMoreThan = (sensorToMeasure > 8 && moreThan && distance > sensorValue);
    touchIsEqual = (sensorToMeasure < 9 && !moreThan && sensorToMeasure > 6 && touchValues[sensorToMeasure-7] == sensorValue);
    irLessThan = (sensorToMeasure < 7 && !moreThan && irValuesMapped[sensorToMeasure-1] < sensorValue);
    irMoreThan = (sensorToMeasure < 7 && moreThan && irValuesMapped[sensorToMeasure-1] > sensorValue);
    runMotors(leftSpeed, rightSpeed, 0);
    if(laserLessThan || laserMoreThan || touchIsEqual || irLessThan || irMoreThan) sensorConditionMet = 1;
    else sensorConditionMet = 0;
  } while ((laserLessThan || laserMoreThan || touchIsEqual || irLessThan || irMoreThan) && currentDuration < duration);
}