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
  digitalWrite(LOCK_PIN, open ? RELAY_ON : RELAY_OFF);
}

void MyLock::teardown() {
}
