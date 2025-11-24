#pragma once

#include "settings-internal.h"

class SettingsStorage {
   public:
    SettingsStorage();

    inline StoredSettings* get() { return &_settings; }
    virtual void load();
    virtual void save();

   private:
    StoredSettings _settings;
};