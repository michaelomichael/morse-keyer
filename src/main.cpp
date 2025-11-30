#if defined ARDUINO

#include <Arduino.h>

#include "morse-tree.h"
#include "settings-cli.h"
#include "settings.h"

#define KEYER_PIN 10
#define SPEAKER_PIN 9
#define LED_PIN LED_BUILTIN
#define MAX_SENTENCE_CHARS 100
#define MAX_MORSE_SYMBOLS_PER_LETTER 10
#define PRESSED LOW
#define RELEASED HIGH

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

bool isTickOn_m = false;
unsigned long lastTickTimestamp_m = 0;

Event lastEvent_m = Event::InitialisationStarted;
unsigned long lastEventTimeMillis_m = millis();

String sentence_m = String("");
String morseSymbolsInCurrentLetter_m = String("");

SerialAdapter serialAdapter(&Serial);
SettingsStorage settingsStorage_m;
Settings settings_m(&settingsStorage_m);

SettingsCli settingsCli_m(&serialAdapter, &settingsStorage_m);

void log(const char* text) {
    if (settings_m.getLoggingEnabled()) {
        Serial.println(text);
    }
}

void setLastEvent(Event newEvent) {
    lastEvent_m = newEvent;
    lastEventTimeMillis_m = millis();
}

void logCurrentSentence() {
    if (settings_m.getLoggingEnabled()) {
        Serial.print("Sentence is now: [");
        Serial.print(sentence_m);
        Serial.println("]");
    }
}

void sendBackspace() {
    if (sentence_m.length() > 0) {
        sentence_m.remove(sentence_m.length() - 1);
        logCurrentSentence();
    }
}

void debounce() { delay(settings_m.getDebounceMillis()); }

void checkKeyerState() {
    int newKeyerState = digitalRead(KEYER_PIN);

    unsigned long millisSinceLastEvent = millis() - lastEventTimeMillis_m;

    if (newKeyerState == PRESSED) {
        if (lastEvent_m == Event::KeyUp || lastEvent_m == Event::LetterComplete || lastEvent_m == Event::WordComplete ||
            lastEvent_m == Event::InitialisationComplete) {
            log("Keyer pressed");

            tone(SPEAKER_PIN, settings_m.getToneFrequencyHertz());

            setLastEvent(Event::Keydown);
            debounce();
        } else if (lastEvent_m == Event::Keydown && millisSinceLastEvent > settings_m.getFirstBackspaceMillis()) {
            log("Initial backspace");
            noTone(SPEAKER_PIN);
            sendBackspace();
            debounce();
            setLastEvent(Event::KeydownFirstBackspaceSent);
        } else if (lastEvent_m == Event::KeydownFirstBackspaceSent &&
                   millisSinceLastEvent > settings_m.getRepeatBackspaceMillis()) {
            // TODO - add an intermediate pause check between the first backspace and the repeats,
            //        so we get a chance to release before the repeats kick in, but also can do
            //        fast repeats once they get going.
            log("Repeat backspace");
            sendBackspace();
            setLastEvent(Event::KeydownFirstBackspaceSent);
        }
    } else {
        if (lastEvent_m == Event::Keydown) {
            log("Keyer released");
            noTone(SPEAKER_PIN);

            bool isDot = (millisSinceLastEvent < settings_m.getDashMillis());

            if (morseSymbolsInCurrentLetter_m.length() > MAX_MORSE_SYMBOLS_PER_LETTER) {
                log("Too many symbols added for this letter already. Will discard this one.");
            } else {
                morseSymbolsInCurrentLetter_m.concat(isDot ? '.' : '-');
            }

            if (settings_m.getLoggingEnabled()) {
                Serial.print("Keyer was pressed for ");
                Serial.print(millisSinceLastEvent, DEC);
                Serial.print(" which is a ");
                Serial.print(isDot ? "dot" : "dash");
                Serial.print(". Current word is [");
                Serial.print(morseSymbolsInCurrentLetter_m);
                Serial.println("].");
            }

            setLastEvent(Event::KeyUp);
            debounce();
        } else if (lastEvent_m == Event::KeydownFirstBackspaceSent) {
            log("Keyer released during backspacing");

            setLastEvent(Event::WordComplete);
            debounce();
        }
    }
}

void appendCharToCurrentSentence(char c) {
    if (sentence_m.length() > MAX_SENTENCE_CHARS) {
        sentence_m = String("");
    }
    sentence_m.concat(c);
    logCurrentSentence();
}

char getTranslatedLetter() {
    char letter = getLetterForMorseSymbols(morseSymbolsInCurrentLetter_m.c_str());
    return letter == NONE ? '%' : letter;
}

void tick() {
    analogWrite(LED_PIN, isTickOn_m ? 0 : 20);
    isTickOn_m = !isTickOn_m;

    unsigned long currentTimestamp = millis();
    unsigned long millisSinceLastEvent = currentTimestamp - lastEventTimeMillis_m;

    if (lastEvent_m == Event::KeyUp && millisSinceLastEvent > settings_m.getNewLetterMillis()) {
        log("New letter");
        char newLetter = getTranslatedLetter();
        appendCharToCurrentSentence(newLetter);
        morseSymbolsInCurrentLetter_m = String("");
        setLastEvent(Event::LetterComplete);
    } else if (lastEvent_m == Event::LetterComplete && millisSinceLastEvent > settings_m.getNewWordMillis()) {
        log("New word");

        appendCharToCurrentSentence(' ');
        setLastEvent(Event::WordComplete);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(KEYER_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    while (!Serial) {
        ;  // wait for serial port to connect. Needed for native USB
    }
    settingsStorage_m.load();
    settingsStorage_m.print(&serialAdapter);

    if (digitalRead(KEYER_PIN) == PRESSED) {
        StoredSettings* settings = settingsStorage_m.get();

        if (settings->toneEnabled) {
            log("Toggling the tone volume to OFF");
            settings->toneEnabled = false;
        } else {
            log("Toggling the tone volume to ON");
            settings->toneEnabled = true;
        }
        settingsStorage_m.save();

        log("Waiting for keyer to be released before continuing");
        while (digitalRead(KEYER_PIN) == PRESSED);
        debounce();
    }

    log("Initialisation complete!");
    setLastEvent(Event::InitialisationComplete);
}

void loop() {
    settingsCli_m.tick();
    unsigned long currentTimestamp = millis();

    if (currentTimestamp - lastTickTimestamp_m > settings_m.getTickMillis()) {
        lastTickTimestamp_m = currentTimestamp;
        tick();
    }

    checkKeyerState();
}

#else

int main() {}
#endif