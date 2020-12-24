#include "Arduino.h"
#include "conditions.h"

#include "Wire.h"

// Keypad settings
#define ROWS 4
#define COLS 4

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'#', '0', '*', 'D'}
};

// Configure the rows and columns based on the wiring
byte rowPins[ROWS] = {3, 2, 1, 0};  //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5, 4};  //connect to the column pinouts of the keypad

MyKeyPad::MyKeyPad(Conditions &conditions)
  : _conditions(conditions),
    _display(conditions.display),
    _keypad(Keypad_I2C(makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574, &Wire))
{
}

void MyKeyPad::setup() {
  Wire.begin();
  _keypad.begin();
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

    // Clear the LCD
    _display.resetCursorPosition(1, 0);
    _display.update(false, "                ");
    _display.resetCursorPosition(1, 0);

    _curPassword = "";
    return;
  }

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

