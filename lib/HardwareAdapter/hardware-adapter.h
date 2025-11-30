#pragma once

// Provides an abstraction around Arduino-specific functions, so that we can mock them in tests.
class HardwareAdapter {
   public:
    virtual void begin();
    virtual unsigned long millis();
    virtual void delay(unsigned long millis);
    virtual bool isKeyerPressed();
    virtual void writeLed(bool state);
    virtual void keyboardType(char c);
    virtual void keyboardBackspace();
    virtual void tone(unsigned int frequencyHertz, unsigned int volumePercent);
    virtual void noTone();
};