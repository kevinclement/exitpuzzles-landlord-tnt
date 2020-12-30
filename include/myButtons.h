#ifndef MYBUTTONS_h
#define MYBUTTONS_h

class Conditions;

// Toggle Buttons Settings
#define TOGGLE_BUTTON1 6 // input toggle button1
#define TOGGLE_BUTTON2 2 // input toggle button2
#define TOGGLE_BUTTON3 5 // input toggle button3
#define TOGGLE_BUTTON4 4 // input toggle button4
#define TOGGLE_BUTTON5 3 // input toggle button5
#define WINBTN         9 // location of win button
#define WIRE_DOOR      8 // pop-up button to tell if wire door example is open

#define WIRE_DOOR_OPEN LOW
#define WIRE_DOOR_CLOSED HIGH

class MyButtons {
public:
  MyButtons(Conditions &conditions);
  void setup();
  void handle();
  void teardown();

  // expose so conditions can apply logic
  bool toggles[5] = {false, false, false, false, false};
  bool winBtn = false;
  bool wireDoor = false;

private:
  Conditions &_conditions;
  bool enabled = true;
  bool lastToggles[5] = {false, false, false, false, false};
  bool curToggles[5] =  {false, false, false, false, false};
    
  bool curWin = false;
  bool lastWin = false;
  int  initialWin  = true;

  bool curWireDoor = false;
  bool lastWireDoor = false;

  long lastDebounceTime = 0; 
  long debounceDelay = 100;

  void checkButton(int reading, bool &btnOn);
};

#endif
