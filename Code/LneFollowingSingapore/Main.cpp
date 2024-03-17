#include "Arduino.h"
#include "Functions.h"

byte mainLoopCount = 250;
byte blackThreshold = 20;
byte silverThreshold = 220;
bool leftBlackWithRight = 0;
bool rightBlackWithLeft = 0;
bool doubleBlack = 0;
bool leftGreen = 0;
bool rightGreen = 0;
byte greenRanges[4][2] = {
  {170-20, 170+10}, 
  {80, 200},
  {130, 200}, 
  {180-20, 180+5},
};

void running() {
  if(mainLoopCount < 250) mainLoopCount++;
  readSensors(1, 0);
  leftBlackWithRight = irValuesMapped[0] < blackThreshold && irValuesMapped[1] < 195;
  rightBlackWithLeft = irValuesMapped[1] < blackThreshold && irValuesMapped[0] < 190;
  doubleBlack = irValuesMapped[0] < blackThreshold+30 && irValuesMapped[1] < blackThreshold+30;
  leftGreen = irValuesMapped[2] > greenRanges[0][0] && irValuesMapped[2] < greenRanges[0][1] && irValuesMapped[3] > greenRanges[1][0] && irValuesMapped[3] < greenRanges[1][1];
  rightGreen = irValuesMapped[4] > greenRanges[2][0] && irValuesMapped[4] < greenRanges[2][1] && irValuesMapped[5] > greenRanges[3][0] && irValuesMapped[5] < greenRanges[3][1];
  if(irValuesMapped[0] > silverThreshold && irValuesMapped[1] > silverThreshold && mainLoopCount > 50) {
    Serial.print("Terminal: ");
    runWhile(stopMotorFrequency-200, stopMotorFrequency-400, 2, 1, 50, 75); 
    if(sensorConditionMet) {
      Serial.println("Found");
      runMotors(stopMotorFrequency+200, stopMotorFrequency+400, currentDuration);
    } else {
      runWhile(stopMotorFrequency+200, stopMotorFrequency+400, 1, 1, 50, 150);
      if(sensorConditionMet) {
        Serial.println("Found");
        runMotors(stopMotorFrequency-200, stopMotorFrequency-400, currentDuration);
      } else runWhile(stopMotorFrequency-400, stopMotorFrequency-200, 2, 1, 50, 75);
      if(sensorConditionMet) {
        Serial.println("Found");
        runMotors(stopMotorFrequency+200, stopMotorFrequency+400, currentDuration);
      } else {
        Serial.println("None");
        runMotors(stopMotorFrequency-200, stopMotorFrequency-400, currentDuration);
        mainLoopCount = 0;
      }
    }
    runMotors(stopMotorFrequency, stopMotorFrequency, 4000);
  }
  else if(((leftBlackWithRight && leftGreen) || (rightBlackWithLeft && rightGreen)) && mainLoopCount > 50) detectGreenIntersections();
  else if(touchValues[0] == 0 && touchValues[1] == 0) avoidObstacle(90+40, 90-40, 30, 0.9);
  else followLine(2, 1, 1, 2, 400, 337);
}

void detectGreenIntersections() {
  mainLoopCount = 50;
  Serial.print(F("Green Intersections: "));
  if (doubleBlack && leftGreen && rightGreen) {
    Serial.println(F("Double"));
    runMotors(stopMotorFrequency-500, stopMotorFrequency-500, 900);
    runWhile(stopMotorFrequency-400, stopMotorFrequency-400, 2, 1, 160, 0);
  } else if (leftBlackWithRight && leftGreen) {
    Serial.println(F("Left"));
    runMotors(stopMotorFrequency+500, stopMotorFrequency-500, 150);
    runMotors(stopMotorFrequency-300, stopMotorFrequency-500, 300);
    runWhile(stopMotorFrequency-200, stopMotorFrequency-400, 2, 1, 180, 0);
  } else if (rightBlackWithLeft && rightGreen) {
    Serial.println(F("Right"));
    runMotors(stopMotorFrequency+500, stopMotorFrequency-500, 150);
    runMotors(stopMotorFrequency+500, stopMotorFrequency+300, 300);
    runWhile(stopMotorFrequency+400, stopMotorFrequency+200, 1, 1, 180, 0);
  }
}

void followObstacle(byte leftSpeed, byte rightSpeed, byte setLaserDistance, float sensitivity) {
  bool irSeenBlack[6] = {false, false, false, false, false, false};
  while(irSeenBlack[0] == 0 || irSeenBlack[1] == 0 || irSeenBlack[2] == 0 || irSeenBlack[3] == 0 || irSeenBlack[4] == 0 || irSeenBlack[5] == 0) {
    readSensors(1, 1);
    if(irValuesMapped[0] < blackThreshold && !irSeenBlack[0]) irSeenBlack[0] = true;
    if(irValuesMapped[1] < blackThreshold && !irSeenBlack[1]) irSeenBlack[1] = true;
    if(irValuesMapped[2] < blackThreshold && !irSeenBlack[2]) irSeenBlack[2] = true;
    if(irValuesMapped[3] < blackThreshold && !irSeenBlack[3]) irSeenBlack[3] = true;
    if(irValuesMapped[4] < blackThreshold && !irSeenBlack[4]) irSeenBlack[4] = true;
    if(irValuesMapped[5] < blackThreshold && !irSeenBlack[5]) irSeenBlack[5] = true;
    int distanceError = sensitivity * (setLaserDistance - distance);
    Serial.println(distanceError);
    runMotors(leftSpeed - distanceError, rightSpeed - distanceError/2, 0);
  }
}

void avoidObstacle(byte leftSpeed, byte rightSpeed, byte setLaserDistance, float sensitivity) {
  Serial.print(F("Obstacle Detected\n"));
  runMotors(stopMotorFrequency-500, stopMotorFrequency+500, 200);
  runMotors(stopMotorFrequency+500, stopMotorFrequency+500, 300);
  byte distanceMoreThanPreviousCounter = 0;
  do {
    runMotors(90+20, 90+20, 0);
    readSensors(1, 1);
    if(previousDistance >= distance && distance < 150) distanceMoreThanPreviousCounter++;
    else distanceMoreThanPreviousCounter = 0;
  } while (distanceMoreThanPreviousCounter < 9);
  runMotors(90+60, 90-80, 300);
  followObstacle(leftSpeed, rightSpeed, setLaserDistance, sensitivity);
  runWhile(stopMotorFrequency+400, stopMotorFrequency+300, 1, 1, 80, 0);
}

void followLine(float errorMultiplier, float irFrontBias, float irInnerDampen, float irOuterBias, int leftBaseSpeed, int rightBaseSpeed) {
  int lineError = errorMultiplier * (irFrontBias*irValuesMapped[0] + irInnerDampen*irValuesMapped[3] + irOuterBias*irValuesMapped[2] - irFrontBias*irValuesMapped[1] - irInnerDampen*irValuesMapped[4] - irOuterBias*irValuesMapped[5]);
  if((irValuesMapped[0] < blackThreshold+30 && irValuesMapped[2] < blackThreshold+30) || (irValuesMapped[1] < blackThreshold+30 && irValuesMapped[5] < blackThreshold+30)) lineError = -lineError;
  runMotors(stopMotorFrequency + leftBaseSpeed + lineError, stopMotorFrequency - rightBaseSpeed + lineError, 0);
}