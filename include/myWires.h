#ifndef MYWIRES_h
#define MYWIRES_h

class Conditions;

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

