#include "settings-ui.h"

#include <string.h>

namespace {
const char* COMMAND_LIST = "list";
const char* COMMAND_SET = "set";

const char* SETTINGS_KEY_LOGGING_ENABLED = "loggingEnabled";
const char* SETTINGS_KEY_TICK_DURATION_MILLIS = "tickDurationMillis";
const char* SETTINGS_KEY_DEBOUNCE_MILLIS = "debounceMillis";
const char* SETTINGS_KEY_TICKS_BEFORE_NEW_LETTER = "ticksBeforeNewLetter";
const char* SETTINGS_KEY_TICKS_BEFORE_NEW_WORD = "ticksBeforeNewWord";
const char* SETTINGS_KEY_TICKS_BEFORE_DASH = "ticksBeforeDash";
const char* SETTINGS_KEY_TICKS_BEFORE_FIRST_BACKSPACE = "ticksBeforeFirstBackspace";
const char* SETTINGS_KEY_TICKS_BEFORE_SECOND_BACKSPACE = "ticksBeforeSecondBackspace";
const char* SETTINGS_KEY_TICKS_BEFORE_REPEAT_BACKSPACE = "ticksBeforeRepeatBackspace";
const char* SETTINGS_KEY_TONE_VOLUME_PERCENT = "toneVolumePercent";
const char* SETTINGS_KEY_TONE_FREQUENCY = "toneFrequency";
}  // namespace

SettingsUi::SettingsUi(SerialAdapter* serial, SettingsStorage* storage) {
    this->_serial = serial;
    this->_storage = storage;
}

void SettingsUi::tick() {
    while (this->_serial->isLineReady()) {
        const char* command = this->_serial->readWord();

        if (command == nullptr) {
            this->_serial->write("ERR: Empty command.\n");
        } else if (strcmp(command, COMMAND_LIST) == 0) {
            this->_handleListSettingsCommand();
        } else if (strcmp(command, COMMAND_SET) == 0) {
            this->_handleSetCommand();
        } else {
            this->_serial->write("ERR: Unknown command: '");
            this->_serial->write(command);
            this->_serial->write("'.\n");
        }
        this->_serial->skipRestOfLine();
    }
}

void SettingsUi::_handleListSettingsCommand() {
    this->_serial->write("OK: 11\n");
    this->_outputSetting(SETTINGS_KEY_LOGGING_ENABLED, this->_storage->get()->loggingEnabled);
    this->_outputSetting(SETTINGS_KEY_TICK_DURATION_MILLIS, this->_storage->get()->tickDurationMillis);
    this->_outputSetting(SETTINGS_KEY_DEBOUNCE_MILLIS, this->_storage->get()->debounceMillis);
    this->_outputSetting(SETTINGS_KEY_TICKS_BEFORE_NEW_LETTER, this->_storage->get()->ticksBeforeNewLetter);
    this->_outputSetting(SETTINGS_KEY_TICKS_BEFORE_NEW_WORD, this->_storage->get()->ticksBeforeNewWord);
    this->_outputSetting(SETTINGS_KEY_TICKS_BEFORE_DASH, this->_storage->get()->ticksBeforeDash);
    this->_outputSetting(SETTINGS_KEY_TICKS_BEFORE_FIRST_BACKSPACE, this->_storage->get()->ticksBeforeFirstBackspace);
    this->_outputSetting(SETTINGS_KEY_TICKS_BEFORE_SECOND_BACKSPACE, this->_storage->get()->ticksBeforeSecondBackspace);
    this->_outputSetting(SETTINGS_KEY_TICKS_BEFORE_REPEAT_BACKSPACE, this->_storage->get()->ticksBeforeRepeatBackspace);
    this->_outputSetting(SETTINGS_KEY_TONE_VOLUME_PERCENT, this->_storage->get()->toneVolumePercent);
    this->_outputSetting(SETTINGS_KEY_TONE_FREQUENCY, this->_storage->get()->toneFrequency);
}

