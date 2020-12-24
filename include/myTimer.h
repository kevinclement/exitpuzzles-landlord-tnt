#ifndef MYTIMER_h
#define MYTIMER_h

#include "LedControl.h"
class Conditions;

// Timer Settings
#define TIMER_HOURS        1
#define TIMER_MINUTES      8
#define TIMER_SECONDS      0
#define TIMER_MILLISECONDS 0

#define STANDARD_INTERVAL 9  // How fast the timer will update (ms) - BUG: this should be 10, but when set to 10 i lost 5 seconds in 1:15 test
#define PENALTY_INTERVAL  1  // How fast during penalty time
#define PENALTY_COST      10 // How much to penalize (s)

// Display Settings
#define DSP_BRIGHTNESS 1  // brightness level (0 is min, 15 is max) 
#define DSP_DIN        11  // pin 4 to DIN
#define DSP_CLK        12  // pin 3 to Clock
#define DSP_LOAD       13  // pin 2 to Load

class MyTimer {
public:
  MyTimer(Conditions &conditions);
  
  void setup();
  void handle();
  void win();
  void penalty();
  void permanentPenalty(bool on);
  void blink();
  void teardown();
  String getTimeLeft();
  void setTimeLeft(int h, int m, int s);

private:
  Conditions &_conditions;
  LedControl lc = LedControl(DSP_DIN, DSP_CLK, DSP_LOAD, 1);
  
  bool enabled               = true;  // flag to enable timer
  unsigned long previousTime = 0;     // tracks last time we fired the timer 

  int interval        = STANDARD_INTERVAL; // allows for variable times
  int hours           = TIMER_HOURS;
  int minutes         = TIMER_MINUTES;
  int seconds         = TIMER_SECONDS;
  int milliseconds    = TIMER_MILLISECONDS;
  int penaltyHours    = -1;
  int penaltyMinutes  = -1;
  int penaltySeconds  = -1;
  bool warningPrinted[4] = {false};

  // blink settings
  bool blinkEnabled       = false;
  unsigned long blinkTime = 0;
  bool blinkOn            = false;
  int blinkInterval       = 600;
  int blinkTimes          = 3;
  
  void setDisplay(int hours, int minutes, int seconds, int milliseconds, LedControl lc);
};


#endif
