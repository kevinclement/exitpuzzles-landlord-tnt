#include "Arduino.h"
#include "conditions.h"

MyLightSensor::MyLightSensor(Conditions &conditions)
  : _conditions(conditions)
{  
}

void MyLightSensor::setup() {
}

// Main loop function
void MyLightSensor::handle() {
  bool light = analogRead(LS_PIN) < LS_THRESHOLD;
  if (light && !sawLight) {
    sawLight = true;
    _conditions.lightSensed();
  }
}

void MyLightSensor::teardown() {
}
