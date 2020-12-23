#include "Arduino.h"
#include "conditions.h"

// Wire input locations
#define PIN_RELAY   22 // Relay pin
#define RELAY_ON    0
#define RELAY_OFF   1

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
  if (solOn && enabled && (millis() - turnedOn) > timeoutTime) {
    Serial.println("Key is out, turning off solenoid to reduce heat.");
    enabled = false;
  }
  
  // write out to the relay
  digitalWrite(PIN_RELAY, solOn && enabled ? RELAY_ON : RELAY_OFF);
}

void MyKeyShooter::shoot() {
  Serial.println("KeyShooter: Shooting.");
  solOn = true;
  turnedOn = millis();
}

void MyKeyShooter::teardown() {
}
