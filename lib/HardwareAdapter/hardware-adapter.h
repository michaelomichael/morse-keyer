#pragma once

class HardwareAdapter {
    virtual void delay(unsigned long millis);
    virtual int readKeyer(int pin);
    // virtual void keyboardPrint(char c);
    virtual void tone(unsigned int frequency, unsigned int
};