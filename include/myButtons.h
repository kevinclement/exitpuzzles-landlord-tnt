#ifndef MYBUTTONS_h
#define MYBUTTONS_h

class Conditions;

class MyButtons {
public:
  MyButtons(Conditions &conditions);
  void setup();
  void handle();
  void teardown();

  // expose so condition can apply logic
  bool btn1   = false;
  bool btn2   = false;
  bool winBtn = false;

private:
  Conditions &_conditions;
  bool enabled = true;
  bool btn1State = false;
  bool btn2State = false;
  bool lastBtn1  = false;
  bool lastBtn2  = false;
  bool winState  = false;
  bool lastWin   = false;
  int  initialWin  = true;
  
  long lastDebounceTime = 0; 
  long debounceDelay = 100;

  void checkButton(int reading, bool &btnOn);
};

#endif
