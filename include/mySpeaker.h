#ifndef MYSPEAKER_h
#define MYSPEAKER_h

// Buzzer Settings
#define BUZZ A10         // pin 5 to Buzzer
#define FAILURE_BEEPS 3  // How many times to beep on failure

class MySpeaker {
public:
  MySpeaker();

  void setup();
  void buzz(int freq, int del, int del2);
  void tick();
  void toggleTickEnabled();
  void tickEnabled();
  void win();
  void funSound();
  void fail();
  void penalty();
  void badSwitch();
  void teardown();
  
private:
  bool _tickSoundEnabled = false; // should tick sound be enabled?
  void beep(unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds);
  void uhoh();
  void bonk();
};

#endif
