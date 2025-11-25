#pragma once

#include <inttypes.h>

#include "settings-internal.h"

class SettingsStorage {
   public:
    inline SettingsStorage() {}

    inline StoredSettings* get() { return &_settings; }
    virtual bool load();
    virtual void save();

   protected:
    virtual void readEeprom(int address, uint8_t* dest, int length);
    virtual void writeEeprom(int address, uint8_t* src, int length);
    virtual uint8_t readEeprom(int address);
    virtual void writeEeprom(int address, uint8_t value);

   private:
    StoredSettings _settings;
};