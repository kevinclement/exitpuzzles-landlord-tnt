#include "Arduino.h"
#include "pitches.h"
#include "mySpeaker.h"

MySpeaker::MySpeaker()
{  
}

void MySpeaker::setup() {
  // Initialize buzzer
  pinMode(BUZZ, OUTPUT);
  digitalWrite(BUZZ, LOW); //Buzzer Off 
}

void MySpeaker::teardown() {
  // NOOP: nothing to do
}

void MySpeaker::beep(int frequencyInHertz, long timeInMilliseconds)     // the sound producing function  
{
  int x;   
  long delayAmount = (long)(1000000/frequencyInHertz); 
  long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2)); 
  for (x=0;x<loopTime;x++)   
  {   
      digitalWrite(BUZZ,HIGH); 
      delayMicroseconds(delayAmount); 
      digitalWrite(BUZZ,LOW); 
      delayMicroseconds(delayAmount); 
  }
}

void MySpeaker::buzz(int freq, int del, int del2) {
  for (int i = 0; i < freq; i++) { // Buzzer frequency sound
    digitalWrite (BUZZ, HIGH); // Buzzer On
    delay (del);
    digitalWrite (BUZZ, LOW);  // Buzzer Off
    delay (del2);
  }
}

void MySpeaker::uhoh() {
  beep(415,100);
  delay(80);
  beep(279,100);
  delay(80);
}

void MySpeaker::tick() {
  // only do audio tick if its enabled
  if (_tickSoundEnabled) {
    buzz(5, 2, 1);
  }
}

// Turn tick audio on/off
void MySpeaker::toggleTickEnabled() {
  _tickSoundEnabled = !_tickSoundEnabled;
}

void MySpeaker::tickEnabled() {
  // TODO: Add back, don't want to listen to it right now
  //_tickSoundEnabled = true;
}

void MySpeaker::win() {
  beep(1200,100);
  delay(80);
  beep(1200,100);
  beep(1500,200);
}

void MySpeaker::funSound() {
  beep(1200,100);
  delay(80);
  beep(1200,100);
  beep(1500,200);
}

void MySpeaker::fail() {
  uhoh();
}

void MySpeaker::penalty() {
  uhoh();
}

void MySpeaker::badSwitch() {
  uhoh();
}
