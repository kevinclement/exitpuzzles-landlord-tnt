#include "Arduino.h"
#include "conditions.h"

MyLock::MyLock(Conditions &conditions)
  : _conditions(conditions)
{  
}

void MyLock::setup() {
  digitalWrite(LOCK_PIN, RELAY_OFF);
  pinMode(LOCK_PIN, OUTPUT);
}

void MyLock::handle() {
  // if we've shot the key and it's been more than timeout, turn off sol since it gets hot
  if (_open &&  (millis() - _poweredOnTime) > LOCK_SOL_TIMEOUT_MS) {
    Serial.println("Lock timeout. Turning off solenoid to reduce heat.");
    close();
  }

  digitalWrite(LOCK_PIN, _open ? RELAY_ON : RELAY_OFF);
}

void MyLock::teardown() {
  // since the solenoid gets hot
  // as an extra precaution I'm setting it forced off
  close();
  
}

void MyLock::open() {
  Serial.println("Lock Opening...");
  _open = true;
  _poweredOnTime = millis();
}

void MyLock::close() {
  // only report to console if we've had a state change
  if (_open) {
    Serial.println("Lock Closing...");
  }
  
  _open = false;
}
