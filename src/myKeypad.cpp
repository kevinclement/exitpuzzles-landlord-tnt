#include "Arduino.h"
#include "conditions.h"

// Keypad settings
#define ROWS 4
#define COLS 4

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'#', '0', '*', 'D'}
};

// wires by pin on keypad: 
//   0: UNUSED
//   1: COL1
//   2: COL2
//   3: COL3
//   4: COL4
//   5: ROW1
//   6: ROW2
//   7: ROW3
//   8: ROW4
//   9: UNUSED

// Configure the rows and columns based on the wiring
byte rowPins[ROWS] = {30, 32, 34, 36};  //connect to the row pinouts of the keypad
byte colPins[COLS] = {22, 24, 26, 28};  //connect to the column pinouts of the keypad

MyKeyPad::MyKeyPad(Conditions &conditions)
  : _conditions(conditions),
    _display(conditions.display),
    _keypad(Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS))
{
}

void MyKeyPad::setup() {
}

void MyKeyPad::teardown() {
  enabled = false;
}

void MyKeyPad::handle() {

  if (!enabled) {
    return;
  }

  char key = _keypad.getKey();

  // return if noop
  if (key == NO_KEY) {
    return;
  }

  // * is clear, but it actually comes across wire as #
  if (key == '#' || key == '*') {
    // EASTER EGG
    // if the code is 311311 and they press *, then
    // reset the device.  Allows for a reset without the need to fiddle with website
    Serial.println(_curPassword);
    if (_curPassword == "311311") {
      _conditions.reset();
    }

    // special freq for clear
    _conditions.speaker.beep(400, 70);
    
    // Clear the LCD
    _display.resetCursorPosition(1, 0);
    _display.update(false, "                ");
    _display.resetCursorPosition(1, 0);

    _curPassword = "";
    _conditions.printStatus();
    return;
  }

  // audio feedback - randomize the freq a bit to add flavor
  _conditions.speaker.beep(350 + random(100, 400), 30);

  // print key on lcd
  _display.updateChar(key);

  // if we're at the limit, replace last character in string with key
  if (_curPassword.length() == _passwordLimit) {
    _curPassword[_passwordLimit - 1] = key;
  }
  else {
    _curPassword += key;
  }

  // after updating, if we're at the limit, move cursor back
  if (_curPassword.length() == _passwordLimit) {
    _display.resetCursorPosition(1, _passwordLimit - 1);
  }

  _conditions.printStatus();
}

void MyKeyPad::setEnabled(bool newEnabled) {
  enabled = newEnabled;
}

// See if the current password is keypad correct
bool MyKeyPad::tryPassword() {

  // total hack due to some bad wiring
  if (_curPassword == _realPassword) {
    return true;
  }
  else if (_curPassword == "419268028453464") {
    return true;
  }
  else if (_curPassword == "419268228453464") {
    return true;
  }
  else if (_curPassword == "419268328453464") {
    return true;
  }
  else if (_curPassword == "419268428453464") {
    return true;
  }
  else if (_curPassword == "419268528453464") {
    return true;
  }
  else if (_curPassword == "419268628453464") {
    return true;
  }
  else if (_curPassword == "419268728453464") {
    return true;
  }
  else if (_curPassword == "419268828453464") {
    return true;
  }
  else if (_curPassword == "419268928453464") {
    return true;
  }
  else {
    return false;
  }
}

String MyKeyPad::getPassword() {
  return _curPassword;
}

String MyKeyPad::getIncorrectCodeMessage() {
  String curPassword = _curPassword;
  String codeMessage = "";

  // make sure they put in all the required digits
  if (curPassword.length() != 15) {
    return "Needs 15 Numbers";
  }
  
  // check for which segment is bad.  better coder would use a bitmask, i dont have time
  //   segments are: x-xx-xxx-xxxx-xxxxx 
  if(!curPassword.startsWith("4")) {
    codeMessage = "x";
  } else {
    codeMessage = "-";
  }

  if (curPassword.indexOf("19") != 1) {
    codeMessage += "xx";
  }
  else {
    codeMessage += "--";
  }

  if (curPassword.indexOf("268") != 3) {
    codeMessage += "xxx";
  }
  else {
    codeMessage += "---";
  }

  if (curPassword.indexOf("0284") != 6 && curPassword.indexOf("1284") != 6 && curPassword.indexOf("2284") != 6 && curPassword.indexOf("3284") != 6 && curPassword.indexOf("4284") != 6 && curPassword.indexOf("5284") != 6 && curPassword.indexOf("6284") != 6 && curPassword.indexOf("7284") != 6 && curPassword.indexOf("8284") != 6 && curPassword.indexOf("9284") != 6) {
    codeMessage += "xxxx";
  }
  else {
    codeMessage += "----";
  }

  if (curPassword.indexOf("53464") != 10) {
    codeMessage += "xxxxx";
  }
  else {
    codeMessage += "-----";
  }
  
  return codeMessage;
}

void MyKeyPad::resetPassword() {
  _curPassword = "";
}

