#if defined ARDUINO

// #include "ArduinoWrapper.h"

#include <Arduino.h>

#include "morse-tree.h"

#define KEYER_PIN 10
#define LED_PIN LED_BUILTIN
#define MAX_SENTENCE_CHARS 100
#define MAX_MORSE_SYMBOLS_PER_LETTER 10
#define PRESSED LOW
#define RELEASED HIGH

enum class Event { Keydown, KeydownBackspaceSent, KeyUp, LetterComplete, WordComplete };

// Settings
unsigned long millisPerTick_m = 70;
unsigned long debounceMillis_m = 10;
float newLetterThresholdInTicks_m = 7.0;
float newWordThresholdInTicks_m = 8.0;
float dashThresholdInTicks_m = 2.5;
float backspaceRepeatInTicks_m = 7;
float backspacePauseBeforeRepeatInTicks_m = 20;

bool isTickOn_m = false;
unsigned long lastTickTimestamp_m = 0;

Event lastEvent_m = Event::WordComplete;
unsigned long lastEventTimeMillis_m = 0;

String sentence_m = String("");
String morseSymbolsInCurrentLetter_m = String("");

void setup() {
    Serial.begin(9600);
    pinMode(KEYER_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
}

void log(const char* text) { Serial.println(text); }

void setLastEvent(Event newEvent) {
    lastEvent_m = newEvent;
    lastEventTimeMillis_m = millis();
}

void logCurrentSentence() {
    Serial.print("Sentence is now: [");
    Serial.print(sentence_m);
    Serial.println("]");
}

void sendBackspace() {
    if (sentence_m.length() > 0) {
        sentence_m.remove(sentence_m.length() - 1);
        logCurrentSentence();
    }
}

void debounce() { delay(debounceMillis_m); }

void checkKeyerState() {
    int newKeyerState = digitalRead(KEYER_PIN);

    unsigned long millisSinceLastEvent = millis() - lastEventTimeMillis_m;

    if (newKeyerState == PRESSED) {
        if (lastEvent_m == Event::KeyUp || lastEvent_m == Event::LetterComplete || lastEvent_m == Event::WordComplete) {
            log("Keyer pressed");

            setLastEvent(Event::Keydown);
            debounce();
        } else if (lastEvent_m == Event::Keydown &&
                   millisSinceLastEvent > backspacePauseBeforeRepeatInTicks_m * millisPerTick_m) {
            log("Initial backspace");

            sendBackspace();
            debounce();
            setLastEvent(Event::KeydownBackspaceSent);
        } else if (lastEvent_m == Event::KeydownBackspaceSent &&
                   millisSinceLastEvent > backspaceRepeatInTicks_m * millisPerTick_m) {
            log("Repeat backspace");
            sendBackspace();
            setLastEvent(Event::KeydownBackspaceSent);
        }
    } else {
        if (lastEvent_m == Event::Keydown) {
            log("Keyer released");

            bool isDot = (millisSinceLastEvent < dashThresholdInTicks_m * millisPerTick_m);
            morseSymbolsInCurrentLetter_m.concat(isDot ? '.' : '-');

            Serial.print("Keyer was pressed for ");
            Serial.print(millisSinceLastEvent, DEC);
            Serial.print(" which is a ");
            Serial.print(isDot ? "dot" : "dash");
            Serial.print(". Current word is [");
            Serial.print(morseSymbolsInCurrentLetter_m);
            Serial.println("].");

            setLastEvent(Event::KeyUp);
            debounce();
        } else if (lastEvent_m == Event::KeydownBackspaceSent) {
            log("Keyer released during backspacing");

            setLastEvent(Event::WordComplete);
            debounce();
        }
    }
}

void appendCharToCurrentSentence(char c) {
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

    if (lastEvent_m == Event::KeyUp && millisSinceLastEvent > newLetterThresholdInTicks_m * millisPerTick_m) {
        log("New letter");
        char newLetter = getTranslatedLetter();
        appendCharToCurrentSentence(newLetter);
        morseSymbolsInCurrentLetter_m = String("");
        setLastEvent(Event::LetterComplete);
    } else if (lastEvent_m == Event::LetterComplete &&
               millisSinceLastEvent > newWordThresholdInTicks_m * millisPerTick_m) {
        log("New word");

        appendCharToCurrentSentence(' ');
        setLastEvent(Event::WordComplete);
    }
}

void loop() {
    unsigned long currentTimestamp = millis();

    if (currentTimestamp - lastTickTimestamp_m > millisPerTick_m) {
        lastTickTimestamp_m = currentTimestamp;
        tick();
    }

    checkKeyerState();
}

#else

int main() {}
#endif