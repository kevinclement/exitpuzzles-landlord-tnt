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

// When to turn off the displays so we don't run them through the night
#define TURN_OFF_TIMEOUT_MS 2700000

class Conditions {
public:
  Conditions();

  MyDisplay display;
  MyTimer timer;
  MyWires wires;
  MyLock lock;
  MyKeyPad keypad;
  MySpeaker speaker;
  MySerial serial;
  MyButtons buttons;
  MyLightSensor lightSensor;  
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
  void upKey();
  void downKey();
  void lockOpen();
  void lockClose();
  void overrideWinButton();
  void overrideToggle();
  void overrideBadWire();
  void overrideDoorAjar(bool enable);
  void setCodeAfterWin(bool enable);
  void forceWin();
  void reset();
  bool _overrideWinButton;

  void wireStateChange();
  void toggleStateChange();
  void wireDoorStateChange();
    
private:
  long _finishedAt = 0;
  bool _disabledDisplays = false;
  bool _solved = false;
  bool _solvedKey;
  bool _light = false;
  bool _codeAfterWin = true;
  bool _inToggleFailState = false;
  bool _inWireFailState = false;
  bool _exampleDoorOpened = false;
  bool _overrideDoorAjar = false;
  bool _overrideToggle;
  bool _overrideBadWire;

  bool _badWiresReported[4] = { false, false, false, false };

  void teardown();
  void updateState();
  bool checkBlackWire(char wire, int reportIndex, int srcIndex, char goodValue);
};

#endif
