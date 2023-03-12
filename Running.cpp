#include "ModeSelector.h"
#include <Arduino.h>

void TurnRight();
void TurnLeft();
void TurnAround();
void CarryOn();

int Integral, LastError, LeftMotor, RightMotor;

void Running() {
  Serial.print("Running\t");

  ReadColourSensors();
  CalibrateColourSensorValues(LowerBound, UpperBound);

  if(ColourSensorValues[0] < 20 && ColourSensorValues[3] < 20)                                            Intersection(2);
  else if(ColourSensorValues[0] < 40 && ColourSensorValues[1] < 50 && ColourSensorValues[3] < 15)         Intersection(0);
  else if(ColourSensorValues[0] < 15 && ColourSensorValues[3] < 40 && ColourSensorValues[4] < 50)         Intersection(1);
  else                                                                                                    PID(Speed, Kp, Ki, Kd);
}   
 
void SpeedGuard() {
  if (LeftMotor  > MAX-MID) LeftMotor  = MAX-MID;	
 	if (RightMotor > MAX-MID) RightMotor = MAX-MID;
 	if (LeftMotor  < MIN-MID) LeftMotor  = MIN-MID;
  if (RightMotor < MIN-MID) RightMotor = MIN-MID;

	
}

void PID(int v, float Kp, float Ki, float Kd) {
  int Error = (ColourSensorValues[0] + ColourSensorValues[1] + ColourSensorValues[2]) - (ColourSensorValues[3] + ColourSensorValues[4] + ColourSensorValues[5]);
  Integral += Error;
  int Derivative = Error - LastError;

  int Turn = Kp*Error + Ki*Integral + Kd*Derivative;
  LeftMotor = v + Turn;
  RightMotor = v - Turn;

  LastError = Error;

  SpeedGuard();

  Run(LeftMotor, RightMotor);

  if(PIDOutput == 1) {
		char Buffer[100];
		sprintf(Buffer, "\t\tError:%d\tIntegral:%d\tDerivative:%d\tLMotor:%d\tRMotor:%d", Error, Integral, Derivative, LeftMotor, RightMotor);
		Serial.print(Buffer);
	}
}

void Intersection(int side) {
  Run(0, 0, 50);
  ReadColourSensors();
  CalibrateColourSensorValues(LowerBound, UpperBound);

  char Buffer[40];
  sprintf(Buffer, "\nChecking For Green for type %d", side);
  Serial.print(Buffer);

  if(side == 0) {
    if(ColourSensorValues[4] < 90) {
      sprintf(Buffer, "\n\tGreen Identified");
      Serial.print(Buffer);
      TurnRight();
    } else {
      PID(Speed, Kp, Ki, Kd);
    }
  }
  if(side == 1) {
    if(ColourSensorValues[1] < 90) {
      sprintf(Buffer, "\n\tGreen Identified");
      Serial.print(Buffer);
      TurnLeft();
    } else {
      PID(Speed, Kp, Ki, Kd);
    }
  }
  if(side == 2) {
    int LeftSide = 0, RightSide = 0, Turn = 0;

    if(ColourSensorValues[1] < 85 && ColourSensorValues[2] < 90) {
      LeftSide = 1;
      Turn = 1;
    }
    if(ColourSensorValues[4] < 85 && ColourSensorValues[5] < 90) {
      RightSide = 1;
      Turn = 2;
    }
    if(LeftSide == 1 && RightSide == 1) {
      Turn = 3;
    }

    if(Turn == 0)       CarryOn();
    else if(Turn == 1)  TurnLeft();
    else if(Turn == 2)  TurnRight();
    else if(Turn == 3)  TurnAround();

    sprintf(Buffer, "\n\tLeftSide: %d RightSide: %d", LeftSide, RightSide);
    Serial.print(Buffer);
  }
}

void TurnRight() {
  Run(250, 250, 350);
  Run(0, 0, 0);
  Run(250, -250, 1800);

  while(ColourSensorValues[0] > 70) {
    ReadColourSensors();
    CalibrateColourSensorValues(LowerBound, UpperBound);
  }
}

void TurnLeft() {
  Run(250, 250, 350);
  Run(0, 0, 0);
  Run(-250, 250, 2200);

  while(ColourSensorValues[4] > 70) {
    ReadColourSensors();
    CalibrateColourSensorValues(LowerBound, UpperBound);
  }
}

void TurnAround() {
  Run(-250, 250, 4800);

  while(ColourSensorValues[0] > 70) {
    ReadColourSensors();
    CalibrateColourSensorValues(LowerBound, UpperBound);
  }
}

void CarryOn() {
  Run(250, 250, 400);
}