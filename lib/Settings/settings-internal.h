#pragma once

struct StoredSettings {
    unsigned long tickDurationMillis = 70;
    unsigned long debounceMillis = 10;
    float ticksBeforeNewLetter = 7;
    float ticksBeforeNewWord = 8;
    float ticksBeforeDash = 2.5;
    float ticksBeforeFirstBackspace = 20;
    float ticksBeforeSecondBackspace = 13;
    float ticksBeforeRepeatBackspace = 7;
};