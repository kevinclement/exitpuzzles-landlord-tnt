#include "Arduino.h"
#include "conditions.h"

MyButtons::MyButtons(Conditions &conditions)
  : _conditions(conditions)
{  
}

void MyButtons::setup() {
  pinMode(TOGGLE_BUTTON1, INPUT_PULLUP);
  pinMode(TOGGLE_BUTTON2, INPUT_PULLUP);
  pinMode(WINBTN, INPUT_PULLUP); 
}

// Main loop function
void MyButtons::handle() {

  if (!enabled) {
    return;
  }
  
  int btn1Reading = analogRead(TOGGLE_BUTTON1);
  int btn2Reading = analogRead(TOGGLE_BUTTON2);
  int winReading = analogRead(WINBTN);
  
  // TMP
  ///////////////////////////////////////////
  // Note: 50is when its up
  // Note: 1000> when its down
  // Need to see it go from down to up to down  
  ////////////////////////////////////////////

  checkButton(btn1Reading, btn1);
  checkButton(btn2Reading, btn2);
  checkButton(winReading, winBtn);

  if ((btn1 != lastBtn1) || 
      (btn2 != lastBtn2) || 
      (winBtn != lastWin)) {
    lastDebounceTime = millis();
  }
  
  // debounce the button changes
  if ((millis() - lastDebounceTime) > debounceDelay) {

    // Check button1 change
    if (btn1 != btn1State) {
      btn1State = btn1;
      _conditions.stateChange();
    }

    // Check button2 change
    if (btn2 != btn2State) {
      btn2State = btn2;
      _conditions.stateChange();
    }
    
    // Check win condition
    if (winBtn != winState && !_conditions._overrideWinButton) {
      winState = winBtn;
      if (winState) {
        if (initialWin) {
          initialWin = false;
        }
        else {
          _conditions.tryCode();
        }
      }      
    }
  }

  lastBtn1 = btn1;
  lastBtn2 = btn2;
  lastWin = winBtn;
}

void MyButtons::teardown() {
  enabled = false;
}

void MyButtons::checkButton(int reading, bool &btnOn) {
  
  // button switched on state
  if (reading > 500 && !btnOn) {
    btnOn = true;
  }

  // buttons switched to off state
  if (reading <= 500 && btnOn) {
    btnOn = false;
  }
}
