#include "Arduino.h"
#include "myCommunicate.h"

MyCommunicate::MyCommunicate(Conditions &conditions)
  : _conditions(conditions)
{  
}

void MyCommunicate::setup() {
  Wire.begin(); // join i2c bus (address optional for master)

  // Make sure slave device is enabled when we start
  // This allows for software reset of this arduino and then turning on the slave
  sendEvent(3);
}

// Main loop function
void MyCommunicate::handle() {
  if (!enabled) {
    return;
  }
}

void MyCommunicate::penalty() {
  sendEvent(0);
}

void MyCommunicate::badSwitch() {
  sendEvent(0);
}

void MyCommunicate::win() {
  sendEvent(2);
}

void MyCommunicate::fail() {
  sendEvent(1);
}

void MyCommunicate::teardown() {
  enabled = false;
}

// Chat with slave
void MyCommunicate::sendEvent(int eventType) {
  if (!enabled) {
    return;
  }
  
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(eventType);     // sends one byte
  Wire.endTransmission();    // stop transmitting  
}


