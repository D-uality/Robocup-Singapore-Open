#include <Arduino.h>
#include "Robot.h"
#include "ModeSelector.h"

const int ColourSensorPins[6] = {A0, A1, A2, A3, A4, A5};
const int ServoPins[4] = {31, 29, 27, 25};
const int JoyStickPins[2] = {A6, A7};

cRobot Robot(ColourSensorPins, JoyStickPins);
Servo TopLeft;
Servo BottomLeft;
Servo TopRight;
Servo BottomRight;

int Mode = 0;
int ProgramTick = 0;

void setup() {
  Serial.begin(115200);
  
  TopLeft.attach(ServoPins[0]);
  BottomLeft.attach(ServoPins[1]);
  TopRight.attach(ServoPins[2]);
  BottomRight.attach(ServoPins[3]);
}

void loop() {
  char Buffer[40];
  sprintf(Buffer, "%d    |    ", ProgramTick);
  Serial.print(Buffer);

  while(Serial.available() > 0) {
    String IncomingString = Serial.readString();

    if(IncomingString == "s") Mode = 0;
    if(IncomingString == "g") Mode = 1;
    if(IncomingString == "c") Mode = 2;
    if(IncomingString == "r") Mode = 3;
    if(IncomingString == "t") Mode = 4;
  }

  if(Mode == 0) {
    Serial.print("Stopped    |    ");
    Robot.InitializeCalibrationNumbers();
    Run(0, 0);
  } else if(Mode == 1) {
    Running();
    ProgramTick += 1;
  } else if(Mode == 2) {
    Mode = Robot.Calibrate();
  } else if(Mode == 3) {
    Reading();
  } else if(Mode == 4) {
    Testing();
    ProgramTick += 1;
  }

  Serial.println();
  delay(2);
}