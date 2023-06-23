#include <Arduino.h>
#include "Robot.h"
#include "ModeSelector.h"

extern cRobot Robot;
extern int ProgramTick;

void AvoidObstacle();

void Testing() {
  // Serial.print("    Testing    |    ");

  // int JoyStickValues[2];
  // Robot.ReadJoyStick(JoyStickValues);

  // if(JoyStickValues[0] == 1 && ProgramTick > 200) {
  //   AvoidObstacle();
  // } else {
  //   ProgramTick += 1;
  //   Run(-30, -30);
  // }

  for(int i=0; i<30; i+=2) {
    for(int j=0; j<100; j++) {
      char Buffer[40];
      sprintf(Buffer, "      %d %d", (-30-i), (-30+i));
      Serial.println(Buffer);

      Run(-30-i, -30+i);
      delay(1);

      int JoyStickValues[2];
      Robot.ReadJoyStick(JoyStickValues);

      if(JoyStickValues[0] == 1 || JoyStickValues[1] == -1) break;
    }
  }
}

void AvoidObstacle() {
  Run(30, 30, 800);
  Run(30, -30, 1000);
  Run(0, 0, 1000);

  for(int i=0; i<30; i++) {
    Run(-30-i, -30+i);
    
    int JoyStickValues[2];
    Robot.ReadJoyStick(JoyStickValues);

    char Buffer[40];
    sprintf(Buffer, "        %d %d", -30-1, -30+i);
    Serial.print(Buffer);

    if(JoyStickValues[0] == 1 && JoyStickValues[1] == -1) break;
  }
}