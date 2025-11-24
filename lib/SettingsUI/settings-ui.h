#pragma once

#include "serial-adapter.h"
#include "settings-internal.h"
#include "settings-storage.h"

class SettingsUi {
   public:
    SettingsUi(SerialAdapter* serial, SettingsStorage* storage);
    void tick();

   private:
    SerialAdapter* _serial;
    SettingsStorage* _storage;

    void _handleListSettingsCommand();
    void _handleSetCommand();
    void _outputSetting(const char* key, bool value);
    void _outputSetting(const char* key, unsigned long value);
    void _outputSetting(const char* key, float value);

    template <typename T>
    bool _validateAndSet(const T* (SerialAdapter::*readValueFunc)(), T* dest);
};
