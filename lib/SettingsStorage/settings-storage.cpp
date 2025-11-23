#include "settings-storage.h"

// private variables
namespace {
StoredSettings storedSettings;
}  // namespace

StoredSettings* initStoredSettings() { return &storedSettings; }