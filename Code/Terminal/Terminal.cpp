#include "Arduino.h"
#include "Functions.h"

const int Open = 500;
const int Closed = 1000;
const int CheckLift = 1260;
const int PartialLift = 1650;
const int Lifted = 2100;

int VictimCount = 0;

void Terminal() {
	float kP = 0.3, kD = 3;

	int Offset = 100;
    Distance = Laser.readRangeSingleMillimeters();
    int LastError = Offset - Distance;

    unsigned long StartTime = millis();

	while(true) {
        if(Laser.readRangeNoBlocking(LaserDistance)) Distance = LaserDistance;

        int Error = Offset - Distance;
        int Derivative = Error - LastError;
        int Turn = constrain(Error * kP + Derivative * kD, -10, 10);

        char Buffer[40];
        sprintf(Buffer, "%d %d %d    |    %d %d %d", Distance, Error, Derivative, Turn, 20 + Turn, 20 - Turn);
        Serial.println(Buffer);

        LastError = Error;

        unsigned long EndTime = millis();

        if(EndTime - StartTime > 300) {
            Run(0, 0, 800);
            StartTime = millis();
        } else {
            Run(20 - Turn, 20 + Turn);
        }

        if(digitalRead(TouchPins[1]) == 0) {
            // Turn 45
            Run(-20, -20, 500);
            Run(-20, 20, 1000);
        }

		if(COM.available() > 0) {
            MoveCloser();
		}
    }
}

void MoveCloser() {
    Run(0, 0, 100);

    int x, y = 0;
    int Offset =  150;
    float kP = 0.2;
    int yReference = 45;
    unsigned long StartTime = millis();
    unsigned long EndTime = millis()+100;
    Serial.println(EndTime-StartTime);

    while(true) {
        while(COM.available() > 0) {
			String Data = COM.readString();
            // Serial.println(Data);
			String Type;

			int Comma1 = Data.indexOf(",");
			int Comma2 = Data.indexOf(",", Comma1 + 1);

			x    = Data.substring(0, Comma1).toInt();
			y    = Data.substring(Comma1+1, Comma2).toInt();
			Type = Data.substring(Comma2+1);

            StartTime = EndTime = millis();

            // Serial.print("Data Received:    ");
			// Serial.print(x);
			// Serial.print(" ");
			// Serial.print(y);
			// Serial.print(" ");
			// Serial.println(Type);
		}
        
        if(y > yReference) {
            Grab();
        }

        if(EndTime - StartTime > 6000) break;

        int Error = Offset - x;
        int Turn = constrain(Error * kP, -10, 10);

        char Buffer[40];
        sprintf(Buffer, "%d %d    |    %d %d %d", x, y, Error, Turn, 20 + Turn, 20 - Turn);
        Serial.println(Buffer);

        Run(20-Turn, 20+Turn, 300);
        Run(0, 0);

        while(COM.available() == 0) {
            EndTime = millis();
            Serial.println(EndTime-StartTime);
            if(EndTime - StartTime > 6000) break;
        }
    }
}

void Grab() {
    Serial.println("Grabbing Victim");
    
    Claw.writeMicroseconds(Open);
    delay(1500);

    Run(10, 10, 2000);
    Run(0, 0);

    ClawIncrement(Closed);
    delay(500);

    Run(-10, -10, 2000);
    Run(0, 0);
    delay(1000);
    
    ClawIncrement(CheckLift);
    
    delay(2000);    

    while(COM.available() > 0) { String Data = COM.readString(); }
    COM.println("Grabbing Victim");

    while(COM.available() == 0) { delay(1); }
    while(COM.available() > 0) {
        String Message = COM.readString();
        Serial.println(Message);

        if(Message == "Ball Found") {
            VictimCount ++;
            ClawIncrement(PartialLift);

            delay(1000);

            Serial.println("Find Triangle");
            COM.println("Find Triangle");
        } else {
            ClawIncrement(Lifted);
            delay(1000);
        }
    }
}

void FindTriangle() {
    if(COM.available() > 0) {
        String Data = COM.readString();
        int xAverage = Data.toInt();
        Serial.println(xAverage);
    }
    
    Run(20, -20);
}