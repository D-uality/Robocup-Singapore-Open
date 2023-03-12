// ------------------------------------------------------------------------------------------------------------ INCLUDES

#include <Arduino.h>
#include "ModeSelector.h"


// ------------------------------------------------------------------------------------------------------------ VARIABLES

const int DriveMotors[4] = {0, 1, 2, 3};
const int ColourSensors[6] = {A0, A1, A2, A3, A4, A5};

const int MotorFrequency = 220;
const int MID = 1278;
const int MIN = MID-800;
const int MAX = MID+800;
const int StraightWeight = 32;
const int ProgramDelay = 2;
const int LowerBound = 0;
const int UpperBound = 100;

int mode = 0;
int ColourSensorValues[6];
int CalibrationNumbers[6][2];

const int Speed = 300;
const int Kp = 10;
const int Ki = 0;
const int Kd = 3;

const int RunOutput = 1;
int ReadColourSensorsOutput = 2;
const int PIDOutput = 1;

// ------------------------------------------------------------------------------------------------------------ LOOP

void loop() {
  Mode();

  if(mode == 0) {
    Serial.print("Stopped\t");
    InitaliseCalibrationNumbers();
    Run(0, 0);
  } else if(mode == 1) {
    Running();
  } else if(mode == 2) {
    Testing();  
  } else if(mode == 3) {
    Calibrating();
  } else if(mode == 4) {
    Reading();
  }
 
  Serial.println();
  delay(ProgramDelay);
}