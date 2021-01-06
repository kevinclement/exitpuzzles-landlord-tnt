#ifndef MYWIRES_h
#define MYWIRES_h

class Conditions;

// Wire maping:
//   1->4 : A0 (100k resistor - 742 avg reading)
//   B->D : A1 (4.7k resistor - 125 avg reading)
//   A->3 : A2 (10k resistor  - 228 avg reading)
//   C->2 : A3 (no resistor   -  15 avg reading)

// Source Wires - they control the signal strength
//  1: 700-775
//  A: 200-275
//  B: 100-175
//  C: 0-75
#define WIRE_SRC_1_LOW      700
#define WIRE_SRC_1_HIGH     775
#define WIRE_SRC_A_LOW      200
#define WIRE_SRC_A_HIGH     275
#define WIRE_SRC_B_LOW      100
#define WIRE_SRC_B_HIGH     175
#define WIRE_SRC_C_LOW        0
#define WIRE_SRC_C_HIGH      75
#define WIRE_SRC_UNPLUGGED  800

// Destination Wires
#define WIRE_DST_4   A0
#define WIRE_DST_D   A1
#define WIRE_DST_3   A2
#define WIRE_DST_2   A3

// Array mappings for destination so I stop going crazy
#define WIRE_DST_4_I 0
#define WIRE_DST_D_I 1
#define WIRE_DST_3_I 2
#define WIRE_DST_2_I 3

class MyWires {
public:
  MyWires(Conditions &conditions);
  void setup();
  void handle();
  void teardown();

  // exposed state so condition can use it properly
  char wiresSrc[4] = { 'U', 'U', 'U', 'C' };      // start out assuming wire 4 is plugged in
                                                  // so we don't cause a change state right away

  char mockWireSrc[4] =  { 'M', 'M', 'M', 'M' };  // mock that allows me to trigger wire connections remotely to
                                                  // test things without having to be physically present
private:
  Conditions &_conditions;

  unsigned long lastDebounceTime = 0; 
  unsigned long debounceDelay = 400;

  char lastWiresSrc[4] = { 'U', 'U', 'U', 'C' };
  char curWiresSrc[4]  = { 'U', 'U', 'U', 'C' };
  

  void checkWire(int reading, int index);
  char determineSource(int sig);
};

#endif

