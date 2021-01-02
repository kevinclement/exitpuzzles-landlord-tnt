#include "Arduino.h"
#include "conditions.h"
#include <Bounce2.h>

MyWires::MyWires(Conditions &conditions)
  : _conditions(conditions)
{  
}

void MyWires::setup() {
  pinMode(WIRE_DST_4, INPUT_PULLUP);
  pinMode(WIRE_DST_D, INPUT_PULLUP);
  pinMode(WIRE_DST_3, INPUT_PULLUP); 
  pinMode(WIRE_DST_2, INPUT_PULLUP); 
}

void MyWires::checkWire(int reading, char &wireOn) {
  // determine source location of reading
  char src = determineSource(reading);
  
  wireOn = src;
}

void MyWires::handle() {
  
  // Serial.print("wire1: ");
  // Serial.println(analogRead(WIRE1));

  // Wire Mappings:
  //   1->4 : A0 (100k resistor - 742 avg reading)
  //   B->D : A1 (4.7k resistor - 125 avg reading)
  //   A->3 : A2 (10k resistor  - 228 avg reading)
  //   C->2 : A3 (no resistor   -  15 avg reading)
  checkWire(analogRead(WIRE_DST_4), curWiresSrc[0]);
  checkWire(analogRead(WIRE_DST_D), curWiresSrc[1]);
  checkWire(analogRead(WIRE_DST_3), curWiresSrc[2]);
  checkWire(analogRead(WIRE_DST_2), curWiresSrc[3]);
  
  if ((curWiresSrc[0] != lastWiresSrc[0]) || 
      (curWiresSrc[1] != lastWiresSrc[1]) || 
      (curWiresSrc[2] != lastWiresSrc[2]) || 
      (curWiresSrc[3] != lastWiresSrc[3])) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    for (int i=0; i<4; i++) {
      if (wiresSrc[i] != curWiresSrc[i]) {
        wiresSrc[i] = curWiresSrc[i];
        
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
    lastWiresSrc[i] = curWiresSrc[i];
  }
}

void MyWires::teardown() {
}

char MyWires::determineSource(int sig) {
  char src;

  if (sig > WIRE_SRC_C_LOW && sig < WIRE_SRC_C_HIGH) {
    src = 'C';
  } else if (sig > WIRE_SRC_B_LOW && sig < WIRE_SRC_B_HIGH) {
    src = 'B';
  } else if (sig > WIRE_SRC_A_LOW && sig < WIRE_SRC_A_HIGH) {
    src = 'A';
  } else if (sig > WIRE_SRC_1_LOW && sig < WIRE_SRC_1_HIGH) {
    src = '1';
  } else if (sig > WIRE_SRC_UNPLUGGED) {
    src = 'U';
  } else {
    src = 'E';
  }

  return src;
}
