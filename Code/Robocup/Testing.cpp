#include <Arduino.h>
#include "Robot.h"
#include "ModeSelector.h"

extern cRobot Robot;

void AvoidObstacle();

void Testing() {
  Serial.print("    Testing    |    ");

  int JoyStickValues[2];
  Robot.ReadJoyStick(JoyStickValues);

  if(JoyStickValues[1] == 1) {
    AvoidObstacle();
  } else {
    Run(30, 30);
  }
}

void AvoidObstacle() {
  Run(0, 0, 1000);
  Run(-30, -30, 1300);
  Run(0, 0, 300);
  Run(-30, 50, 1000);
  Run(0, 0, 1000);
}