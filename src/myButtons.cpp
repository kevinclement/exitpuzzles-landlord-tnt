#include "Arduino.h"
#include "conditions.h"

MyButtons::MyButtons(Conditions &conditions)
  : _conditions(conditions)
{  
}

void MyButtons::setup() {
  pinMode(TOGGLE_BUTTON1, INPUT_PULLUP);
  pinMode(TOGGLE_BUTTON2, INPUT_PULLUP);
  pinMode(TOGGLE_BUTTON3, INPUT_PULLUP);
  pinMode(TOGGLE_BUTTON4, INPUT_PULLUP);
  pinMode(TOGGLE_BUTTON5, INPUT_PULLUP);
  pinMode(WINBTN, INPUT_PULLUP); 
}

// Main loop function
void MyButtons::handle() {

  if (!enabled) {
    return;
  }

  checkButton(!digitalRead(TOGGLE_BUTTON1), curToggles[0]);
  checkButton(!digitalRead(TOGGLE_BUTTON2), curToggles[1]);
  checkButton(!digitalRead(TOGGLE_BUTTON3), curToggles[2]);
  checkButton(!digitalRead(TOGGLE_BUTTON4), curToggles[3]);
  checkButton(!digitalRead(TOGGLE_BUTTON5), curToggles[4]);

  checkButton(digitalRead(WINBTN), winBtn);

  if ((curToggles[0] != lastToggles[0]) || 
      (curToggles[1] != lastToggles[1]) || 
      (curToggles[2] != lastToggles[2]) || 
      (curToggles[3] != lastToggles[3]) || 
      (curToggles[4] != lastToggles[4]) || 
      (winBtn != lastWin)) {
    lastDebounceTime = millis();
  }
  
  // debounce the button changes
  if ((millis() - lastDebounceTime) > debounceDelay) {

    // Check toggles change
    for (int i=0; i<5; i++) {
      if (toggles[i] != curToggles[i]) {
        toggles[i] = curToggles[i];
        _conditions.toggleStateChange();
      }
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

  for (int i=0; i<5; i++) {
    lastToggles[i] = curToggles[i];
  }

  lastWin = winBtn;
}

void MyButtons::teardown() {
  enabled = false;
}

void MyButtons::checkButton(int reading, bool &btnOn) {
  
  // button switched on state
  if (reading == HIGH && !btnOn) {
    btnOn = true;
  }

  // buttons switched to off state
  if (reading == LOW && btnOn) {
    btnOn = false;
  }
}
