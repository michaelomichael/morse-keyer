#pragma once

#include "serial-adapter.h"
#include "settings-internal.h"
#include "settings-storage.h"

/// @brief Handles getting and setting of configuration values via the serial interface.
class SettingsCli {
   public:
    SettingsCli(SerialAdapter* serial, SettingsStorage* storage);
    void tick();

   private:
    SerialAdapter* _serial;
    SettingsStorage* _storage;

    void handleListCommand();
    void handleSetCommand();
    void outputSetting(const char* key, bool value);
    void outputSetting(const char* key, unsigned int value);
    void outputSetting(const char* key, float value);

    template <typename T>
    bool validateAndSet(const T* (SerialAdapter::*readValueFunc)(), T* dest);
};
