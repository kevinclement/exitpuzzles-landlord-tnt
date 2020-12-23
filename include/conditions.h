#ifndef CONDITIONS_h
#define CONDITIONS_h

#include "myDisplay.h"
#include "myTimer.h"
#include "myKeypad.h"
#include "mySpeaker.h"
#include "mySerial.h"
#include "myButtons.h"
#include "myCommunicate.h"
#include "myLightSensor.h"
#include "myWires.h"
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
  MyCommunicate communicate;
  MyLightSensor lightSensor;
  MyWires wires;
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
  void printTime();
  void shootKey();
  void overrideWinButton();
  void overrideToggle();
  void overrideBadWire();
  void disableCodeAfterWin();
  void forceWin();
  bool hasFinished() const { return _finished; }
  bool _overrideWinButton;

  void stateChange();
    
private:
  bool _finished;
  bool _solvedKey;
  bool _light = false;
  bool _codeAfterWin = true;
  bool _overrideToggle;
  bool _overrideBadWire;

  void teardown();
};

#endif
