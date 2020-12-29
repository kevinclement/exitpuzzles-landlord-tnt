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
  pinMode(WIRE3, INPUT_PULLUP); 
  pinMode(BADWIRE, INPUT_PULLUP); 
}

void MyWires::checkWire(int reading, int lower, int upper, bool &wireOn) {
  // check if outside range and mark it off if it is
  if (reading < lower || reading > upper) {
    wireOn = false;
  } else {
    wireOn = true;
  }
}

void MyWires::handle() {
  
  // Serial.print("wire1: ");
  // Serial.println(analogRead(WIRE1));

  // Wire Mappings:
  //   1->4 : A0 (100k resistor - 742 avg reading)
  //   B->D : A1 (4.7k resistor - 125 avg reading)
  //   A->3 : A2 (10k resistor  - 228 avg reading)
  //   C->2 : A3 (no resistor   -  15 avg reading)
  checkWire(analogRead(WIRE1),  700, 850, curWires[0]);
  checkWire(analogRead(WIRE2),   90, 250, curWires[1]);
  checkWire(analogRead(WIRE3),  175, 315, curWires[2]);
  checkWire(analogRead(BADWIRE),  0, 100, curWires[3]);
  
  if ((curWires[0] != lastWires[0]) || 
      (curWires[1] != lastWires[1]) || 
      (curWires[2] != lastWires[2]) || 
      (curWires[3] != lastWires[3])) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    for (int i=0; i<4; i++) {
      if (wires[i] != curWires[i]) {
        wires[i] = curWires[i];
        
        // Serial.print("wire changed: ");
        // Serial.print(i);
        // Serial.print(" value: ");
        // Serial.print(wires[i]);
        // Serial.println();

        _conditions.wireStateChange();
      }
    }
  }

  for (int i=0; i<4; i++) {
    lastWires[i] = curWires[i];
  }
}

void MyWires::teardown() {
}
