#include "Arduino.h"
#include "consts.h"
#include "myDisplay.h"

// Set the LCD address to 0x20 for a 16 chars and 2 line display
MyDisplay::MyDisplay() : 
  _lcd(0x27, 16, 2)
{
}

void MyDisplay::setup() {
  
  // Initialize the LCD
  _lcd.init();
  _lcd.backlight();
  _lcd.print(DEFAULT_DISPLAY);
  _lcd.setCursor(0, 1);
  _lcd.cursor_on();
}

void MyDisplay::handle() {
  if (_blinksLeft > 0) {
    
    if ((millis() - _lastBlinkTime) > _blinkDelay) {
      _blinksLeft--;

      if (_blinksLeft % 2 == 0) {
         _lcd.backlight();  
      } else { 
        _lcd.noBacklight();  
      }
      
      _lastBlinkTime = millis();
    }
  }
}

void MyDisplay::teardown() {
  // NOOP: want to keep the display on
}

// Full line update
void MyDisplay::update(bool line1, String text) {
  _lcd.setCursor(0, line1? 0 : 1);
  _lcd.print(text);
}

// Single character print
void MyDisplay::updateChar(char aChar) {
  _lcd.print(aChar);
}

void MyDisplay::off() {
  _lcd.noDisplay();
  _lcd.noBacklight();
}

void MyDisplay::on() {
  _lcd.display();
  _lcd.backlight();
}

// Clear the whole display, both lines.  
// NOTE: Pretty expensive
void MyDisplay::clear() {
  _lcd.clear();
}

// Resets cursor position
void MyDisplay::resetCursorPosition(byte row, byte column) {
  _lcd.setCursor(column, row);
}

void MyDisplay::blink() {
  _blinksLeft = TIMES_TO_BLINK;
}
