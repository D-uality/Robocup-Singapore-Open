#include <Wire.h>

const int ArduinoAddress = 0x08;
int Mode = 0;
int EvacZoneTrigger = 1;
int ServoControls[4] = {0, 0, 0, 0};

#include <Servo.h>

const int ServoPins = [10, 11, 9, 12];
Servo Servos[4];

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    // 0 - Arduino LineFollowing
    // 1 - RPI Evacuation

    Wire.begin(ArduinoAddress);
    Wire.onReceive(ReceiveEvent);
    Wire.onRequest(RequestEvent);

    for(int i=0; i<4; i++) {
        Servos[i].attach(ServoPins[i]);
    }
}

void ReceiveEvent(int Length) {
    int Buffer = Wire.read();

    if(Length ==  1) {
        return;
    }

    Mode = Wire.read();
    Serial.print("Reading RPI: ");
    Serial.println(Mode);
    
    if(Mode == 1) {
        ServoControls[0] = Wire.read();
        ServoControls[1] = Wire.read();
        ServoControls[2] = Wire.read();
        ServoControls[3] = Wire.read();
    }
    
}

void RequestEvent() {
    Serial.print("Sending RPI: ");

    if(Mode == 0) {
        Wire.write(EvacZoneTrigger);
        Serial.println(EvacZoneTrigger);
    }
    if(Mode == 1) {
        
    }
}

void Run(int v1, int v2, int d=2) {
    
}

void loop() {

}