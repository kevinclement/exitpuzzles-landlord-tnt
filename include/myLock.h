#ifndef MYLOCK_h
#define MYLOCK_h

class Conditions;

#define LOCK_PIN   A8
#define RELAY_ON   0
#define RELAY_OFF  1

class MyLock {
public:
  MyLock(Conditions &conditions);
  void setup();
  void handle();
  void teardown();

  bool open = false;

private:
  Conditions &_conditions;
};

#endif

