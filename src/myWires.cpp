#include "Arduino.h"
#include "conditions.h"
#include <Bounce2.h>

// Wire input locations
#define WIRE1   A8
#define WIRE2   A9
#define WIRE3   A10
#define BADWIRE A4

MyWires::MyWires(Conditions &conditions)
  : _conditions(conditions)
{  
}

void MyWires::setup() {
  pinMode(WIRE1, INPUT_PULLUP);
  pinMode(WIRE2, INPUT_PULLUP);
  pinMode(WIRE3,  INPUT_PULLUP); 
  pinMode(BADWIRE, INPUT_PULLUP); 
}

void MyWires::checkWire(int reading, int onBegVal, int onEndVal, int offVal, bool &wireOn) {
  // wire on state
  if (reading >= onBegVal && reading < onEndVal && !wireOn) {
    wireOn = true;
  }

  // wire off state
  if (reading >= offVal && wireOn) {
    wireOn = false;
  }
}


void MyWires::handle() {

  //int wireReading = analogRead(BADWIRE);
  //Serial.println(wireReading);
  //int wire1Reading = analogRead(WIRE1);
  //Serial.println(wire1Reading);
  //int wire3Reading = analogRead(WIRE3);
  //Serial.println(wire3Reading);
  
  checkWire(analogRead(WIRE1), 20,  40,  800, wire1);
  checkWire(analogRead(WIRE2), 220, 350, 800, wire2);
  checkWire(analogRead(WIRE3), 698, 705, 800, wire3);
  checkWire(analogRead(BADWIRE), 20, 100, 800, badwire);

  if ((wire1 != lastWire1) || 
      (wire2 != lastWire2) ||
      (wire3 != lastWire3) || 
      (badwire != lastBadwire)) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (wire1 != wire1State) {
      _conditions.stateChange();      
      wire1State = wire1;
    }

    if (wire2 != wire2State) {
      _conditions.stateChange();      
      wire2State = wire2;
    }

    if (wire3 != wire3State) {
      _conditions.stateChange();      
      wire3State = wire3;
    }

    if (badwire != badwireState) {
      _conditions.stateChange();      
      badwireState = badwire;
    }
  }

  lastWire1 = wire1;
  lastWire2 = wire2;
  lastWire3 = wire3;
  lastBadwire = badwire;
}

void MyWires::teardown() {
}
