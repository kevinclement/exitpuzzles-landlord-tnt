#include "Arduino.h"
#include "conditions.h"

MyTimer::MyTimer(Conditions &conditions) 
  : _conditions(conditions)
{  
}

void MyTimer::setup() {
  // Initialize the display
  lc.shutdown(0, false); // Enable display
  lc.setIntensity(0, DSP_BRIGHTNESS);  // Set brightness level (0 is min, 15 is max)
  lc.clearDisplay(0); // Clear display register
}

void MyTimer::teardown() {
  // turn the timer off, but keep the display on
  enabled = false;
}

// Main loop function
void MyTimer::handle() {
  // only do this work if we still have a timer
  if (enabled) {
    unsigned long currentTime = millis();
    
    if (currentTime - previousTime > interval) {
      previousTime = currentTime;

      // Update all the times
      milliseconds--;
      if (milliseconds <= 0 && (seconds > 0 || minutes > 0 || hours > 0)) {
        milliseconds = 99;
        seconds--;

        // seconds have changed, trigger an audio tick
        _conditions.speaker.tick();

        if (seconds < 0 && (minutes > 0 || hours > 0)) {
          minutes--;
          seconds = 59;
        }

        if (minutes < 0 && hours > 0) {
          hours--;
          minutes = 59;
        }
      }

      // Do check on big time boundaries (10,5,3,1)
      if (hours == 0 && minutes == 10 && seconds == 0 && !warningPrinted[0]) {
        warningPrinted[0] = true;
        Serial.println("Warning: 10 minutes remaining.");
        _conditions.printStatus();
      } else if (hours == 0 && minutes == 5 && seconds == 0 && !warningPrinted[1]) {
        warningPrinted[1] = true;
        Serial.println("Warning: 5 minutes remaining.");
        _conditions.printStatus();
      } else if (hours == 0 && minutes == 3 && seconds == 0 && !warningPrinted[2]) {
        warningPrinted[2] = true;
        Serial.println("Warning: 3 minutes remaining.");
        _conditions.printStatus();
      } else if (hours == 0 && minutes == 1 && seconds == 0 && !warningPrinted[3]) {
        warningPrinted[3] = true;
        Serial.println("Warning: 1 minutes remaining.");
        _conditions.printStatus();
      } 

      // Countdown is Finished? Sound Alarm
      if (milliseconds == 0 && seconds == 0) {

        // one last update to clear the display
        setDisplay(hours, minutes, seconds, milliseconds, lc);

        // boom it
        _conditions.lose();
      }
      else {
        // update the display
        setDisplay(hours, minutes, seconds, milliseconds, lc);
      }
    }

    // check if we should turn off penalty
    if (hours == penaltyHours && minutes == penaltyMinutes && seconds == penaltySeconds) {
      penaltyHours = penaltyMinutes = penaltySeconds = -1;
      interval = STANDARD_INTERVAL;

      _conditions.penaltyMet();
    }
  }

  // blink when they win
  if(blinkEnabled && millis() - blinkTime > blinkInterval) {
    blinkTime = millis();
    blinkOn = !blinkOn;  
    
    // if previously off, turn on
    if (blinkOn) {
      lc.clearDisplay(0);
    }
    else {
      setDisplay(hours, minutes, seconds, -1, lc);

      blinkTimes--;

      if (blinkTimes <= 0) {
        blinkEnabled = false;
      }
    }   
  }
}

void MyTimer::win() {
  blinkEnabled = true;
}

// Limited penalty
void MyTimer::penalty() {
  interval = PENALTY_INTERVAL;

  // calculate penalty target time
  // start out by just subtracting penalty from seconds
  penaltyHours = hours;
  penaltyMinutes = minutes;
  penaltySeconds = seconds - PENALTY_COST;

  // if we went negative, then fix it
  if (penaltySeconds < 0) {
    penaltySeconds = 60 + penaltySeconds;
    penaltyMinutes--;
  }

  // if minutes went negative then fix that
  if (penaltyMinutes < 0) {
    penaltyMinutes = 60 + penaltyMinutes;
    penaltyHours--;
  }
}

// Permanent penalty
void MyTimer::permanentPenalty(bool on) {
  interval = on ? PENALTY_INTERVAL: STANDARD_INTERVAL;
}

void MyTimer::blink() {
  blinkEnabled = true;
}

String MyTimer::getTimeLeft() {
  String s = "";

  if (hours < 10) {
    s += "0";
  }
  s += hours;
  s += ":";
  if (minutes < 10) {
    s += "0";
  }
  s += minutes;
  s += ":";
  if (seconds < 10) {
    s += "0";
  }
  s += seconds;

  return s;
}

void MyTimer::setTimeLeft(int h, int m, int s) {
  hours = h;
  minutes = m;
  seconds = s;
}

// Set the display given the value
void MyTimer::setDisplay(int hours, int minutes, int seconds, int milliseconds, LedControl lc) {
  int returnVal = 0;
  int decimal[8] = {0};

  // write out the time every 2 minutes, do it on the 10s mark to give me time to respond in the pi
  if (((hours > 0) || (minutes > 0)) && minutes % 1 == 0 && seconds == 10 && milliseconds == 99) {
    _conditions.printStatus();
  }

  // pull out the individual numbers for the display
  decimal[7] = hours / 10;
  decimal[6] = hours % 10;
  decimal[5] = minutes / 10;
  decimal[4] = minutes % 10;
  decimal[3] = seconds / 10;
  decimal[2] = seconds % 10;
  decimal[1] = milliseconds / 10;
  decimal[0] = milliseconds % 10;  
  
  byte highestNonZero = 0;
  
  // now set all the digits using library
  for (int i=8; i>0; i--) {
    int pos = i - 1;
    bool showDot = (milliseconds != -1 && pos == 2) || (milliseconds == -1 && pos == 4);
    int digit = decimal[pos];

    // if current column is higher than highest, it becomes new highest
    if (digit != 0 && pos > highestNonZero) {
      highestNonZero = pos; 
    }

    // non-zero we can just display, no problem
    if (digit != 0) {
      lc.setDigit(0, pos, digit, showDot);
    }
    else {
      // special case winning condition, ms will be -1, we want to clear out last two digits
      if (milliseconds == -1 && (pos == 1 || pos == 0)) {
        lc.setChar(0, pos, ' ', false);
      }
      else {
        // if number is 0 and column is less than highest then we can display it
        if (pos < highestNonZero) {
          lc.setDigit(0, pos, digit, showDot);
        }
        else {
          // Otherwise, clear it out.
          // NOTE: set false to showDot if you want to keep dot whole time
          lc.setChar(0, pos, ' ', false);
        }
      }
    }
  }
}
