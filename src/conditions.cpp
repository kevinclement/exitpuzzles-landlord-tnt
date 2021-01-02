#include "Arduino.h"
#include "conditions.h"
#include "consts.h"
#include "version.h"
#include "SoftReset.h"

Conditions::Conditions() 
  : display(),
    timer(*this),
    wires(*this),
    lock(*this),
    keypad(*this),
    speaker(),
    serial(*this),
    buttons(*this),
    lightSensor(*this),
    keyShooter(*this)
{
}

void Conditions::setup() {
  // Initialize the serial so we can debug
  serial.setup();

  // Setup the keypad
  keypad.setup();

  // Setup the buttons
  buttons.setup();
  
  // Setup the timer (LED)
  timer.setup();

  // Setup the wires
  wires.setup();

  // Setup the lock
  lock.setup();
  
  // Setup the display (LCD)
  display.setup();

  // Setup the speaker
  speaker.setup();

  // Setup the light sensor
  lightSensor.setup();

  // Setup the key shooter
  keyShooter.setup();

  // Print help out to serial
  printHelp();
}

void Conditions::handle() {
  
  // handle the toggle buttons
  buttons.handle();

  // handle the display
  display.handle();

  // handle the wires
  wires.handle();

  // handle the lock
  lock.handle();

  // handle the keypad
  keypad.handle();

  // handle the timer (LED)
  timer.handle();

  // handle any serial input
  serial.handle();

  // handle any light sensing
  lightSensor.handle();

  // handle any key shooting
  keyShooter.handle();
}

// They win!
void Conditions::win() {
  _solved = true;

  // Update display
  display.clear();

  // If we're in code after win mode, change the message
  if (_codeAfterWin) {
    display.update(true,  "    YOU WIN!    ");
    display.update(false, "EXIT CODE: 1940 ");
  } else {
    display.update(true,  "    YOU WIN!    ");
    display.update(false, "CONGRATULATIONS!");
  }
  
  // Trigger component wins
  timer.win();
  speaker.win();
  display.blink();

  // Write to the serial so we know they won
  Serial.println("They WON!");
  Serial.print("Time left: ");
  Serial.print(timer.getTimeLeft());
  Serial.print(" Password: ");
  Serial.println(keypad.getPassword());
  
  // cleanup
  teardown();

  printStatus();
}

// Try the current code on the keypad to see if its the correct one
void Conditions::tryCode() {
  if (keypad.tryPassword()) {
    win();
  }
  else {
    penalty(false);

    Serial.print("Invalid password tried: ");
    Serial.println(keypad.getPassword());

    // TODO: can remove this if I update for every character press
    printStatus();
  }
}

// They lose
void Conditions::lose() {
  
  Serial.println("BOOM!!!");

  // Update display
  display.clear();
  display.update(true,  "     BOOM!      ");
  display.update(false, "  YOU'RE DEAD!  ");

  // Component fails
  speaker.fail();

  // Cleanup
  teardown();

  printStatus();
}

// Handles penalty, will be a limited speed up
void Conditions::penalty(bool manualWirePenalty) {
  Serial.println("Penalty!");

  if (manualWirePenalty) {
    display.update(true,  "Incorrect Wires ");
  }
  else {
    display.update(true,  "Incorrect Code  ");
    display.update(false,  keypad.getIncorrectCodeMessage());
  }
  timer.penalty();
  speaker.penalty();
  
  keypad.setEnabled(false);
}

void Conditions::penaltyMet() {
  Serial.println("Done with penalty");

  // fix up the display to be fully reset
  display.clear();
  display.update(true, DEFAULT_DISPLAY);
  display.resetCursorPosition(1, 0);  
  keypad.resetPassword();
  keypad.setEnabled(true);
}