template <typename T>
bool SettingsUi::_validateAndSet(const T* (SerialAdapter::*readValueFunc)(), T* dest) {
    const T* newValue = (this->_serial->*readValueFunc)();
    if (newValue == nullptr) {
        this->_serial->write("ERR: Invalid value received.\n");
        return false;
    } else {
        *dest = *newValue;
        this->_storage->save();
        this->_serial->write("OK\n");
        return true;
    }
}

void SettingsUi::_handleSetCommand() {
    const char* settingKey = this->_serial->readWord();
    if (strcmp(settingKey, SETTINGS_KEY_LOGGING_ENABLED) == 0) {
        this->_validateAndSet(&SerialAdapter::readBool, &(this->_storage->get()->loggingEnabled));
    } else if (strcmp(settingKey, SETTINGS_KEY_TICK_DURATION_MILLIS) == 0) {
        this->_validateAndSet(&SerialAdapter::readUnsignedLong, &(this->_storage->get()->tickDurationMillis));
    } else if (strcmp(settingKey, SETTINGS_KEY_DEBOUNCE_MILLIS) == 0) {
        this->_validateAndSet(&SerialAdapter::readUnsignedLong, &(this->_storage->get()->debounceMillis));
    } else if (strcmp(settingKey, SETTINGS_KEY_TICKS_BEFORE_NEW_LETTER) == 0) {
        this->_validateAndSet(&SerialAdapter::readFloat, &(this->_storage->get()->ticksBeforeNewLetter));
    } else if (strcmp(settingKey, SETTINGS_KEY_TICKS_BEFORE_NEW_WORD) == 0) {
        this->_validateAndSet(&SerialAdapter::readFloat, &(this->_storage->get()->ticksBeforeNewWord));
    } else if (strcmp(settingKey, SETTINGS_KEY_TICKS_BEFORE_DASH) == 0) {
        this->_validateAndSet(&SerialAdapter::readFloat, &(this->_storage->get()->ticksBeforeDash));
    } else if (strcmp(settingKey, SETTINGS_KEY_TICKS_BEFORE_FIRST_BACKSPACE) == 0) {
        this->_validateAndSet(&SerialAdapter::readFloat, &(this->_storage->get()->ticksBeforeFirstBackspace));
    } else if (strcmp(settingKey, SETTINGS_KEY_TICKS_BEFORE_SECOND_BACKSPACE) == 0) {
        this->_validateAndSet(&SerialAdapter::readFloat, &(this->_storage->get()->ticksBeforeSecondBackspace));
    } else if (strcmp(settingKey, SETTINGS_KEY_TICKS_BEFORE_REPEAT_BACKSPACE) == 0) {
        this->_validateAndSet(&SerialAdapter::readFloat, &(this->_storage->get()->ticksBeforeRepeatBackspace));
    } else if (strcmp(settingKey, SETTINGS_KEY_TONE_VOLUME_PERCENT) == 0) {
        this->_validateAndSet(&SerialAdapter::readUnsignedLong, &(this->_storage->get()->toneVolumePercent));
    } else if (strcmp(settingKey, SETTINGS_KEY_TONE_FREQUENCY) == 0) {
        this->_validateAndSet(&SerialAdapter::readUnsignedLong, &(this->_storage->get()->toneFrequency));
    } else {
        this->_serial->write("ERR: Unknown setting key '");
        this->_serial->write(settingKey);
        this->_serial->write("'\n");
    }
}

void SettingsUi::_outputSetting(const char* key, bool value) {
    this->_serial->write("  ");
    this->_serial->write(key);
    this->_serial->write(" = ");
    this->_serial->writeBool(value);
    this->_serial->write("\n");
}

void SettingsUi::_outputSetting(const char* key, float value) {
    this->_serial->write("  ");
    this->_serial->write(key);
    this->_serial->write(" = ");
    this->_serial->writeFloat(value);
    this->_serial->write("\n");
}

void SettingsUi::_outputSetting(const char* key, unsigned long value) {
    this->_serial->write("  ");
    this->_serial->write(key);
    this->_serial->write(" = ");
    this->_serial->writeUnsignedLong(value);
    this->_serial->write("\n");
}