#include <Joystick.h>

int speedPin = 7;
int steerPin = 3;


unsigned long speedUpTime = 0;
unsigned long steerUpTime = 0;
unsigned long speedDuration = 1500;
unsigned long steerDuration = 1500;
int triggered = 0;

int xMin = 2000;
int xMax = 1000;

int yMin = 2000;
int yMax = 1000;

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  0, 0,                  // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering


void setup() {  
  /*  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  */
  Joystick.begin();
  Joystick.setXAxisRange(xMin, xMax);
  Joystick.setYAxisRange(yMin, yMax);  

  pinMode(speedPin, INPUT_PULLUP);
  pinMode(steerPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(speedPin), speed, CHANGE);
  attachInterrupt(digitalPinToInterrupt(steerPin), steer, CHANGE);
}

void speed() {
  unsigned long currentTime = micros();
  if (digitalRead(speedPin)) {
    speedUpTime = currentTime;
  } else {
    speedDuration = currentTime - speedUpTime;
    triggered++;
  }
}

void steer() {
  unsigned long currentTime = micros();
  if (digitalRead(steerPin)) {
    steerUpTime = currentTime;
  } else {
    steerDuration = currentTime - steerUpTime;
    triggered++;
  }
}

 
void loop() {
  if (triggered) {
    if (speedDuration < xMin && speedDuration >= 1000) {
      xMin = speedDuration;
      Joystick.setXAxisRange(xMin, xMax);
    }
    if (steerDuration < yMin && steerDuration > 1000) {
      yMin = steerDuration;
      Joystick.setYAxisRange(yMin, yMax);
    }

    if (speedDuration > xMax && speedDuration <= 2000) {
      xMax = speedDuration;
      Joystick.setXAxisRange(xMin, xMax);
    }
    if (steerDuration > yMax && steerDuration <= 2000) {
      yMax = steerDuration;
      Joystick.setYAxisRange(yMin, yMax);
    }

    Joystick.setXAxis(speedDuration);
    Joystick.setYAxis(steerDuration);
    Joystick.sendState();
    /*
    Serial.print(speedDuration);
    Serial.print(" ");
    Serial.print(steerDuration);
    Serial.print(" Triggers: ");
    Serial.println(triggered);
    */
    triggered = 0;
    //delay(1000);
  }
  // Nothing needed
}