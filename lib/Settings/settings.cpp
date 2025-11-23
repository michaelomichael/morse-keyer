#include "settings.h"

#include "settings-internal.h"
#include "settings-storage.h"

namespace {
// Private variables
Settings settings;
}  // namespace

Settings* initSettings() {
    StoredSettings* storedSettings = initStoredSettings();

    // clang-format off
    settings.loggingEnabled        = true;
    settings.tickMillis            = storedSettings->tickDurationMillis;
    settings.debounceMillis        = storedSettings->debounceMillis;
    settings.newLetterMillis       = storedSettings->ticksBeforeNewLetter * settings.tickMillis;
    settings.newWordMillis         = storedSettings->ticksBeforeNewWord * settings.tickMillis;
    settings.dashMillis            = storedSettings->ticksBeforeDash * settings.tickMillis;
    settings.firstBackspaceMillis       = storedSettings->ticksBeforeFirstBackspace * settings.tickMillis;
    settings.secondBackspaceMillis = storedSettings->ticksBeforeSecondBackspace * settings.tickMillis;
    settings.repeatBackspaceMillis = storedSettings->ticksBeforeRepeatBackspace * settings.tickMillis;
    // clang-format on

    return &settings;
}