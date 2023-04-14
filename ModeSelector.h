#include "Arduino.h"
#ifndef MODESELECTOR_H
#define MODESELECTOR_H

void Running();
  void PID(int ColourSensorValues[], int v, float Kp, float Ki, float Kd);
  void Intersection(int Type);
  void IntersectionTurn(int Turn);
void Testing();

void Reading();

#endif
