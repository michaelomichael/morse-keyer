#pragma once

enum class KeyerState { Pressed, Released };

class HardwareAdapter {
   public:
    virtual void begin();
    virtual unsigned long millis();
    virtual void delay(unsigned long millis);
    virtual bool isKeyerPressed();
    virtual void writeLed(bool state);
    // virtual void keyboardPrint(char c);
    virtual void tone(unsigned int frequencyHertz, unsigned int volumePercent);
    virtual void noTone();
};