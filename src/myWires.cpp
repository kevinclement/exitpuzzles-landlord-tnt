#include "Arduino.h"
#include "conditions.h"
#include <Bounce2.h>

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
  // TODO: FIX

  // // wire on state
  // if (reading >= onBegVal && reading < onEndVal && !wireOn) {
  //   wireOn = true;
  // }

  // // wire off state
  // if (reading >= offVal && wireOn) {
  //   wireOn = false;
  // }
}


void MyWires::handle() {

  //int wireReading = analogRead(BADWIRE);
  //Serial.println(wireReading);
  // Serial.print("wire2: ");
  // Serial.print(analogRead(WIRE2));
  // Serial.print(" wire3: ");
  // Serial.print(analogRead(WIRE3));
  // Serial.print(" bad: ");
  // Serial.println(analogRead(BADWIRE));
  
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
      _conditions.wireStateChange();      
      wire1State = wire1;
    }

    if (wire2 != wire2State) {
      _conditions.wireStateChange();      
      wire2State = wire2;
    }

    if (wire3 != wire3State) {
      _conditions.wireStateChange();      
      wire3State = wire3;
    }

    if (badwire != badwireState) {
      _conditions.wireStateChange();      
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
