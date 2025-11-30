#pragma once

#define KEYBOARD_LAYOUT_DEFAULT 0
#define KEYBOARD_LAYOUT_DVORAK 1

struct StoredSettings {
    bool loggingEnabled = true;
    unsigned int tickDurationMillis = 70;
    unsigned int debounceMillis = 50;
    float ticksBeforeNewLetter = 7;
    float ticksBeforeNewWord = 8;
    float ticksBeforeDash = 2.5;
    float ticksBeforeFirstBackspace = 30;
    float ticksBeforeSecondBackspace = 13;
    float ticksBeforeRepeatBackspace = 3;
    bool toneEnabled = true;
    unsigned int toneVolumePercent = 50;
    unsigned int toneFrequencyHertz = 440;
    bool keyboardEnabled = true;
    unsigned int keyboardLayout = 0;  // Default
};