#include <Arduino.h>
#include "Robot.h"
#include "ModeSelector.h"

const int ColourSensorPins[6] = {A0, A1, A2, A3, A4, A5};
const int ServoPins[4] = {13, 12, 11, 10};

cRobot Robot(ColourSensorPins);
Servo TopLeft;
Servo BottomLeft;
Servo TopRight;
Servo BottomRight;

int Mode = 0;

void setup() {
  Serial.begin(115200);
  
  TopLeft.attach(13);
  BottomLeft.attach(12);
  TopRight.attach(11);
  BottomRight.attach(10);
}

void loop() {
  while(Serial.available() > 0) {
    String IncomingString = Serial.readString();

    if(IncomingString == "s") Mode = 0;
    if(IncomingString == "g") Mode = 1;
    if(IncomingString == "c") Mode = 2;
  }

  if(Mode == 0) {
    Serial.print("Stopped\t\t");
    Robot.InitializeCalibrationNumbers();
    Run(0, 0);
  } else if(Mode == 1) {
    Running();
  } else if(Mode == 2) {
    Mode = Robot.Calibrate();
  }

  Serial.println();
}