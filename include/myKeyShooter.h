#ifndef MYKEYSHOOTER_h
#define MYKEYSHOOTER_h

class Conditions;

class MyKeyShooter {
public:
  MyKeyShooter(Conditions &conditions);
  void setup();
  void handle();
  void shoot();
  void teardown();

private:
  Conditions &_conditions;

  bool enabled = true;
  bool solOn = false;
  long turnedOn = 0;
  long timeoutTime = 45000; // turn off after 45 seconds
};

#endif

