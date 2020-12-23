#ifndef MYDISPLAY_h
#define MYDISPLAY_h

#include <LiquidCrystal_I2C.h>

class MyDisplay {
public:
  MyDisplay();
  
  void setup();
  void handle();
  void update(bool line1, String text);
  void updateChar(char aChar);
  void clear();
  void resetCursorPosition(byte row, byte column);
  void teardown();
  void blink();

private:
  LiquidCrystal_I2C _lcd;
  int _blinksLeft = 0;
  long _lastBlinkTime = 0; 
  long _blinkDelay = 150;
  int TIMES_TO_BLINK = 14;  // should equal 7 total, off then on
};

#endif
