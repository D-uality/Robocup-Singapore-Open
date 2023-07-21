#include <Arduino.h>
#include "Robot.h"
#include "ModeSelector.h"

extern cRobot Robot;
extern int ProgramTick;

void AvoidObstacle();

void Testing() {
  Serial.print("Testing    |    ");

  Run(15, -15, 13.3*1000);
  Run(0, 0, 3000);
}

