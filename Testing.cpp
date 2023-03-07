#include "HardwareSerial.h"
#include "ModeSelector.h"
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

void Testing() {
  Serial.print("Testing\t");


  Run(0, 0, 100);
  Run(300, 300, 600);
  Run(0, 0, 100);

  Run(-300, 300, 2300);
  Run(0, 0, 100);

  Run(300, 300, 600);
  Run(0, 0, 5000);
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