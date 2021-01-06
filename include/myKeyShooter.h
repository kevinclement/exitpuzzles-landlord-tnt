#ifndef MYKEYSHOOTER_h
#define MYKEYSHOOTER_h

class Conditions;

// Wire input locations
#define PIN_RELAY   A9 // Relay pin
#define RELAY_ON    0
#define RELAY_OFF   1

// how long before we time out the solenoid
#define KEY_SOL_TIMEOUT_MS 45000

class MyKeyShooter {
public:
  MyKeyShooter(Conditions &conditions);
  void setup();
  void handle();
  void up();
  void down(bool timeout);
  void teardown();

  bool _up = false;

private:
  Conditions &_conditions;

  long _poweredOnTime = 0;
};

#endif

