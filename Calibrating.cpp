#include "ModeSelector.h"
#include <EEPROM.h>
#include <Arduino.h>

void Calibrating() {
  Serial.print("Calibrating\t");
  Run(0, 0);
  Calibrate();
}

void InitaliseCalibrationNumbers() {
  int InputNumbers[6][2][2];

  for (int SensorNum = 0; SensorNum < 6; SensorNum++) {
    for (int Extremity = 0; Extremity < 2; Extremity++) {
      for (int i = 0; i < 2; i++) {
        InputNumbers[SensorNum][Extremity][i] = EEPROM.read(4 * SensorNum + 2 * Extremity + i);
      }
      InputNumbers[SensorNum][Extremity][1] = InputNumbers[SensorNum][Extremity][1] * 256;
      CalibrationNumbers[SensorNum][Extremity] = InputNumbers[SensorNum][Extremity][0] + InputNumbers[SensorNum][Extremity][1];
    }
  }
}

void CalibrateColourSensorValues(int LowerBound, int UpperBound) {
  for (int SensorNum = 0; SensorNum < 6; SensorNum++) {
    ColourSensorValues[SensorNum] = map(ColourSensorValues[SensorNum], CalibrationNumbers[SensorNum][1], CalibrationNumbers[SensorNum][0], LowerBound, UpperBound);
  }

  if (ReadColourSensorsOutput == 2) {
    char Buffer[100];
    sprintf(Buffer, "\t\tTL:%d\tCL:%d\tBL:%d\tTR:%d\tCR:%d\tBR:%d", ColourSensorValues[0], ColourSensorValues[1], ColourSensorValues[2], ColourSensorValues[3], ColourSensorValues[4], ColourSensorValues[5]);
    Serial.print(Buffer);
  }
}

