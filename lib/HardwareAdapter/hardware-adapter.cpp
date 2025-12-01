#include "hardware-adapter.h"

#ifdef ARDUINO

#include <Arduino.h>
#include <EEPROM.h>
#include <Keyboard.h>

#define KEYER_PIN 10
#define SPEAKER_PIN 9
#define LED_PIN LED_BUILTIN

void HardwareAdapter::begin() {
    pinMode(KEYER_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    Keyboard.begin();
}

unsigned long HardwareAdapter::millis() { return ::millis(); }

void HardwareAdapter::delay(unsigned long millis) { ::delay(millis); }

bool HardwareAdapter::isKeyerPressed() { return digitalRead(KEYER_PIN) == LOW; }

void HardwareAdapter::writeLed(bool state) { analogWrite(LED_PIN, state ? 20 : 0); }

void HardwareAdapter::keyboardType(char c) { Keyboard.print(c); };

void HardwareAdapter::keyboardBackspace() {
    Keyboard.print((char)0x08);  // ^H
};

void HardwareAdapter::tone(unsigned int frequencyHertz, unsigned int volumePercent) {
    ::tone(SPEAKER_PIN, frequencyHertz);
}

void HardwareAdapter::noTone() { ::noTone(SPEAKER_PIN); }

uint8_t HardwareAdapter::readEeprom(int address) { return EEPROM.read(address); }
void HardwareAdapter::writeEeprom(int address, uint8_t value) { EEPROM.write(address, value); }

#else  // Mockable void implementation
void HardwareAdapter::begin() {}
unsigned long HardwareAdapter::millis() { return 0; }
void HardwareAdapter::delay(unsigned long millis) {}
bool HardwareAdapter::isKeyerPressed() { return false; }
void HardwareAdapter::writeLed(bool state) {}
void HardwareAdapter::keyboardType(char c) {};
void HardwareAdapter::keyboardBackspace() {};
void HardwareAdapter::tone(unsigned int frequencyHertz, unsigned int volumePercent) {}
void HardwareAdapter::noTone() {}
uint8_t HardwareAdapter::readEeprom(int address) { return 0; }
void HardwareAdapter::writeEeprom(int address, uint8_t value) {}
#endif