#pragma once

struct Settings {
    bool loggingEnabled;
    unsigned long tickMillis;
    unsigned long debounceMillis;
    unsigned long newLetterMillis;
    unsigned long newWordMillis;
    unsigned long dashMillis;
    unsigned long firstBackspaceMillis;
    unsigned long secondBackspaceMillis;
    unsigned long repeatBackspaceMillis;
};

Settings* initSettings();