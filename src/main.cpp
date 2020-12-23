#include <Arduino.h>
#include "conditions.h"

// TODO:
//  expose light sensor logic as a function
//  light check for the magnet to not turn on and burn out
//  race conditions for win button bad
//  clean: unsigned long/int/byte - might be able to optimize storage

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
