#include "settings-cli.h"

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
const char* SETTINGS_KEY_TONE_ENABLED = "toneEnabled";
const char* SETTINGS_KEY_TONE_VOLUME_PERCENT = "toneVolumePercent";
const char* SETTINGS_KEY_TONE_FREQUENCY_HERTZ = "toneFrequencyHertz";
}  // namespace

SettingsCli::SettingsCli(SerialAdapter* serial, SettingsStorage* storage) {
    this->_serial = serial;
    this->_storage = storage;
}

void SettingsCli::tick() {
    while (this->_serial->isLineReady()) {
        const char* command = this->_serial->readWord();

        if (command == nullptr) {
            this->_serial->write("ERR: Empty command.\n");
        } else if (strcmp(command, COMMAND_LIST) == 0) {
            this->_handleListCommand();
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

void SettingsCli::_handleListCommand() {
    this->_serial->write("OK: 12\n");
    this->_outputSetting(SETTINGS_KEY_LOGGING_ENABLED, this->_storage->get()->loggingEnabled);
    this->_outputSetting(SETTINGS_KEY_TICK_DURATION_MILLIS, this->_storage->get()->tickDurationMillis);
    this->_outputSetting(SETTINGS_KEY_DEBOUNCE_MILLIS, this->_storage->get()->debounceMillis);
    this->_outputSetting(SETTINGS_KEY_TICKS_BEFORE_NEW_LETTER, this->_storage->get()->ticksBeforeNewLetter);
    this->_outputSetting(SETTINGS_KEY_TICKS_BEFORE_NEW_WORD, this->_storage->get()->ticksBeforeNewWord);
    this->_outputSetting(SETTINGS_KEY_TICKS_BEFORE_DASH, this->_storage->get()->ticksBeforeDash);
    this->_outputSetting(SETTINGS_KEY_TICKS_BEFORE_FIRST_BACKSPACE, this->_storage->get()->ticksBeforeFirstBackspace);
    this->_outputSetting(SETTINGS_KEY_TICKS_BEFORE_SECOND_BACKSPACE, this->_storage->get()->ticksBeforeSecondBackspace);
    this->_outputSetting(SETTINGS_KEY_TICKS_BEFORE_REPEAT_BACKSPACE, this->_storage->get()->ticksBeforeRepeatBackspace);
    this->_outputSetting(SETTINGS_KEY_TONE_ENABLED, this->_storage->get()->toneEnabled);
    this->_outputSetting(SETTINGS_KEY_TONE_VOLUME_PERCENT, this->_storage->get()->toneVolumePercent);
    this->_outputSetting(SETTINGS_KEY_TONE_FREQUENCY_HERTZ, this->_storage->get()->toneFrequencyHertz);
}

template <typename T>
bool SettingsCli::_validateAndSet(const T* (SerialAdapter::*readValueFunc)(), T* dest) {
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

void SettingsCli::_handleSetCommand() {
    const char* settingKey = this->_serial->readWord();
    if (strcmp(settingKey, SETTINGS_KEY_LOGGING_ENABLED) == 0) {
        this->_validateAndSet(&SerialAdapter::readBool, &(this->_storage->get()->loggingEnabled));
    } else if (strcmp(settingKey, SETTINGS_KEY_TICK_DURATION_MILLIS) == 0) {
        this->_validateAndSet(&SerialAdapter::readUnsignedInt, &(this->_storage->get()->tickDurationMillis));
    } else if (strcmp(settingKey, SETTINGS_KEY_DEBOUNCE_MILLIS) == 0) {
        this->_validateAndSet(&SerialAdapter::readUnsignedInt, &(this->_storage->get()->debounceMillis));
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
    } else if (strcmp(settingKey, SETTINGS_KEY_TONE_ENABLED) == 0) {
        this->_validateAndSet(&SerialAdapter::readBool, &(this->_storage->get()->toneEnabled));
    } else if (strcmp(settingKey, SETTINGS_KEY_TONE_VOLUME_PERCENT) == 0) {
        this->_validateAndSet(&SerialAdapter::readUnsignedInt, &(this->_storage->get()->toneVolumePercent));
    } else if (strcmp(settingKey, SETTINGS_KEY_TONE_FREQUENCY_HERTZ) == 0) {
        this->_validateAndSet(&SerialAdapter::readUnsignedInt, &(this->_storage->get()->toneFrequencyHertz));
    } else {
        this->_serial->write("ERR: Unknown setting key '");
        this->_serial->write(settingKey);
        this->_serial->write("'\n");
    }
}

void SettingsCli::_outputSetting(const char* key, bool value) {
    this->_serial->write("  ");
    this->_serial->write(key);
    this->_serial->write(" = ");
    this->_serial->writeBool(value);
    this->_serial->write("\n");
}

void SettingsCli::_outputSetting(const char* key, float value) {
    this->_serial->write("  ");
    this->_serial->write(key);
    this->_serial->write(" = ");
    this->_serial->writeFloat(value);
    this->_serial->write("\n");
}

void SettingsCli::_outputSetting(const char* key, unsigned int value) {
    this->_serial->write("  ");
    this->_serial->write(key);
    this->_serial->write(" = ");
    this->_serial->writeUnsignedInt(value);
    this->_serial->write("\n");
}