#include "HardwareSerial.h"
#include "ModeSelector.h"
#include <Arduino.h>

void Reading() {
  Serial.print("Reading\t");

  Run(0, 0);

  ReadColourSensorsOutput = 1;
  ReadColourSensors();
  ReadColourSensorsOutput = 2;
  CalibrateColourSensorValues(LowerBound, UpperBound);

  delay(50);
}