#pragma once

struct StoredSettings {
    bool loggingEnabled = true;
    unsigned int tickDurationMillis = 70;
    unsigned int debounceMillis = 50;
    float ticksBeforeNewLetter = 7;
    float ticksBeforeNewWord = 8;
    float ticksBeforeDash = 2.5;
    float ticksBeforeFirstBackspace = 20;
    float ticksBeforeSecondBackspace = 13;
    float ticksBeforeRepeatBackspace = 4;
    bool toneEnabled = true;
    unsigned int toneVolumePercent = 50;
    unsigned int toneFrequencyHertz = 440;
};