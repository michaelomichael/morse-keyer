#pragma once

#include "hardware-adapter.h"
#include "serial-adapter.h"
#include "settings-internal.h"

/// @brief Responsible for saving and retrieving settings from EEPROM.
class SettingsStorage {
   public:
    inline SettingsStorage(HardwareAdapter* hardwareAdapter) { _hardwareAdapter = hardwareAdapter; }

    inline StoredSettings* get() { return &_settings; }
    virtual bool load();
    virtual void save();
    virtual void print(SerialAdapter* serial);

   private:
    StoredSettings _settings;
    HardwareAdapter* _hardwareAdapter;

    virtual void readEeprom(int address, uint8_t* dest, int length);
    virtual void writeEeprom(int address, uint8_t* src, int length);
};