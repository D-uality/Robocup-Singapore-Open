 #include "Functions.h"

int Mode = 0;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    COM.begin(115200);

    Laser.init();
	Laser.setTimeout(500);
	Laser.setMeasurementTimingBudget(33000);

    pinMode(TouchPins[0], INPUT_PULLUP);
	pinMode(TouchPins[1], INPUT_PULLUP);

    for(int i=0; i<4; i++) { 
        Servos[i].attach(ServoPins[i]); 
        Servos[i].write(90);
    }
    
    Claw.attach(4);
    // Claw.writeMicroseconds(Open);
    // delay(1500);
    // ClawIncrement(Closed);
    // delay(1500);
    // ClawIncrement(CheckLift);
    // delay(1500);
    // ClawIncrement(PartialLift);
    // delay(1500);
    Claw.writeMicroseconds(Lifted);
}

void loop() {
    if(Mode == 0) {

        Serial.println("Waiting for start.");
        if(COM.available() > 0) Mode = COM.read();

    }

    else if(Mode == 1) {

        Serial.println("Line Following");

        for(int i=0; i<3; i++) {
            Serial.println(i);
            COM.println("1");
			delay(500);
        }

        Mode = 2;
        COM.println("2");
    }

    else if(Mode == 2) {
        Terminal();
    }

    else {
        Serial.println("asdkfjlksd");
        Mode = 0;
    }
}