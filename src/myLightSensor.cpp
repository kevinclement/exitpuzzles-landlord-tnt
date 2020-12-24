#include "Arduino.h"
#include "conditions.h"

MyLightSensor::MyLightSensor(Conditions &conditions)
  : _conditions(conditions)
{  
}

void MyLightSensor::setup() {
  pinMode(LS_PIN, INPUT_PULLUP);
}

// Main loop function
void MyLightSensor::handle() {

  // int lightVal =  analogRead(LS_PIN);
  // Serial.print("light: ");
  // Serial.println(lightVal);

  bool light = analogRead(LS_PIN) < LS_THRESHOLD;
  if (light && !sawLight) {
    sawLight = true;
    _conditions.lightSensed();
  }
}

void MyLightSensor::teardown() {
}
