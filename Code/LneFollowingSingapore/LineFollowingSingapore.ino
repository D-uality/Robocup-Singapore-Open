#include "Functions.h"

byte mode = 'g';
const byte arduinoAddress = 0x08;
int ServoControls[4] = {0, 0, 0, 0};


void setup() { 
  Serial.begin(115200); 
  Wire.begin();
  frontLeft.attach(11); 
  backLeft.attach(12); 
  frontRight.attach(9); 
  backRight.attach(5);
  claw.attach(4);
  for(byte i = 0; i < 6; i++) {
    pinMode(irReceiverPins[i], INPUT); 
    pinMode(irDiodePins[i], OUTPUT);
    if(i < 4) pinMode(touchPins[i], INPUT_PULLUP);
  }
  laser.init();
  laser.setTimeout(500);
  laser.setMeasurementTimingBudget(33000);
  for(byte i = 0; i < 10; i++) readSensors(0, 1);

  Wire.begin(arduinoAddress);
  // Wire.onReceive(ReceiveEvent);
  // Wire.onRequest(RequestEvent);
}

// void ReceiveEvent(int Length) {
//   int Buffer = Wire.read();

//   if(Length ==  1) {
//     return;
//   }

//   Mode = Wire.read();
//   Serial.print("Reading RPI: ");
//   Serial.print(Mode);
  
//   if(Mode == 1) {
//     ServoControls[0] = Wire.read();
//     ServoControls[1] = Wire.read();
//     ServoControls[2] = Wire.read();
//     ServoControls[3] = Wire.read();

//     char Buffer[40];
//     sprintf(Buffer, "    %d %d %d %d", ServoControls[0], ServoControls[1], ServoControls[2], ServoControls[3]);
//     Serial.print(Buffer);
//   }

//   Serial.println();
// }

// void RequestEvent() {
//   Serial.print("Sending RPI: ");

//   if(Mode == 0) {
//     Wire.write(EvacZoneTrigger);
//     Serial.println(EvacZoneTrigger);
//   }
// }

void loop() { 
  if (Serial.available() > 0) { 
    Serial.println(F("Reading for instructions"));
    mode = Serial.read();
    while(Serial.available() > 0) Serial.read();
    Serial.println(mode);
  }
  switch(mode) {
    case 'g': running(); break;
    case 'c': calibrateSensors(1); break;
    case 'r': 
      runMotors(stopMotorFrequency, stopMotorFrequency, 0);
      readSensors(1, 1); 
      break;
    case 0: runMotors(stopMotorFrequency, stopMotorFrequency, 0); break;
    case 't': test(); break;
    case 's': 
      Serial.println(F("---------- Command Line ----------"));
      Serial.println(F("g = Main"));
      Serial.println(F("c = Calibration"));
      Serial.println(F("r = Read Sensors"));
      Serial.println(F("s = Stop"));
      Serial.println(F("t = Test"));
      calibrateSensors(0);
      mode = 0;
      break;
  }
}