#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Arduino.h"
#include <VL53L0X.h>
#include <Servo.h>
#define COM Serial1

extern VL53L0X Laser;
extern uint16_t Distance;
extern uint16_t LaserDistance;

extern const int TouchPins[2];
extern int TouchValues[2];

extern Servo Servos[4];
extern int ServoPins[4];
extern Servo Claw;
 
extern const int Open;
extern const int Closed;
extern const int CheckLift;
extern const int PartialLift;
extern const int Lifted;

void Terminal();
void MoveCloser();
void Grab();
void FindTriangle();
void Run(int v1, int v2, int d=2);
void ClawIncrement(int Position);

#endif