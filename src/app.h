#pragma once

#include "hardware-adapter.h"
#include "morse-tree.h"
#include "serial-adapter.h"
#include "settings.h"

#define MAX_SENTENCE_CHARS 100
#define MAX_MORSE_SYMBOLS_PER_LETTER 10

enum class Event {
    InitialisationStarted,
    InitialisationComplete,
    Keydown,
    KeydownFirstBackspaceSent,
    KeydownSecondBackspaceSent,
    KeyUp,
    LetterComplete,
    WordComplete
};

class App {
   public:
    /// @brief  Call once at the start of the program
    virtual void setup(HardwareAdapter* hardwareAdapter, SerialAdapter* serialAdapter, Settings* settings,
                       SettingsStorage* settingsStorage, MorseTree* morseTree);

    /// @brief Should be called as often as possible
    virtual void checkKeyerState();

    /// @brief  Should be called every tick interval, as defined in settings::getTickMillis()
    virtual void tick();

   private:
    void setLastEvent(Event newEvent);
    void debounce();
    void appendCharToCurrentSentence(char c);
    void sendBackspace();
    char getTranslatedLetter();
    void log(const char* text);
    void logCurrentSentence();

    HardwareAdapter _hardwareAdapter;
    SerialAdapter* _serialAdapter;
    Settings* _settings;
    MorseTree* _morseTree;

    bool _isLedOn = false;

    Event _lastEvent = Event::InitialisationStarted;
    unsigned long _lastEventTimeMillis = 0;

    char _sentence[MAX_SENTENCE_CHARS] = "";
    char _morseSymbolsInCurrentLetter[MAX_MORSE_SYMBOLS_PER_LETTER] = "";
};