void Calibrate() {
  int ColourSensorExtremity[6][2] = { { 512, 512 }, { 512, 512 }, { 512, 512 }, { 512, 512 }, { 512, 512 }, { 512, 512 } };
  int bits[6][2][10] = { { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } } };
  int bytes[6][2][2][8] = {
    { { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 } }, { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 } } },
    { { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 } }, { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 } } },
    { { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 } }, { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 } } },
    { { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 } }, { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 } } },
    { { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 } }, { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 } } },
    { { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 } }, { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 } } },
  };
  int OutNum[6][2][2] = { { { 0, 0 }, { 0, 0 } }, { { 0, 0 }, { 0, 0 } }, { { 0, 0 }, { 0, 0 } }, { { 0, 0 }, { 0, 0 } }, { { 0, 0 }, { 0, 0 } }, { { 0, 0 }, { 0, 0 } } };
  char Buffer[100];
  String IncomingByte = "";
  while (mode == 3) {
    Serial.print("Calibrating");
    if (Serial.available() > 0) {
      Serial.print("Reading BYTE");
      IncomingByte = Serial.readString();
      if (IncomingByte == "e") {
        mode = 0;
      } else if (IncomingByte == "w") {
        Run(0, 0);
        Serial.println();
        for (int SensorNum = 0; SensorNum < 6; SensorNum++) {
          for (int Extremity = 0; Extremity < 2; Extremity++) {

            for (int bit = 9; bit >= 0; bit--) {
              bits[SensorNum][Extremity][9 - bit] = (ColourSensorExtremity[SensorNum][Extremity] & (1 << bit)) != 0;
            }

            bytes[SensorNum][Extremity][1][6] = bits[SensorNum][Extremity][0];
            bytes[SensorNum][Extremity][1][7] = bits[SensorNum][Extremity][1];
            bytes[SensorNum][Extremity][0][0] = bits[SensorNum][Extremity][2];
            bytes[SensorNum][Extremity][0][1] = bits[SensorNum][Extremity][3];
            bytes[SensorNum][Extremity][0][2] = bits[SensorNum][Extremity][4];
            bytes[SensorNum][Extremity][0][3] = bits[SensorNum][Extremity][5];
            bytes[SensorNum][Extremity][0][4] = bits[SensorNum][Extremity][6];
            bytes[SensorNum][Extremity][0][5] = bits[SensorNum][Extremity][7];
            bytes[SensorNum][Extremity][0][6] = bits[SensorNum][Extremity][8];
            bytes[SensorNum][Extremity][0][7] = bits[SensorNum][Extremity][9];
            for (int i = 0; i < 2; i++) {
              for (int j = 0; j < 8; j++) {
                OutNum[SensorNum][Extremity][i] += (1 << j) * bytes[SensorNum][Extremity][i][7 - j];
              }
            }
            sprintf(Buffer, "\t\t\t%d = %d%d%d%d%d%d%d%d%d%d = %d%d%d%d%d%d%d%d + %d%d%d%d%d%d%d%d = %d + %d", ColourSensorExtremity[SensorNum][Extremity], bits[SensorNum][Extremity][0], bits[SensorNum][Extremity][1], bits[SensorNum][Extremity][2], bits[SensorNum][Extremity][3], bits[SensorNum][Extremity][4], bits[SensorNum][Extremity][5], bits[SensorNum][Extremity][6], bits[SensorNum][Extremity][7], bits[SensorNum][Extremity][8], bits[SensorNum][Extremity][9], bytes[SensorNum][Extremity][0][0], bytes[SensorNum][Extremity][0][1], bytes[SensorNum][Extremity][0][2], bytes[SensorNum][Extremity][0][3], bytes[SensorNum][Extremity][0][4], bytes[SensorNum][Extremity][0][5], bytes[SensorNum][Extremity][0][6], bytes[SensorNum][Extremity][0][7], bytes[SensorNum][Extremity][1][0], bytes[SensorNum][Extremity][1][1], bytes[SensorNum][Extremity][1][2], bytes[SensorNum][Extremity][1][3], bytes[SensorNum][Extremity][1][4], bytes[SensorNum][Extremity][1][5], bytes[SensorNum][Extremity][1][6], bytes[SensorNum][Extremity][1][7], OutNum[SensorNum][Extremity][0], OutNum[SensorNum][Extremity][1]);
            Serial.println(Buffer);
          }
        }
        for (int SensorNum = 0; SensorNum < 6; SensorNum++) {
          for (int Extremity = 0; Extremity < 2; Extremity++) {
            for (int i = 0; i < 2; i++) {
              EEPROM.write(4 * SensorNum + 2 * Extremity + i, OutNum[SensorNum][Extremity][i]);
            }
          }
          Serial.println();
        }
        mode = 0;
        delay(3000);
      }
    } else {
      for (int SensorNum = 0; SensorNum < 6; SensorNum++) {
        ColourSensorValues[SensorNum] = analogRead(ColourSensors[SensorNum]);
        if (SensorNum == 0 || SensorNum == 1 || SensorNum == 2) ColourSensorValues[SensorNum] = map(ColourSensorValues[SensorNum], 0, 1023, 1023, 0);

        if (ColourSensorValues[SensorNum] > ColourSensorExtremity[SensorNum][0]) ColourSensorExtremity[SensorNum][0] = ColourSensorValues[SensorNum];  // Check if there is a new maximum
        if (ColourSensorValues[SensorNum] < ColourSensorExtremity[SensorNum][1]) ColourSensorExtremity[SensorNum][1] = ColourSensorValues[SensorNum];  // Check if there is a new minimum
      }
      sprintf(Buffer, "\t\t%d %d %d %d %d %d", ColourSensorExtremity[0][0], ColourSensorExtremity[1][0], ColourSensorExtremity[2][0], ColourSensorExtremity[3][0], ColourSensorExtremity[4][0], ColourSensorExtremity[5][0]);
      Serial.print(Buffer);
      sprintf(Buffer, "\t\t%d %d %d %d %d %d", ColourSensorExtremity[0][1], ColourSensorExtremity[1][1], ColourSensorExtremity[2][1], ColourSensorExtremity[3][1], ColourSensorExtremity[4][1], ColourSensorExtremity[5][1]);
      Serial.println(Buffer);
    }
  }
}