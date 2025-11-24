#pragma once

struct StoredSettings {
    bool loggingEnabled = true;
    unsigned long tickDurationMillis = 70;
    unsigned long debounceMillis = 10;
    float ticksBeforeNewLetter = 7;
    float ticksBeforeNewWord = 8;
    float ticksBeforeDash = 2.5;
    float ticksBeforeFirstBackspace = 20;
    float ticksBeforeSecondBackspace = 13;
    float ticksBeforeRepeatBackspace = 7;
    unsigned long toneVolumePercent = 50;
    unsigned long toneFrequency = 440;
};