#include <Arduino.h>
#include "conditions.h"

// Globals
Conditions conditions;

void setup() {

  conditions.setup();

  // might not be needed (delay for 1 second)
  delay(1000);
}

void loop() {

  conditions.handle();
}