void Conditions::wireStateChange() {
  bool badWireOn = wires.wiresSrc[WIRE_DST_2_I] != 'C' && !_overrideBadWire;

  if (!_light && badWireOn) { 
    Serial.println("Detected bad wire but light is off.  Ignoring.");
    badWireOn = false;
  }

  if (badWireOn && !_inWireFailState) {
    Serial.println("Permanent penalty: missing red wire");
    _inWireFailState = true;
    updateState();
  }
  else if (!badWireOn && _inWireFailState) {
    Serial.println("Permanent penalty fixed: red wire");
    _inWireFailState = false;
    updateState();    
  }

  // check for all wires being turned on
  // no bad wires, and they have opened the example wire door
  if (!badWireOn && 
      (_exampleDoorOpened || _overrideDoorAjar) && 
      wires.wiresSrc[WIRE_DST_4_I] == '1' && 
      wires.wiresSrc[WIRE_DST_D_I] == 'B' &&
      wires.wiresSrc[WIRE_DST_3_I] == 'A') {
    shootKey();
  }

  printStatus();
}

void Conditions::toggleStateChange() {
  bool badToggleOn = (buttons.toggles[0] || buttons.toggles[3] ) && !_overrideToggle;

  if (!_light && badToggleOn) {
    Serial.println("Detected bad toggle but light is off.  Ignoring.");
    badToggleOn = false;
  }

  if (badToggleOn && !_inToggleFailState) {
    Serial.println("Permanent penalty: bad toggles");
    _inToggleFailState = true;
    updateState();
  }
  else if (!badToggleOn && _inToggleFailState) {
    Serial.println("Permanent penalty fixed: toggles good");
    _inToggleFailState = false;
    updateState();      
  }

  // check for pass condition for toggles (↓  ↑  ↑  ↓  ↑ )
  if (!buttons.toggles[0] && buttons.toggles[1] && buttons.toggles[2] && !buttons.toggles[3] && buttons.toggles[4]) {
    Serial.println("Toggles Correct!");
    speaker.openLock();
    lock.open();
  } else {
    // make sure to close the lock if toggles aren't correct
    lock.close();
  }

  printStatus();
}

void Conditions::wireDoorStateChange() 
{
  if (buttons.wireDoor && !_exampleDoorOpened) {
    Serial.println("Example wire door OPENED.");

    // this is latched, don't ever switch it back to false
    // until the device is reset
    _exampleDoorOpened = true;

    printStatus();
  }
}

void Conditions::updateState() {
  if (_inToggleFailState || _inWireFailState) {
    timer.permanentPenalty(true);
    speaker.badSwitch();
    keypad.setEnabled(false);
    display.update(false, FULL_EMPTY_LINE);
  }

  if (_inToggleFailState) {
    display.update(true, "Incorrect Toggle");
  } else if (_inWireFailState) {
    display.update(true, "Replace Red Wire");
  }

  // No penalties, back to normal
  if (!_inToggleFailState && !_inWireFailState) {
    timer.permanentPenalty(false);
    display.update(true, DEFAULT_DISPLAY);
    display.resetCursorPosition(1, 0);
    display.update(false, keypad.getPassword());
    keypad.setEnabled(true);
  }
}

// Function to override toggle error
void Conditions::overrideToggle() {
  _overrideToggle = !_overrideToggle;
  toggleStateChange();
}

void Conditions::overrideBadWire() {
  _overrideBadWire = !_overrideBadWire;
  wireStateChange();
}

void Conditions::overrideWinButton() {
  _overrideWinButton = !_overrideWinButton;
  printStatus();
}
void Conditions::overrideDoorAjar(bool enable) {
  _overrideDoorAjar = enable;
  
  // need to print status to reflect it, however
  // there is a case where you did this override to 'solve' the state of the wires
  // so we should just trigger a wire state change, and that will in-turn update the status
  wireStateChange();
}

void Conditions::forceWin() {
  win();
}

// Function to shoot key
void Conditions::shootKey() {
  if (!_solvedKey) {
    keyShooter.shoot();
    speaker.shootKey();
    _solvedKey = true;
    printStatus();
  }
}

