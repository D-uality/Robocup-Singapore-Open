#include "HardwareSerial.h"
#include "ModeSelector.h"
#include <Arduino.h>

int CheckLeftSideGreen();
int CheckRightSideGreen();
void IntersectionTurning(int Turn);

int Integral, LastError, LeftMotor, RightMotor;

void Running() {
  Serial.print("Running\t");

  ReadColourSensors();
  CalibrateColourSensorValues(LowerBound, UpperBound);
  
  if(ColourSensorValues[0] < 5 && ColourSensorValues[3] < 5)    Intersection(0);
  else if(ColourSensorValues[0] < 5)                              Intersection(1);
  else if(ColourSensorValues[3] < 5)                              Intersection(2);
  else                                                            PID(500, 10, 0, 0);
}

void SpeedGuard() {
  if (LeftMotor  > MAX-MID) LeftMotor  = MAX-MID;	
 	if (RightMotor > MAX-MID) RightMotor = MAX-MID;
 	if (LeftMotor  < MIN-MID) LeftMotor  = MIN-MID;
  if (RightMotor < MIN-MID) RightMotor = MIN-MID;

	if(LeftMotor > 0) 	 LeftMotor = map(LeftMotor,     0, 800,   60, 800);
	else			           LeftMotor = map(LeftMotor,  -800,   0, -800, -60);
	if(RightMotor > 0) 	RightMotor = map(RightMotor,    0, 800,   60, 800);
	else			          RightMotor = map(RightMotor, -800,   0, -800, -60);
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
  int Turn = -1;
  Run(0, 0, 200);

  int LeftSide = CheckLeftSideGreen();
  int RightSide = CheckRightSideGreen();

  if(side == 0) {
    // Cross Intersection

    char Buffer[40];
    sprintf(Buffer, "Checking both sides: %d and %d", LeftSide, RightSide);
    Serial.println();
    Serial.println(Buffer);

    if(LeftSide == 1)                     Turn = 1;
    if(RightSide == 1)                    Turn = 2;
    if(LeftSide == 1 && RightSide == 1)   Turn = 3;
    if(LeftSide == 0 && RightSide == 0)   Turn = 0;
  } else if (side == 1) {
    // T Intersection left

    char Buffer[40];
    sprintf(Buffer, "Checking left sides: %d", LeftSide);
    Serial.println();
    Serial.println(Buffer);

    if(LeftSide == 1)         Turn = 1;
    else                      Run(-300, 300, 300);

  } else if (side == 2) {
    // T Intersection right

    char Buffer[40];
    sprintf(Buffer, "Checking right sides: %d", RightSide);
    Serial.println();
    Serial.println(Buffer);

    if(RightSide == 1)        Turn = 2;
    else                      Run(300, -300, 300);

  }

  if(Turn == 1) {
    Run(0, 0, 100);
    Run(300, 300, 600);
    Run(0, 0, 100);

    Run(-300, 300, 2300);
    Run(0, 0, 100);

    Run(300, 300, 600);
  } else if(Turn == 2) {
    Run(0, 0, 100);
    Run(300, 300, 600);
    Run(0, 0, 100);

    Run(300, -300, 2300);
    Run(0, 0, 100);

    Run(300, 300, 600);
  }
}

int CheckLeftSideGreen() {
  int LeftSide = 0;
  if(ColourSensorValues[1] <= 90 && ColourSensorValues[2] <= 90) {
    LeftSide = 1;
  }
  return LeftSide;
}

int CheckRightSideGreen() {
  int RightSide = 0;
  if(ColourSensorValues[4] <= 90 && ColourSensorValues[5] <= 90) {
    RightSide = 1;
  }
  return RightSide;
}

void IntersectionTurning(int Turn) {
  if(Turn == 1) {
    Run(-200, 500, 2000);
  }
}