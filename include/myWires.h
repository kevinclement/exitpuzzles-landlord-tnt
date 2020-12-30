#ifndef MYWIRES_h
#define MYWIRES_h

class Conditions;

// Wire maping:
//   1->4 : A0 (100k resistor - 742 avg reading)
//   B->D : A1 (4.7k resistor - 125 avg reading)
//   A->3 : A2 (10k resistor  - 228 avg reading)
//   C->2 : A3 (no resistor   -  15 avg reading)
#define WIRE1   A0
#define WIRE2   A1
#define WIRE3   A2
#define BADWIRE A3

class MyWires {
public:
  MyWires(Conditions &conditions);
  void setup();
  void handle();
  void teardown();

  // exposed state so condition can use it properly
  bool wires[4] = { false, false, false, true };  // start out assuming wire 4 is plugged in
                                                  // so we don't cause a change state right away

private:
  Conditions &_conditions;

  long lastDebounceTime = 0; 
  long debounceDelay = 400;
  
  bool lastWires[4] = { false, false, false, true };
  bool curWires[4]  = { false, false, false, true };

  void checkWire(int reading, int lower, int upper, bool &wireOn);
};

#endif

