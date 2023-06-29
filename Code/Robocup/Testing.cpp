#include <Arduino.h>
#include "Robot.h"
#include "ModeSelector.h"

extern cRobot Robot;
extern int ProgramTick;

void AvoidObstacle();

void Testing() {
  Serial.print("Testing    |    ");

  int JoyStickValues[2];
  Robot.ReadJoyStick(JoyStickValues);

  if(JoyStickValues[0] != 0 || JoyStickValues[1] != 0)  AvoidObstacle();
  else                                                  Run(30, 30);

}

void AvoidObstacle() {
  Run(-30, -30, 500);
  Run(-30, 30, 1200);

  int Offset = 10;
  int JoyStickValues[2];
  Robot.ReadJoyStick(JoyStickValues);

  while(JoyStickValues[0] == 0 && JoyStickValues[1] == 0) {
    Robot.ReadJoyStick(JoyStickValues);
    Run(30, 30 - Offset);
    delay(200);

    Offset += 2;
    if(Offset > 40) Offset = 40;
  }
}