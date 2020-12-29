#ifndef MYWIRES_h
#define MYWIRES_h

class Conditions;

// ## Wire maping 
// 1->4 : A0 (100k resistor - 742 avg reading)
// B->D : A1 (4.7k resistor - 125 avg reading)
// A->3 : A2 (10k resistor - 228 avg reading)
// C->2 : A3 (no resistor - 15 avg reading)
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
  bool wire1        = false;
  bool wire2        = false;
  bool wire3        = false;
  bool badwire      = true; // start out assuming its plugged in so we don't change state

private:
  Conditions &_conditions;

  long lastDebounceTime = 0; 
  long debounceDelay = 400;
  
  bool wire1State   = false;
  bool wire2State   = false;
  bool wire3State   = false;
  bool badwireState = true;
  bool lastWire1    = false;
  bool lastWire2    = false;
  bool lastWire3    = false;
  bool lastBadwire  = true;

  void checkWire(int reading, int onBegVal, int onEndVal, int offVal, bool &wireOn);
};

#endif

