#include "Arduino.h"
#include "conditions.h"
#include "consts.h"

Conditions::Conditions() 
  : display(),
    timer(*this),
    wires(*this),
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

// New style of state change handling.
// We just tell conditions that it needs to go figure out the state from all the objects
// and update all the parts
void Conditions::stateChange() {

  bool badToggleOn = (buttons.btn1 || buttons.btn2) && !_overrideToggle;
  bool badWireOn = !wires.badwire && !_overrideBadWire;

  if (!_light) {
    if (badToggleOn) {
      Serial.println("Detected bad toggle but light is off.  Ignoring.");
      badToggleOn = false;
    }

    if (badWireOn) {
      Serial.println("Detected bad wire but light is off.  Ignoring.");
      badWireOn = false;
    }
  }

  // shared bad state logic
  if (badToggleOn || badWireOn) {
    
    Serial.print("Permanent penalty set - toggle: ");
    Serial.print(badToggleOn);
    Serial.print(" wire: ");
    Serial.println(badWireOn);
     
    timer.permanentPenalty(true);
    communicate.badSwitch();
    speaker.badSwitch();
    keypad.setEnabled(false);      

    // clear 2nd line
    display.update(false, FULL_EMPTY_LINE);
  }

  if (badWireOn) {
    display.update(true, "Replace Red Wire");
  }
  else if (badToggleOn) {
    display.update(true, "Incorrect Toggle");  
  } else {
    // Everything is good, reset to keypad and password current
    Serial.print("Permanent penalty fixed - toggle: ");
    Serial.print(badToggleOn);
    Serial.print(" wire: ");
    Serial.println(badWireOn);
    
    timer.permanentPenalty(false);
    display.update(true, DEFAULT_DISPLAY);
    display.resetCursorPosition(1, 0);
    display.update(false, keypad.getPassword());
    keypad.setEnabled(true);

    // check for all wires being turned on
    if (wires.wire1 && wires.wire2 && wires.wire3) {
      shootKey();
    }
  }
}

// Function to override toggle error
void Conditions::overrideToggle() {
  _overrideToggle = !_overrideToggle;
  stateChange();
}

void Conditions::overrideBadWire() {
  _overrideBadWire = !_overrideBadWire;
  stateChange();
}

void Conditions::overrideWinButton() {
  _overrideWinButton = !_overrideWinButton;
  stateChange();
}

void Conditions::forceWin() {
  win();
}

// Function to shoot key
void Conditions::shootKey() {
  if (!_solvedKey) {
    keyShooter.shoot();
    speaker.funSound();
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
  Serial.println(buttons.btn1);
  Serial.print("  toggle2: ");
  Serial.println(buttons.btn2);
  Serial.print("  wire: ");
  Serial.println(!wires.badwire);
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
