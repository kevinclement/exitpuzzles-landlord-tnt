#ifndef MYSPEAKER_h
#define MYSPEAKER_h

// Buzzer Settings
#define BUZZ A10         // pin A10 to Buzzer
#define FAILURE_BEEPS 3  // How many times to beep on failure

class MySpeaker {
public:
  MySpeaker();

  void setup();
  void buzz(int freq, int del, int del2);
  void beep(int frequencyInHertz, long timeInMilliseconds);
  void tick();
  void toggleTickEnabled();
  void tickEnabled();
  void win();
  void shootKey();
  void openLock();
  void fail();
  void penalty();
  void badSwitch();
  void teardown();
  
private:
  bool _tickSoundEnabled = false; // should tick sound be enabled?
  void uhoh();
};

#endif
