#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>
#include <Servo.h> 
#include <Wire.h>
#include <VL53L0X.h>

extern byte mode;

extern VL53L0X laser;
extern uint16_t previousDistance;
extern uint16_t distance;
extern const byte touchPins[4];
extern const byte irReceiverPins[6];
extern const byte irDiodePins[6];
extern int irValues[6];
extern const byte touchPins[4];
extern bool touchValues[4];
extern bool touchSensorValues[4];
extern int calibratedMin[6];
extern int calibratedMax[6];
extern byte irValuesMapped[6];
void readSensors(bool print, bool readLaser); 
void calibrateSensors(bool spin);

extern Servo frontLeft, backLeft, frontRight, backRight, claw, est; 
extern const int stopMotorFrequency;
extern const int maxMotorFrequency;
extern const int minMotorFrequency;
extern uint16_t currentDuration;
extern bool sensorConditionMet;
void runMotors(int leftSpeed, int rightSpeed, int duration);
void runWhile(int leftSpeed, int rightSpeed, byte sensorToMeasure, bool moreThan, int sensorValue, uint16_t duration);

void running();
void detectGreenIntersections();
bool checkGreen(byte irPin, byte min, byte max);
void avoidObstacle(byte leftSpeed, byte rightSpeed, byte setLaserDistance, float sensitivity);
void followLine(float errorMultiplier, float irFrontBias, float irInnerDampen, float irOuterBias, int leftBaseSpeed, int rightBaseSpeed); 

void test();

#endif