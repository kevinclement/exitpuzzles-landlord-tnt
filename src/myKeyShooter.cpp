#include "Arduino.h"
#include "conditions.h"

MyKeyShooter::MyKeyShooter(Conditions &conditions)
  : _conditions(conditions)
{  
}

void MyKeyShooter::setup() {
  digitalWrite(PIN_RELAY, RELAY_OFF);
  pinMode(PIN_RELAY, OUTPUT);
}

void MyKeyShooter::handle() {

  // if we've shot the key and it's been more than timeout, turn off sol since it gets hot
  if (_up && (millis() - _poweredOnTime) > KEY_SOL_TIMEOUT_MS) {
    Serial.println("Key is out, turning off solenoid to reduce heat.");
    down(true);
  }
  
  // write out to the relay
  digitalWrite(PIN_RELAY, _up ? RELAY_ON : RELAY_OFF);
}

void MyKeyShooter::up() {
  if (!_up) {
    Serial.println("KeyShooter: Shooting.");
  }
  
  _up = true;
  _poweredOnTime = millis();
}

void MyKeyShooter::down(bool timeout) {
  if (_up) {
    Serial.println("KeyShooter going down...");
  }
 
  _up = false;
  _poweredOnTime = 0;

    // if this was from a timeout we should update the status
  if (timeout) {
    _conditions.printStatus();
  }
}

void MyKeyShooter::teardown() {
  // since solenoids gets hot as an extra precaution
  // make sure it's off
  down(false);
}
