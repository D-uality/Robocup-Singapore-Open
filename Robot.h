#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include <EEPROM.h>
#include <Servo.h>

class cRobot {
  private:
    int ColourSensorPins[6];
    int CalibrationNumbers[6][2];

  public:
    cRobot(int pColourSensorPins[]);
    void ReadColourSensors(int pColourSensorValues[], int pCalibratedState);
    void InitializeCalibrationNumbers();
    int Calibrate();
};

void Run(int v1, int v2);

#endif
