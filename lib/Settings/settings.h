#pragma once

#include "settings-storage.h"

/// @brief A wrapper around the StoredSettings, translating ticks into milliseconds.
class Settings {
   public:
    inline Settings(SettingsStorage* storage) { this->_storage = storage; }

    inline bool getLoggingEnabled() const { return _storage->get()->loggingEnabled; }
    inline unsigned long getTickMillis() const { return _storage->get()->tickDurationMillis; }
    inline unsigned long getDebounceMillis() const { return _storage->get()->debounceMillis; }

    inline unsigned long getNewLetterMillis() const {
        return this->getTickMillis() * _storage->get()->ticksBeforeNewLetter;
    }
    inline unsigned long getNewWordMillis() const {
        return this->getTickMillis() * _storage->get()->ticksBeforeNewWord;
    }

    inline unsigned long getDashMillis() const { return this->getTickMillis() * _storage->get()->ticksBeforeDash; }

    inline unsigned long getFirstBackspaceMillis() const {
        return this->getTickMillis() * _storage->get()->ticksBeforeFirstBackspace;
    }

    inline unsigned long getSecondBackspaceMillis() const {
        return this->getTickMillis() * _storage->get()->ticksBeforeSecondBackspace;
    }

    inline unsigned long getRepeatBackspaceMillis() const {
        return this->getTickMillis() * _storage->get()->ticksBeforeRepeatBackspace;
    }

    inline bool getToneEnabled() const { return _storage->get()->toneEnabled; }
    inline unsigned int getToneVolumePercent() const { return _storage->get()->toneVolumePercent; }
    inline unsigned int getToneFrequencyHertz() const { return _storage->get()->toneFrequencyHertz; }

   private:
    SettingsStorage* _storage;
};