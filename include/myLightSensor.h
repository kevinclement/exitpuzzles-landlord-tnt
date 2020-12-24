#ifndef MYLIGHTSENSOR_h
#define MYLIGHTSENSOR_h

class Conditions;

#define LS_PIN A11
#define LS_THRESHOLD 750

class MyLightSensor {
public:
  MyLightSensor(Conditions &conditions);
  void setup();
  void handle();
  void teardown();

private:
  Conditions &_conditions;

  bool sawLight = false;
};

#endif