// When the game is over turn everything off
void Conditions::teardown() {
  
  // store finished state
  _finished = true;

  // let all the components get a chance to turn off
  display.teardown();
  timer.teardown();
  wires.teardown();
  lock.teardown();
  keypad.teardown();
  speaker.teardown();
  serial.teardown();
  buttons.teardown();
  lightSensor.teardown();
  keyShooter.teardown();
}

// Print help with the system first starts up
void Conditions::printHelp() {
  Serial.println("Ultimate Bomb Simulation by kevinc.");
  Serial.println(getFullVersion("landlord-tnt"));

  // Serial class stores all the commands so print its help
  serial.printHelp();

  printStatus();
}

void Conditions::printStatus() {
  Serial.print("status=");
  
  Serial.print("version:");
  Serial.print(GIT_HASH);
  Serial.print(",gitDate:");
  Serial.print(GIT_DATE);
  Serial.print(",buildDate:");
  Serial.print(DATE_NOW);

  // TODO: should I just send all 3 parts here?
  Serial.print(",time:");
  Serial.print(timer.getTimeLeft());

  Serial.print(",toggle1:");
  Serial.print(buttons.toggles[0] ? "true" : "false");
  Serial.print(",toggle2:");
  Serial.print(buttons.toggles[1] ? "true" : "false");
  Serial.print(",toggle3:");
  Serial.print(buttons.toggles[2] ? "true" : "false");
  Serial.print(",toggle4:");
  Serial.print(buttons.toggles[3] ? "true" : "false");
  Serial.print(",toggle5:");
  Serial.print(buttons.toggles[4] ? "true" : "false");
  Serial.print(",togglesFailing:");
  Serial.print(_inToggleFailState ? "true" : "false");
  Serial.print(",overrideToggles:");
  Serial.print(_overrideToggle ? "true" : "false");

  Serial.print(",wireD:");
  Serial.print(wires.wiresSrc[WIRE_DST_D_I]);
  Serial.print(",wire2:");
  Serial.print(wires.wiresSrc[WIRE_DST_2_I]);
  Serial.print(",wire3:");
  Serial.print(wires.wiresSrc[WIRE_DST_3_I]);
  Serial.print(",wire4:");
  Serial.print(wires.wiresSrc[WIRE_DST_4_I]);
  Serial.print(",wiresFailing:");
  Serial.print(_inWireFailState ? "true" : "false");
  Serial.print(",overrideBadWire:");
  Serial.print(_overrideBadWire ? "true" : "false");

  Serial.print(",light:");
  Serial.print(_light ? "true" : "false");
  Serial.print(",exampleDoor:");
  Serial.print(_exampleDoorOpened ? "true" : "false");
  Serial.print(",overrideDoorAjar:");
  Serial.print(_overrideDoorAjar ? "true" : "false");
  Serial.print(",key:");
  Serial.print(_solvedKey ? "true" : "false");
  Serial.print(",password:");
  Serial.print(keypad.getPassword());

  Serial.print(",overrideWinButton:");
  Serial.print(_overrideWinButton ? "true" : "false");

  Serial.print(",finished:");
  Serial.print(_finished ? "true" : "false");
  Serial.print(",solved:");
  Serial.print(_solved ? "true" : "false");
  Serial.print(",timeLeftSolved:");
  if (_solved) {
    Serial.print(timer.getTimeLeft());
  }  

  Serial.println();
}

void Conditions::lightSensed() {
  _light = true;
  Serial.println("Detected light. Turning on sound.");
  speaker.tickEnabled();
  printStatus();
}

void Conditions::disableCodeAfterWin() {
  Serial.println("Disabling code after win mode");
  _codeAfterWin = false;
}

void Conditions::reset() {
  Serial.println("Reset entered, rebooting now...");
  display.clear();
  display.update(true, "REBOOTING...    ");
  soft_restart();
}
