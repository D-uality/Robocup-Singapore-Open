#include "HardwareSerial.h"
#include "ModeSelector.h"
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

void Testing() {
  Serial.print("Testing\t");

  Run(250, 250, 350);
  Run(0, 0, 0);
  Run(250, -250, 1800);

  ReadColourSensors();
  CalibrateColourSensorValues(LowerBound, UpperBound);

  while(ColourSensorValues[0] > 70) {
    ReadColourSensors();
    CalibrateColourSensorValues(LowerBound, UpperBound);
  }

  Run(0, 0);
  while(Serial.available() == 0) {}
}

// void MotorTesting() {
//   motor.setPin(DriveMotors[0], 500);
//   motor.setPin(DriveMotors[1], 0);
//   motor.setPin(DriveMotors[2], 0);
//   motor.setPin(DriveMotors[3], 0);

//   delay(1000);

//   motor.setPin(DriveMotors[0], 0);
//   motor.setPin(DriveMotors[1], 500);
//   motor.setPin(DriveMotors[2], 0);
//   motor.setPin(DriveMotors[3], 0);
  
//   delay(1000);

//   motor.setPin(DriveMotors[0], 0);
//   motor.setPin(DriveMotors[1], 0);
//   motor.setPin(DriveMotors[2], 500);
//   motor.setPin(DriveMotors[3], 0);

//   delay(1000);

//   motor.setPin(DriveMotors[0], 0);
//   motor.setPin(DriveMotors[1], 0);
//   motor.setPin(DriveMotors[2], 0);
//   motor.setPin(DriveMotors[3], 500);

//   delay(1000);
// }