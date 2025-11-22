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

unsigned long millisPerTick_m = 100;  // 70;
unsigned long debounceMillis_m = 10;

float newLetterThresholdInTicks_m = 7.0;
float dashThresholdInTicks_m = 2.5;
float whooshThresholdInTicks_m = 6.0;
float backspaceRepeatInTicks_m = 3;
float backspacePauseBeforeRepeatInTicks_m = 5;

bool isTickOn_m = false;
unsigned long lastTickTimestamp_m = 0;

unsigned long keyerDownTimestamp_m = 0;

unsigned long lastEventTimeMillis_m = 0;
#define EVENT_NONE -1
#define EVENT_KEYDOWN 0
#define EVENT_KEYDOWN_BACKSPACE_SENT 1
#define EVENT_KEYDOWN_REPEAT_BACKSPACE_SENT 2
int lastEvent_m = EVENT_NONE;

bool hasKeyerBeenReleased_m = false;
unsigned long keyerUpTimestamp_m = 0;

unsigned long newLetterTimestamp_m = millis();
bool isNewLetterTimestampSet_m = true;

int lastKeyerState_m = RELEASED;

String sentence_m = String("");
String morseSymbolsInCurrentLetter_m = String("");

void setup() {
    Serial.begin(9600);
    pinMode(KEYER_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
}

void log(const char* text) { Serial.println(text); }

void logCurrentSentence() {
    Serial.print("Sentence is now: [");
    Serial.print(sentence_m);
    Serial.println("]");
}

void checkKeyerState() {
    int newKeyerState = digitalRead(KEYER_PIN);

    if (newKeyerState == PRESSED && lastKeyerState_m == RELEASED &&
        (millis() - keyerUpTimestamp_m > debounceMillis_m)) {
        // Keyer has just been pressed
        log("Keyer pressed");

        lastEvent_m = EVENT_KEYDOWN;
        lastEventTimeMillis_m = millis();

        keyerDownTimestamp_m = millis();
        isNewLetterTimestampSet_m = false;
        lastKeyerState_m = newKeyerState;

    } else if (newKeyerState == RELEASED && lastKeyerState_m == PRESSED &&
               (millis() - keyerDownTimestamp_m > debounceMillis_m)) {
        log("Keyer released");
        keyerUpTimestamp_m = millis();

        unsigned long millisSinceKeyDown = keyerUpTimestamp_m - keyerDownTimestamp_m;

        if (millisSinceKeyDown > whooshThresholdInTicks_m * millisPerTick_m) {
            log("Backspace!");
            if (sentence_m.length() > 0) {
                sentence_m.remove(sentence_m.length() - 1);
            }
            logCurrentSentence();
        } else {
            bool isDot = (millisSinceKeyDown < dashThresholdInTicks_m * millisPerTick_m);

            char c = isDot ? '.' : '-';
            morseSymbolsInCurrentLetter_m.concat(c);

            Serial.print("Keyer was pressed for ");
            Serial.print(millisSinceKeyDown, DEC);
            Serial.print(" which is a ");
            Serial.println(isDot ? "dot" : "dash");
        }
        isNewLetterTimestampSet_m = false;
        lastKeyerState_m = newKeyerState;
    }
}

void appendCharToCurrentSentence(char c) {
    sentence_m.concat(c);
    logCurrentSentence();
}

char getTranslatedLetter() {
    char letter = getLetterForMorseSymbols(morseSymbolsInCurrentLetter_m.c_str());
    if (letter == NONE) {
        return '%';
    } else {
        return letter;
    }
}

void tick() {
    digitalWrite(LED_PIN, isTickOn_m);
    isTickOn_m = !isTickOn_m;

    if (lastKeyerState_m == RELEASED) {
        unsigned long currentTimestamp = millis();
        unsigned long millisSinceKeyUp = currentTimestamp - keyerUpTimestamp_m;

        unsigned long millisSinceNewLetter = currentTimestamp - newLetterTimestamp_m;

        bool isNewWord = isNewLetterTimestampSet_m &&
                         millisSinceNewLetter >= newLetterThresholdInTicks_m * millisPerTick_m &&
                         millisSinceNewLetter < 8 * millisPerTick_m;

        bool isNewLetter = millisSinceKeyUp > 3 * millisPerTick_m && morseSymbolsInCurrentLetter_m.length() > 0;

        if (isNewWord) {
            if (sentence_m.length() > 0) {
                log("New word");
                appendCharToCurrentSentence(' ');
                morseSymbolsInCurrentLetter_m = String("");
            }
        } else if (isNewLetter) {
            log("New letter");
            char newLetter = getTranslatedLetter();
            appendCharToCurrentSentence(newLetter);
            morseSymbolsInCurrentLetter_m = String("");
            newLetterTimestamp_m = millis();
            isNewLetterTimestampSet_m = true;
        }
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