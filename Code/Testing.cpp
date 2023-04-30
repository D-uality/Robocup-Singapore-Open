#include <Arduino.h>
#include "Robot.h"
#include "ModeSelector.h"

extern cRobot Robot;

int v = 30;

void Testing() {
  Serial.print("    Testing    |    ");

  int Forwards = 1.5*v;
  int Backwards = -v;

  Run(Backwards, Forwards, 1500);

  Run(Forwards, Backwards, 1500);
}