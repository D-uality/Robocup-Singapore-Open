#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include <EEPROM.h>
#include <Servo.h>

class cRobot {
  private:
    int ColourSensorPins[6];
    int CalibrationNumbers[6][2];
    int JoyStickPins[2];
  public:
    cRobot(int pColourSensorPins[], int pJoyStickPins[]);
    void ReadColourSensors(int pColourSensorValues[], int pCalibratedState);
    void ReadJoyStick(int pJoyStickValues[]);
    void InitializeCalibrationNumbers();
    int Calibrate();
};

void Run(int v1, int v2, int d=2);
void RunUntil(int v1, int v2, int ColourSensorPin, int ColourValue);

#endif
