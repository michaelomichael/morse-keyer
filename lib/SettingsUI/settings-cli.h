#pragma once

#include "serial-adapter.h"
#include "settings-internal.h"
#include "settings-storage.h"

class SettingsCli {
   public:
    SettingsCli(SerialAdapter* serial, SettingsStorage* storage);
    void tick();

   private:
    SerialAdapter* _serial;
    SettingsStorage* _storage;

    void _handleListCommand();
    void _handleSetCommand();
    void _outputSetting(const char* key, bool value);
    void _outputSetting(const char* key, unsigned int value);
    void _outputSetting(const char* key, float value);

    template <typename T>
    bool _validateAndSet(const T* (SerialAdapter::*readValueFunc)(), T* dest);
};
