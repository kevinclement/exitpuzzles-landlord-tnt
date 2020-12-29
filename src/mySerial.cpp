#include "Arduino.h"
#include "conditions.h"
#include "SoftReset.h"

MySerial::MySerial(Conditions &conditions)
  : _conditions(conditions)
{  
}

void MySerial::setup() {
  Serial.begin(9600);
  
  // set read timeout to something really low so we don't hang the 'tick'
  // Serial.setTimeout(10);
  // TODO: switch back
  while (!Serial); // Wait untilSerial is ready 
}

void MySerial::teardown() {
  // noop: I actually want to keep this working
}

void MySerial::handle() {
  int hours = 0;
  int minutes = 0;
  int seconds = 0;
  
  // check if we need to read from serial
  if (Serial.available()) {
    String str = Serial.readStringUntil(' ');

    if (str == "reset") {
      Serial.println("reset entered");
      soft_restart();
    } 
    else if (str == "set") {
      // expecting format 's hh:mm:ss'
        
      // read the hours (space before)
      //Serial.read();
      hours = Serial.parseInt();

      // read the minutes (colon before)
      Serial.read();
      minutes=Serial.parseInt();

      // read the seconds
      Serial.read();
      seconds=Serial.parseInt();
        
      Serial.print("setting time to ");
      Serial.print(hours);
      Serial.print(":");
      Serial.print(minutes);
      Serial.print(":");
      Serial.println(seconds);

      _conditions.timer.setTimeLeft(hours, minutes, seconds);
    }
    else if (str == "time") {
      _conditions.printTime();
    }
    else if (str == "key") {
      Serial.println("Overriding key shooter, shooting now.");
      _conditions.shootKey();
    }
    else if (str == "wires") {
      Serial.println("Showing error that wires are incorrect.");
      _conditions.penalty(true);
    }
    else if (str == "stopwire") {
      Serial.println("Turning off bad wire penalty.");
      _conditions.overrideBadWire();
    }
    else if (str == "stopwin") {
      Serial.println("Turning off win button.");
      _conditions.overrideWinButton();
    }
    else if (str == "stop") {
      Serial.println("Turning off toggle penalty.");
      _conditions.overrideToggle();
    }
    else if (str == "status") {
      _conditions.printStatus();
    }
    else if (str == "code") {
      _conditions.disableCodeAfterWin();
    }
    else if (str == "win") {
      Serial.println("Forcing a win.");
      _conditions.forceWin();
    }
    else if (str == "blink") {
      _conditions.display.blink();
    }
  }
}

void MySerial::printHelp() {
  Serial.println("Available commands:");
  Serial.println("  reset         - resets the bomb back to start");
  Serial.println("  time          - print how much time is left on the bomb");
  Serial.println("  set HH:MM:SS  - set the time on the bomb to hours (HH), minutes (MM), and seconds (SS)");
  Serial.println("  key           - override and trigger release of key");
  Serial.println("  stop          - override that will turn off toggle error and penalty");
  Serial.println("  stopwire      - override that will turn off bad wire error and penalty");
  Serial.println("  stopwin       - override that will turn off win button");
  Serial.println("  wires         - trigger an error that wires are incorrect");
  Serial.println("  status        - check some internal state");
  Serial.println("  code          - disable use of code after winning");
  Serial.println("  win           - win the device and finish it");
  Serial.println("  blink         - blink the lcd display");
}
