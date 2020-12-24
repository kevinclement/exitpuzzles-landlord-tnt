#ifndef MYKEYPAD_h
#define MYKEYPAD_h

#include "Keypad_I2C.h"

class Conditions;

const uint8_t I2CADDR = 0x20;

// Password settings
#define PASSWORD "419268128453464"  // required password to win

class MyKeyPad {
public:
  MyKeyPad(Conditions &conditions);
  void handle();
  bool tryPassword();
  String getPassword();
  String getIncorrectCodeMessage();
  void resetPassword();
  void setEnabled(bool enabled);
  void setup();
  void teardown();

private:
  Conditions &_conditions;
  MyDisplay &_display;
  bool enabled = true;
  bool keyPadPasswordEnabled = false; // can you press # on the keypad and win the game
  String _realPassword = PASSWORD;
  byte _passwordLimit = _realPassword.length();
  String _curPassword = "";
  Keypad_I2C _keypad;  
};

#endif
