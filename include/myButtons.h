#ifndef MYBUTTONS_h
#define MYBUTTONS_h

class Conditions;

// Toggle Buttons Settings
#define TOGGLE_BUTTON1 7 // input toggle button1
#define TOGGLE_BUTTON2 6 // input toggle button2
#define TOGGLE_BUTTON3 5 // input toggle button3
#define TOGGLE_BUTTON4 4 // input toggle button4
#define TOGGLE_BUTTON5 3 // input toggle button5
#define WINBTN         8 // location of win button

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
