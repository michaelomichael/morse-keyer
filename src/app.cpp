#include "app.h"

void App::setup(HardwareAdapter* hardwareAdapter, SerialAdapter* serialAdapter, Settings* settings,
                SettingsStorage* settingsStorage, MorseTree* morseTree) {
    _hardwareAdapter = hardwareAdapter;
    _serialAdapter = serialAdapter;
    _settings = settings;
    _morseTree = morseTree;

    if (_hardwareAdapter->isKeyerPressed()) {
        StoredSettings* settings = settingsStorage->get();

        if (settings->toneEnabled) {
            log("Toggling the tone volume to OFF");
            settings->toneEnabled = false;
        } else {
            log("Toggling the tone volume to ON");
            settings->toneEnabled = true;
        }
        settingsStorage->save();

        log("Waiting for keyer to be released before continuing");
        while (_hardwareAdapter->isKeyerPressed());
        debounce();
    }

    if (settings->getToneEnabled()) {
        // Emit 2 short beeps to indicate that the tone is enabled
        const int beepDurationMillis = 300;

        for (int i = 0; i < 2; i++) {
            _hardwareAdapter->tone(settings->getToneFrequencyHertz(), settings->getToneVolumePercent());
            _hardwareAdapter->delay(beepDurationMillis);
            _hardwareAdapter->noTone();
            _hardwareAdapter->delay(beepDurationMillis);
        }
    }

    log("Initialisation complete!");
    setLastEvent(Event::InitialisationComplete);
}

void App::tick() {
    _hardwareAdapter->writeLed(_isLedOn);
    _isLedOn = !_isLedOn;

    unsigned long currentTimestamp = _hardwareAdapter->millis();
    unsigned long millisSinceLastEvent = currentTimestamp - _lastEventTimeMillis;

    if (_lastEvent == Event::KeyUp && millisSinceLastEvent > _settings->getNewLetterMillis()) {
        log("New letter");
        char newLetter = getTranslatedLetter();
        appendCharToCurrentSentence(newLetter);
        strcpy(_morseSymbolsInCurrentLetter, "");
        setLastEvent(Event::LetterComplete);
    } else if (_lastEvent == Event::LetterComplete && millisSinceLastEvent > _settings->getNewWordMillis()) {
        log("New word");

        appendCharToCurrentSentence(' ');
        setLastEvent(Event::WordComplete);
    }
}

void App::log(const char* text) {
    if (_settings->getLoggingEnabled()) {
        _serialAdapter->write(text);
        _serialAdapter->write("\n");
    }
}

void App::setLastEvent(Event newEvent) {
    _lastEvent = newEvent;
    _lastEventTimeMillis = millis();
}

void App::logCurrentSentence() {
    if (_settings->getLoggingEnabled()) {
        _serialAdapter->write("Sentence is now: [");
        _serialAdapter->write(_sentence);
        _serialAdapter->write("]\n");
    }
}

void App::sendBackspace() {
    const int len = strnlen(_sentence, MAX_SENTENCE_CHARS);
    if (len > 0) {
        _sentence[len - 1] = '\0';
        logCurrentSentence();
    }

    if (_settings->getKeyboardEnabled()) {
        _hardwareAdapter->keyboardBackspace();
    }
}

void App::debounce() { _hardwareAdapter->delay(_settings->getDebounceMillis()); }

void App::checkKeyerState() {
    unsigned long millisSinceLastEvent = _hardwareAdapter->millis() - _lastEventTimeMillis;

    if (_hardwareAdapter->isKeyerPressed()) {
        if (_lastEvent == Event::KeyUp || _lastEvent == Event::LetterComplete || _lastEvent == Event::WordComplete ||
            _lastEvent == Event::InitialisationComplete) {
            log("Keyer pressed");

            if (_settings->getToneEnabled()) {
                _hardwareAdapter->tone(_settings->getToneFrequencyHertz(), _settings->getToneVolumePercent());
            }

            setLastEvent(Event::Keydown);
            debounce();
        } else if (_lastEvent == Event::Keydown && millisSinceLastEvent > _settings->getFirstBackspaceMillis()) {
            log("Initial backspace");
            _hardwareAdapter->noTone();
            sendBackspace();
            debounce();
            setLastEvent(Event::KeydownFirstBackspaceSent);
        } else if (_lastEvent == Event::KeydownFirstBackspaceSent &&
                   millisSinceLastEvent > _settings->getRepeatBackspaceMillis()) {
            // TODO - add an intermediate pause check between the first backspace and the repeats,
            //        so we get a chance to release before the repeats kick in, but also can do
            //        fast repeats once they get going.
            log("Repeat backspace");
            sendBackspace();
            setLastEvent(Event::KeydownFirstBackspaceSent);
        }
    } else {
        if (_lastEvent == Event::Keydown) {
            log("Keyer released");
            _hardwareAdapter->noTone();

            bool isDot = (millisSinceLastEvent < _settings->getDashMillis());

            if (strlen(_morseSymbolsInCurrentLetter) >= MAX_MORSE_SYMBOLS_PER_LETTER - 1) {
                log("Too many symbols added for this letter already. Will discard this one.");
            } else {
                strcat(_morseSymbolsInCurrentLetter, isDot ? "." : "-");
            }

            if (_settings->getLoggingEnabled()) {
                _serialAdapter->write("Keyer was pressed for ");
                _serialAdapter->writeUnsignedLong(millisSinceLastEvent);
                _serialAdapter->write(" which is a ");
                _serialAdapter->write(isDot ? "dot" : "dash");
                _serialAdapter->write(". Current word is [");
                _serialAdapter->write(_morseSymbolsInCurrentLetter);
                _serialAdapter->write("].\n");
            }

            setLastEvent(Event::KeyUp);
            debounce();
        } else if (_lastEvent == Event::KeydownFirstBackspaceSent) {
            log("Keyer released during backspacing");

            setLastEvent(Event::WordComplete);
            debounce();
        }
    }
}

void App::appendCharToCurrentSentence(char c) {
    int len = strnlen(_sentence, MAX_SENTENCE_CHARS);
    if (len >= MAX_SENTENCE_CHARS - 1) {
        strcpy(_sentence, "");
        len = 0;
    }

    _sentence[len] = c;
    _sentence[len + 1] = '\0';
    logCurrentSentence();

    if (_settings->getKeyboardEnabled()) {
        char mappedChar = _settings->getKeyboardLayout()->remap(c);
        _hardwareAdapter->keyboardType(mappedChar);
    }
}

char App::getTranslatedLetter() {
    char letter = _morseTree->getLetterForMorseSymbols(_morseSymbolsInCurrentLetter);
    if (letter == MorseTree::Incomplete) {
        return '%';
    } else if (letter == MorseTree::Error) {
        return '#';
    } else {
        return letter;
    }
}
