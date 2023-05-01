#include <Arduino.h>
#include "ModeSelector.h"
#include "Robot.h"

extern cRobot Robot;

void Reading() {
  Serial.print("    Reading    |    ");
  
  Run(0, 0);

  int ColourSensorValues[6];
  Robot.ReadColourSensors(ColourSensorValues, 0);
  Serial.print("\t");
  Robot.ReadColourSensors(ColourSensorValues, 1);
  Serial.print("\t");
  Robot.ReadColourSensors(ColourSensorValues, 2);

  delay(50);
}