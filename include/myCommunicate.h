#ifndef MYCOMMUNICATE_h
#define MYCOMMUNICATE_h

#include <Wire.h>
class Conditions;

class MyCommunicate {
public:
  MyCommunicate(Conditions &conditions);
  void setup();
  void handle();
  void win();
  void fail();
  void penalty();
  void badSwitch();
  void teardown();

private:
  Conditions &_conditions;
  void sendEvent(int eventType);
  bool enabled = true;
};



#endif

