#if defined ARDUINO

#include <Arduino.h>

#include "app.h"
#include "hardware-adapter.h"
#include "serial-adapter.h"
#include "settings-cli.h"
#include "settings.h"

SerialAdapter _serialAdapter(&Serial);
HardwareAdapter _hardwareAdapter;
SettingsStorage _settingsStorage;
Settings _settings(&_settingsStorage);
SettingsCli _settingsCli(&_serialAdapter, &_settingsStorage);
App _app;

unsigned long _lastTickTimestamp = 0;

void setup() {
    _serialAdapter.begin();
    _hardwareAdapter.begin();
    _settingsStorage.load();
    _settingsStorage.print(&_serialAdapter);

    _app.setup(&_hardwareAdapter, &_serialAdapter, &_settings, &_settingsStorage);
}

void loop() {
    _settingsCli.tick();
    unsigned long currentTimestamp = _hardwareAdapter.millis();

    if (currentTimestamp - _lastTickTimestamp > _settings.getTickMillis()) {
        _lastTickTimestamp = currentTimestamp;
        _app.tick();
    }

    _app.checkKeyerState();
}

#else

int main() {}
#endif