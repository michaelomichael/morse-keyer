#include "hardware-adapter.h"

#ifdef ARDUINO

#include <Arduino.h>

#define KEYER_PIN 10
#define SPEAKER_PIN 9
#define LED_PIN LED_BUILTIN

void HardwareAdapter::begin() {
    pinMode(KEYER_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
}

unsigned long HardwareAdapter::millis() { ::millis(); }

void HardwareAdapter::delay(unsigned long millis) { ::delay(millis); }

bool HardwareAdapter::isKeyerPressed() { return digitalRead(KEYER_PIN) == LOW; }

void HardwareAdapter::writeLed(bool state) { analogWrite(LED_PIN, state ? 20 : 0); }

// void HardwareAdapter::keyboardPrint(char c) { }

void HardwareAdapter::tone(unsigned int frequencyHertz, unsigned int volumePercent) {
    ::tone(SPEAKER_PIN, frequencyHertz);
}

void HardwareAdapter::noTone() { ::noTone(SPEAKER_PIN); }

#else  // Mockable void implementation
void HardwareAdapter::begin() {}
unsigned long HardwareAdapter::millis() { return 0; }
void HardwareAdapter::delay(unsigned long millis) {}
bool HardwareAdapter::isKeyerPressed() { return false; }
void HardwareAdapter::writeLed(bool state) {}
// void HardwareAdapter::keyboardPrint(char c) {}
void HardwareAdapter::tone(unsigned int frequencyHertz, unsigned int volumePercent) {}
void HardwareAdapter::noTone() {}
#endif