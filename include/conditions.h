#ifndef CONDITIONS_h
#define CONDITIONS_h

#include "myDisplay.h"
#include "myTimer.h"
#include "myKeypad.h"
#include "mySpeaker.h"
#include "mySerial.h"
#include "myButtons.h"
#include "myLightSensor.h"
#include "myWires.h"
#include "myLock.h"
#include "myKeyShooter.h"

class Conditions {
public:
  Conditions();

  MyDisplay display;
  MyTimer timer;
  MyKeyPad keypad;
  MySpeaker speaker;
  MySerial serial;
  MyButtons buttons;
  MyLightSensor lightSensor;
  MyWires wires;
  MyLock lock;
  MyKeyShooter keyShooter;

  void setup();
  void win();
  void tryCode();
  void lose();
  void penalty(bool);
  void penaltyMet();
  void lightSensed();
  void handle();
  void printHelp();
  void printStatus();
  void shootKey();
  void overrideWinButton();
  void overrideToggle();
  void overrideBadWire();
  void disableCodeAfterWin();
  void forceWin();
  bool hasFinished() const { return _finished; }
  bool _overrideWinButton;

  void wireStateChange();
  void toggleStateChange();
  void wireDoorStateChange();
    
private:
  bool _finished;
  bool _solvedKey;
  bool _light = false;
  bool _codeAfterWin = true;
  bool _inToggleFailState = false;
  bool _inWireFailState = false;
  bool _exampleDoorOpened = false;
  bool _overrideToggle;
  bool _overrideBadWire;

  void teardown();
  void updateState();
};

#endif
