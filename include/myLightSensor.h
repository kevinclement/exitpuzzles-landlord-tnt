#ifndef MYLIGHTSENSOR_h
#define MYLIGHTSENSOR_h

class Conditions;

class MyLightSensor {
public:
  MyLightSensor(Conditions &conditions);
  void setup();
  void handle();
  void teardown();

private:
  Conditions &_conditions;

  int lightPin = 1;
  int threshold = 750;
  bool sawLight = false;
};

#endif

