#ifndef ModeSelector
#define ModeSelector

extern const int DriveMotors[4];
extern const int ColourSensors[6];

extern const int MotorFrequency;
extern const int MID;
extern const int MIN;
extern const int MAX;
extern const int StraightWeight;
extern const int LowerBound;
extern const int UpperBound;

extern int mode;
extern int ColourSensorValues[6];
extern int CalibrationNumbers[6][2];

extern const int RunOutput;
extern int ReadColourSensorsOutput;
extern const int PIDOutput;

void Mode();
  void Run(int v1, int v2, int d=1);
  void ReadColourSensors();
void Running();
  void PID(int v, float Kp, float Ki, float Kd);
  void Intersection(int side);
void Testing();
void Calibrating();
  void InitaliseCalibrationNumbers();
  void CalibrateColourSensorValues(int LowerBound, int UpperBound);
  void Calibrate();
void Reading();

#endif