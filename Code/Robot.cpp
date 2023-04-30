#include "Arduino.h"
#include "Robot.h"

extern Servo TopLeft;
extern Servo BottomLeft;
extern Servo TopRight;
extern Servo BottomRight;
extern cRobot Robot;

cRobot::cRobot(int pColourSensorPins[]) {
  for(int i=0; i<6; i++) {
    ColourSensorPins[i] = pColourSensorPins[i];
    pinMode(ColourSensorPins[i], INPUT);
  }
}

void cRobot::ReadColourSensors(int pColourSensorValues[], int pCalibratedState) {
  for(int i=0; i<6; i++) {
    pColourSensorValues[i] = analogRead(ColourSensorPins[i]);
    if(i == 0 || i == 1 || i==2) pColourSensorValues[i] = map(pColourSensorValues[i], 0, 1023, 1023, 0);

    if(pCalibratedState == 1) {
      pColourSensorValues[i] = map(pColourSensorValues[i], CalibrationNumbers[i][0], CalibrationNumbers[i][1], 0, 100);
    }
    else if(pCalibratedState == 2) {
      pColourSensorValues[i] = map(pColourSensorValues[i], CalibrationNumbers[i][0], CalibrationNumbers[i][1], 0, 1023);
    }
  }

  char Buffer[50];
  sprintf(Buffer, "%d %d %d %d %d %d", pColourSensorValues[0], pColourSensorValues[1], pColourSensorValues[2], pColourSensorValues[3], pColourSensorValues[4], pColourSensorValues[5]);
  Serial.print(Buffer);
}

void cRobot::InitializeCalibrationNumbers() {
  for(int i=0; i<6; i++) {
    for(int j=0; j<2; j++) {
      CalibrationNumbers[i][j] = EEPROM.read(4*i + 2*j)*256 + EEPROM.read(4*i + 2*j + 1);
    }
  }

  char Buffer[100];
  sprintf(Buffer, "%d %d %d %d %d %d        %d %d %d %d %d %d", CalibrationNumbers[0][0], CalibrationNumbers[1][0], CalibrationNumbers[2][0], CalibrationNumbers[3][0], CalibrationNumbers[4][0], CalibrationNumbers[5][0], CalibrationNumbers[0][1], CalibrationNumbers[1][1], CalibrationNumbers[2][1], CalibrationNumbers[3][1], CalibrationNumbers[4][1], CalibrationNumbers[5][1]);

  Serial.print(Buffer);
}

int cRobot::Calibrate() {
  int ColourSensorValues[6];
  int ColourSensorValuesExtremites[6][2];
  
  for(int i=0; i<6; i++) {
    ColourSensorValuesExtremites[i][0] = 512;
    ColourSensorValuesExtremites[i][1] = 512;
  }

  int Bytes3D[6][2][2];
  int Bytes1D[24];
  int CalibrationMode = 1;
  
  char Buffer[100];

  while(CalibrationMode != 0) {
    while(Serial.available() > 0) {
      String IncomingString;
      IncomingString = Serial.readString();

      if(IncomingString == "e") CalibrationMode = 0;
      if(IncomingString == "w") CalibrationMode = 2;
      else                      CalibrationMode = 1;
    }

    if(CalibrationMode == 1) {
      Serial.print("    Calibrating    |    ");

      for(int i=0; i<6; i++) {
        ColourSensorValues[i] = analogRead(ColourSensorPins[i]);
        if(i == 0 || i == 1 || i==2) ColourSensorValues[i] = map(ColourSensorValues[i], 0, 1023, 1023, 0);
        if(ColourSensorValues[i] < ColourSensorValuesExtremites[i][0]) ColourSensorValuesExtremites[i][0] = ColourSensorValues[i];
        if(ColourSensorValues[i] > ColourSensorValuesExtremites[i][1]) ColourSensorValuesExtremites[i][1] = ColourSensorValues[i];
      }

      sprintf(Buffer, "%d %d %d %d %d %d        %d %d %d %d %d %d", ColourSensorValuesExtremites[0][0], ColourSensorValuesExtremites[1][0], ColourSensorValuesExtremites[2][0], ColourSensorValuesExtremites[3][0], ColourSensorValuesExtremites[4][0], ColourSensorValuesExtremites[5][0], ColourSensorValuesExtremites[0][1], ColourSensorValuesExtremites[1][1], ColourSensorValuesExtremites[2][1], ColourSensorValuesExtremites[3][1], ColourSensorValuesExtremites[4][1], ColourSensorValuesExtremites[5][1]);
      Serial.println(Buffer);
    }

    if(CalibrationMode == 2) {
      for(int i=0; i<6; i++) {
        for(int j=0; j<2; j++) {
          int Value = ColourSensorValuesExtremites[i][j];
          int HighByte = (Value >> 8) & 0xFF;
          int LowByte = Value & 0xFF;

          sprintf(Buffer, "Value: %d, HighByte: %d, LowByte: %d", Value, HighByte, LowByte);
          Serial.println(Buffer);

          Bytes3D[i][j][0] = HighByte;
          Bytes3D[i][j][1] = LowByte;
        }
      }

      for(int i=0; i<6; i++) {
        for(int j=0; j<2; j++) {
          for(int k=0; k<2; k++) {
            sprintf(Buffer, "Bytes3D[%d][%d][%d] = %d", i, j, k, Bytes3D[i][j][k]);
            Serial.println(Buffer);

            Bytes1D[i*4 + j*2 + k] = Bytes3D[i][j][k];
          }
        }
      }

      for(int i=0; i<24; i++) {
        sprintf(Buffer, "EEPROM.write(%d, %d)", i, Bytes1D[i]);
        Serial.println(Buffer);

        EEPROM.write(i, Bytes1D[i]);
      }

      CalibrationMode = 0;

      delay(1000);
    }
  }

  return 0;
}

void Run(int v1, int v2, int d=2) {
  v1 = map(constrain(v1, -100, 100), -100, 100, -60, 60);
  v2 = map(constrain(v2, -100, 100), -100, 100, -60, 60);

  TopLeft.write(90+v1);
  BottomLeft.write(90+v1);
  TopRight.write(90-v2);
  BottomRight.write(90-v2);

  delay(d);
}

void RunUntil(int v1, int v2, int ColourSensorPin, int ColourValue) {
  int ColourSensorValues[6];
  do {
    Robot.ReadColourSensors(ColourSensorValues, 1);
    Run(v1, v2);
  } while(ColourSensorValues[ColourSensorPin] > ColourValue);
}