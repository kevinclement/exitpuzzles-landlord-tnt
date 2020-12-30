#include "Arduino.h"
#include "conditions.h"
#include "consts.h"

Conditions::Conditions() 
  : display(),
    timer(*this),
    wires(*this),
    lock(*this),
    keypad(*this),
    speaker(),
    serial(*this),
    buttons(*this),
    communicate(*this),
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

  // Setup the communication with slave
  communicate.setup();

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

  // handle any communication
  communicate.handle();

  // handle any light sensing
  lightSensor.handle();

  // handle any key shooting
  keyShooter.handle();
}

// They win!
void Conditions::win() {

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
  communicate.win();
  display.blink();

  // Write to the serial so we know they won
  Serial.println("They WON!");
  Serial.print("Time left: ");
  Serial.print(timer.getTimeLeft());
  Serial.print(" Password: ");
  Serial.println(keypad.getPassword());
  
  // cleanup
  teardown();
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
  communicate.fail();

  // Cleanup
  teardown();
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
  communicate.penalty();
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
  bool badWireOn = !wires.wires[3] && !_overrideBadWire;

  if (!_light && badWireOn) { 
    Serial.println("Detected bad wire but light is off.  Ignoring.");
    badWireOn = false;
  }

  if (badWireOn && !_inWireFailState) {
    Serial.println("Permanent penalty set - wire: 1");
    _inWireFailState = true;
    updateState();
  }
  else if (!badWireOn && _inWireFailState) {
    Serial.println("Permanent penalty fixed - wire: 0");
    _inWireFailState = false;
    updateState();    
  }

  // check for all wires being turned on
  if (!badWireOn && wires.wires[0] && wires.wires[1] && wires.wires[2]) {
    shootKey();
  }
}

void Conditions::toggleStateChange() {
  bool badToggleOn = (buttons.toggles[0] || buttons.toggles[3] ) && !_overrideToggle;

  if (!_light && badToggleOn) {
    Serial.println("Detected bad toggle but light is off.  Ignoring.");
    badToggleOn = false;
  }

  if (badToggleOn && !_inToggleFailState) {
    Serial.print("Permanent penalty set - toggle: ");
    Serial.println(badToggleOn);
    _inToggleFailState = true;
    updateState();
  }
  else if (!badToggleOn && _inToggleFailState) {
    Serial.print("Permanent penalty fixed - toggle: ");
    Serial.println(badToggleOn);
    _inToggleFailState = false;
    updateState();      
  }

  // check for pass condition for toggles
  // ↓  ↑  ↑  ↓  ↑ 
  if (!buttons.toggles[0] && buttons.toggles[1] && buttons.toggles[2] && !buttons.toggles[3] && buttons.toggles[4]) {
    Serial.println("Toggles Correct!");
    speaker.openLock();
    lock.open = true;
  }
}

void Conditions::updateState() {
  if (_inToggleFailState || _inWireFailState) {
    timer.permanentPenalty(true);
    communicate.badSwitch();
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

  // TODO: fix this?? need to call both?
  //stateChange();
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
  }

  // TODO: turn off wire logic once we've solved it?
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
  communicate.teardown();
  lightSensor.teardown();
  keyShooter.teardown();
}

// Print help with the system first starts up
void Conditions::printHelp() {
  Serial.println("Ultimate Bomb Simulation by kevinc.");

  // Serial class stores all the commands so print its help
  serial.printHelp();
}

void Conditions::printStatus() {
  Serial.println("Status: ");
  Serial.print("  toggle1: ");
  Serial.println(buttons.toggles[0]);
  Serial.print("  toggle2: ");
  Serial.println(buttons.toggles[1]);
  Serial.print("  wire: ");
  Serial.println(!wires.wires[3]);
  Serial.print("  key solved: ");
  Serial.println(_solvedKey);
  Serial.print("  all done: ");
  Serial.println(_finished);
}

void Conditions::printTime() {
  Serial.print("current time: ");
  Serial.println(timer.getTimeLeft());
}

void Conditions::lightSensed() {
  _light = true;
  Serial.println("Detected light. Turning on sound.");
  speaker.tickEnabled();
}

void Conditions::disableCodeAfterWin() {
  Serial.println("Disabling code after win mode");
  _codeAfterWin = false;
}
