#ifndef MYKEYPAD_h
#define MYKEYPAD_h

#include <Keypad.h>

class Conditions;

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
  Keypad _keypad;  
};

#endif
