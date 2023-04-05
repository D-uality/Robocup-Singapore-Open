#include <Arduino.h>
#include "Robot.h"
#include "ModeSelector.h"

extern cRobot Robot;

const int Kp = 1;
const int Ki = 0;
const int Kd = 0;
const int v = 30;

int Error = 0;
int Integral = 0;
int Derivative = 0;
int LastError = 0;

void Running() {
  Serial.print("Running\t\t");

  int ColourSensorValues[6];
  Robot.ReadColourSensors(ColourSensorValues, 1);

  PID(ColourSensorValues, v, Kp, Ki, Kd);
}

void PID(int ColourSensorValues[], int v, float Kp, float Ki, float Kd) {
  Error = (ColourSensorValues[0] + ColourSensorValues[1] + ColourSensorValues[2]) - (ColourSensorValues[3] + ColourSensorValues[4] + ColourSensorValues[5]);
  Integral += Error;
  Derivative = Error - LastError;

  int Turn = Kp*Error + Ki*Integral + Kd*Derivative;
  int LeftSide = v + Turn;
  int RightSide = v - Turn;

  Run(LeftSide, RightSide);

  LastError = Error;
}