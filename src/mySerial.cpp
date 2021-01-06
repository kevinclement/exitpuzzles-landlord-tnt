#include "Arduino.h"
#include "conditions.h"

MySerial::MySerial(Conditions &conditions)
  : _conditions(conditions)
{  
}

void MySerial::setup() {
  Serial.begin(115200);
  
  // set read timeout to something really low so we don't hang the 'tick'
  Serial.setTimeout(10);
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
    String msg = Serial.readStringUntil('\n');

    String command = msg;
    String value;

    // check if we need to split on space for advance commands
    for (int i = 0; i <= msg.length(); i++) {
      if (msg.charAt(i) == ' ') {
        command = msg.substring(0, i);
        value = msg.substring(i+1, msg.length());
      }
    }

    if (command == "reset") {
      _conditions.reset();
    } 
    else if (command == "set") {
      // expecting format 's hh:mm:ss'
      String hours = value.substring(0, 2);
      String minutes = value.substring(3, 5);
      String seconds = value.substring(6, 8);

      Serial.print("setting time to ");
      Serial.print(hours);
      Serial.print(":");
      Serial.print(minutes);
      Serial.print(":");
      Serial.println(seconds);

      _conditions.timer.setTimeLeft(hours.toInt(), minutes.toInt(), seconds.toInt());
      
      // refresh status to show new time
      _conditions.printStatus(); 
    }
    else if (command == "time") {
      _conditions.printStatus();
    }
    else if (command == "key") {
      Serial.println("Overriding key shooter, shooting now.");
      _conditions.shootKey();
    }
    else if (command == "keySolenoid") {
      Serial.print("Overriding key solenoid, setting to: ");
      Serial.println(value);

      if (value == "1") {
        _conditions.upKey();
      } else {
        _conditions.downKey();
      }
    }
    else if (command == "lockSolenoid") {
      Serial.print("Overriding lock solenoid, setting to: ");
      Serial.println(value);

      if (value == "1") {
        _conditions.lockOpen();
      } else {
        _conditions.lockClose();
      }
    }
    else if (command == "wires") {
      Serial.println("Showing error that wires are incorrect.");
      _conditions.penalty(true);
    }
    else if (command == "stopwire") {
      Serial.println("Turning off bad wire penalty.");
      _conditions.overrideBadWire();
    }
    else if (command == "stopwin") {
      Serial.println("Turning off win button.");
      _conditions.overrideWinButton();
    }
    else if (command == "mute") {
      Serial.println("Toggling ticker sound.");
      _conditions.speaker.toggleTickEnabled();
    }
    else if (command == "doorajar") {
      if (value == "0") {
        Serial.println("Turning off door ajar sensor.");
        // this sets the override to true, feels a little backwards here
        // but makes sense elsewhere
        _conditions.overrideDoorAjar(true);
      } 
      else if (value == "1") {
        Serial.println("Turning on door ajar sensor.");
        _conditions.overrideDoorAjar(false);
      } else {
        Serial.println("Unknown set value for door ajar.");
      }
    }
    else if (command == "stop") {
      Serial.println("Turning off toggle penalty.");
      _conditions.overrideToggle();
    }
    else if (command == "status") {
      _conditions.printStatus();
    }
    else if (command == "code") {
      _conditions.disableCodeAfterWin();
    }
    else if (command == "win") {
      Serial.println("Forcing a win.");
      _conditions.forceWin();
    }
    else if (command == "M") {
      // Passing State    Sample Failures   Simulate Unplugged    Turn back on reading
      //   M D-B             M D-A               M D-U                M D-M
      //   M 3-A             M 2-1
      //   M 2-C             M 3-B
      //   M 4-1             M 4-C
      //  
      // Possible Destinations (first char):
      //   D,2,3,4
      // Posible Sources (second char:
      //   A,B,C,1
      //
      char w = ' ';
      char badState = ' ';
      SplitDash(value, w, badState);

      int wire = -1;
      if (w == 'D') {
        wire = WIRE_DST_D_I;
      } else if (w == '4') {
        wire = WIRE_DST_4_I;
      } else if (w == '3') {
        wire = WIRE_DST_3_I;
      } else if (w == '2') {
        wire = WIRE_DST_2_I;
      }

      if (badState == ' ') {
        badState = 'A';
      }
      
      Serial.print("Setting mock state for wire ");
      Serial.print(w);
      Serial.print(" - ");
      Serial.println(badState);

      _conditions.wires.mockWireSrc[wire] = badState;
    }
    else if (command == "light") {
      // Mock override the light sensor
      Serial.println("Simulating light turning on...");
      _conditions.lightSensed();
    }
    else if (command == "blink") {
      _conditions.display.blink();
    } else {
      Serial.print("unknown command: '");
      Serial.print(command);
      Serial.println("'");
    }
  }
}

void MySerial::SplitDash(String str, char &first, char &second) { 
  first = str.charAt(0);
  if (str.length() == 3 && str.charAt(1) == '-') {
    second = str.charAt(2);
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
