#include <Arduino.h>
#include "Robot.h"
#include "ModeSelector.h"

extern cRobot Robot;
extern int ProgramTick;

const float Kp = 1.5;
const float Ki = 0;
const float Kd = 10;
const int v = 30;

int Error = 0;
int Integral = 0;
int Derivative = 0;
int LastError = 0;
int Errors[50];

void Running() {
  Serial.print("    Running    |    ");

  int ColourSensorValues[6];
  Robot.ReadColourSensors(ColourSensorValues, 1);

  if(ColourSensorValues[0] < 18 && ColourSensorValues[3] < 18 && ProgramTick > 150 && Errors[49] < 100)                         Intersection(0);
  else if(ColourSensorValues[0] < 10 && ColourSensorValues[3] < 40 && ColourSensorValues[4] < 50 && ProgramTick > 150)          Intersection(1);
  else if(ColourSensorValues[0] < 40 && ColourSensorValues[1] < 50 && ColourSensorValues[3] < 10 && ProgramTick > 150)          Intersection(2);
  else                                                                                                                          PID(ColourSensorValues, v, Kp, Ki, Kd);
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

  Errors[ProgramTick % 50] = Error;

  char Buffer[100];
  sprintf(Buffer, "        Error %d    Integral %d    Derivative %d    LeftSide %d    RightSide %d", Error, Integral, Derivative, LeftSide, RightSide);
  Serial.print(Buffer);
}

void Intersection(int Type) {
  int LeftSideGreen = 0;
  int RightSideGreen = 0;
  int Turn = 3;

  Serial.print("\t\t");
  Run(0, 0, 100);
  int ColourSensorValues[6];
  Robot.ReadColourSensors(ColourSensorValues, 2);

  if(Type == 0) {
    if(ColourSensorValues[2] < 940)   {LeftSideGreen   = 1;  Turn = 1;}
    if(ColourSensorValues[5] < 940)   {RightSideGreen  = 1;  Turn = 2;}
    if(LeftSideGreen == 1 && RightSideGreen == 1)            Turn = 0;
  }

  if(Type == 1) {
    if(ColourSensorValues[1] < 940) {LeftSideGreen = 1; Turn = 1;}
  }

  if(Type == 2) {
    if(ColourSensorValues[4] < 940) {RightSideGreen = 1; Turn = 2;}
  }

  char Buffer[40];
  sprintf(Buffer, "        Type: %d %d|%d = %d", Type, LeftSideGreen, RightSideGreen, Turn);
  Serial.print(Buffer);

  IntersectionTurn(Turn);
  ProgramTick = 0;
}

void IntersectionTurn(int Turn) {
  if(Turn == 0) {                          // 180 Turn
    Run(-40, 50, 3000);
    RunUntil(-40, 50, 3, 70);
  } else if(Turn == 1) {                   // Left Turn
    Run(-30, 50, 1000);
    RunUntil(-30, 50, 3, 70);
  } else if(Turn == 2) {                   // Right Turn
    Run(50, -40, 1000);
    RunUntil(50, -40, 0, 70);
  }
  
  Run(0, 0, 100);
}