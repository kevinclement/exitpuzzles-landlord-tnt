#ifndef MYLOCK_h
#define MYLOCK_h

class Conditions;

#define LOCK_PIN   A8
#define RELAY_ON   0
#define RELAY_OFF  1

// how long before we time out the solenoid
#define LOCK_SOL_TIMEOUT_MS 60000

class MyLock {
public:
  MyLock(Conditions &conditions);
  void setup();
  void handle();
  void teardown();
  void open();
  void close(bool timeout);

  bool _open = false;

private:
  Conditions &_conditions;
  long _poweredOnTime = 0;
  
};

#endif

