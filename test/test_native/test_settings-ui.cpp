#include <gmock/gmock.h>

#include "serial-adapter.h"
#include "settings-cli.h"

using ::testing::InSequence;
using ::testing::Return;

class MockSettingsStorage : public SettingsStorage {
   public:
    MockSettingsStorage() : SettingsStorage(nullptr) {}
    MOCK_METHOD(bool, load, (), (override));
    MOCK_METHOD(void, save, (), (override));
};

class MockSerialAdapter : public SerialAdapter {
   public:
    MockSerialAdapter() : SerialAdapter(nullptr) {}
    MOCK_METHOD(bool, isLineReady, (), (override));
    MOCK_METHOD(const char*, readWord, (), (override));
    MOCK_METHOD(bool*, readBool, (), (override));
    MOCK_METHOD(float*, readFloat, (), (override));
    MOCK_METHOD(unsigned int*, readUnsignedInt, (), (override));
    MOCK_METHOD(bool, skipRestOfLine, (), (override));
    MOCK_METHOD(void, write, (const char* str), (override));
    MOCK_METHOD(void, writeBool, (bool value), (override));
    MOCK_METHOD(void, writeFloat, (float value), (override));
    MOCK_METHOD(void, writeUnsignedInt, (unsigned int value), (override));
};

void initToKnownValues(StoredSettings* settings) {
    settings->loggingEnabled = true;
    settings->tickDurationMillis = 70;
    settings->debounceMillis = 10;
    settings->ticksBeforeNewLetter = 7.0;
    settings->ticksBeforeNewWord = 8.0;
    settings->ticksBeforeDash = 2.5;
    settings->ticksBeforeFirstBackspace = 20.0;
    settings->ticksBeforeSecondBackspace = 13.0;
    settings->ticksBeforeRepeatBackspace = 7.0;
    settings->toneEnabled = true;
    settings->toneVolumePercent = 50;
    settings->toneFrequencyHertz = 1220;
    settings->keyboardEnabled = true;
    settings->keyboardLayout = 0;
}

TEST(ListSettingsCommand, ShouldInitializeAndTick) {
    MockSerialAdapter adapter;
    SettingsCli underTest(&adapter, nullptr);

    EXPECT_CALL(adapter, isLineReady()).Times(1).WillRepeatedly(Return(false));

    underTest.tick();
}

TEST(UnknownCommand, ShouldPrintError) {
    MockSerialAdapter adapter;
    SettingsCli underTest(&adapter, nullptr);

    {
        InSequence seq;
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(true));
        EXPECT_CALL(adapter, readWord()).WillOnce(Return((const char*)"unknown_command"));
        EXPECT_CALL(adapter, skipRestOfLine()).WillOnce(Return(true));
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(false));
    }
    {
        InSequence seq;
        EXPECT_CALL(adapter, write("ERR: Unknown command: '"));
        EXPECT_CALL(adapter, write("unknown_command"));
        EXPECT_CALL(adapter, write("'.\n"));
    }

    underTest.tick();
}

TEST(ListSettings, ShouldPrintSettings) {
    MockSerialAdapter adapter;
    MockSettingsStorage storage;  // Was previously non-mock
    initToKnownValues(storage.get());
    SettingsCli underTest(&adapter, &storage);
    const char* command = "list";

    {
        InSequence seq;
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(true));
        EXPECT_CALL(adapter, readWord()).WillOnce(Return(command));
        EXPECT_CALL(adapter, skipRestOfLine()).WillOnce(Return(true));
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(false));
    }
    {
        InSequence seq;
        EXPECT_CALL(adapter, write("OK: 14\n"));

        EXPECT_CALL(adapter, write("  "));
        EXPECT_CALL(adapter, write("loggingEnabled"));
        EXPECT_CALL(adapter, write(" = "));
        EXPECT_CALL(adapter, writeBool(true));
        EXPECT_CALL(adapter, write("\n"));

        EXPECT_CALL(adapter, write("  "));
        EXPECT_CALL(adapter, write("tickDurationMillis"));
        EXPECT_CALL(adapter, write(" = "));
        EXPECT_CALL(adapter, writeUnsignedInt(70));
        EXPECT_CALL(adapter, write("\n"));

        EXPECT_CALL(adapter, write("  "));
        EXPECT_CALL(adapter, write("debounceMillis"));
        EXPECT_CALL(adapter, write(" = "));
        EXPECT_CALL(adapter, writeUnsignedInt(10));
        EXPECT_CALL(adapter, write("\n"));

        EXPECT_CALL(adapter, write("  "));
        EXPECT_CALL(adapter, write("ticksBeforeNewLetter"));
        EXPECT_CALL(adapter, write(" = "));
        EXPECT_CALL(adapter, writeFloat(7.0));
        EXPECT_CALL(adapter, write("\n"));

        EXPECT_CALL(adapter, write("  "));
        EXPECT_CALL(adapter, write("ticksBeforeNewWord"));
        EXPECT_CALL(adapter, write(" = "));
        EXPECT_CALL(adapter, writeFloat(8.0));
        EXPECT_CALL(adapter, write("\n"));

        EXPECT_CALL(adapter, write("  "));
        EXPECT_CALL(adapter, write("ticksBeforeDash"));
        EXPECT_CALL(adapter, write(" = "));
        EXPECT_CALL(adapter, writeFloat(2.5));
        EXPECT_CALL(adapter, write("\n"));

        EXPECT_CALL(adapter, write("  "));
        EXPECT_CALL(adapter, write("ticksBeforeFirstBackspace"));
        EXPECT_CALL(adapter, write(" = "));
        EXPECT_CALL(adapter, writeFloat(20.0));
        EXPECT_CALL(adapter, write("\n"));

        EXPECT_CALL(adapter, write("  "));
        EXPECT_CALL(adapter, write("ticksBeforeSecondBackspace"));
        EXPECT_CALL(adapter, write(" = "));
        EXPECT_CALL(adapter, writeFloat(13.0));
        EXPECT_CALL(adapter, write("\n"));

        EXPECT_CALL(adapter, write("  "));
        EXPECT_CALL(adapter, write("ticksBeforeRepeatBackspace"));
        EXPECT_CALL(adapter, write(" = "));
        EXPECT_CALL(adapter, writeFloat(7.0));
        EXPECT_CALL(adapter, write("\n"));

        EXPECT_CALL(adapter, write("  "));
        EXPECT_CALL(adapter, write("toneEnabled"));
        EXPECT_CALL(adapter, write(" = "));
        EXPECT_CALL(adapter, writeBool(true));
        EXPECT_CALL(adapter, write("\n"));

        EXPECT_CALL(adapter, write("  "));
        EXPECT_CALL(adapter, write("toneVolumePercent"));
        EXPECT_CALL(adapter, write(" = "));
        EXPECT_CALL(adapter, writeUnsignedInt(50));
        EXPECT_CALL(adapter, write("\n"));

        EXPECT_CALL(adapter, write("  "));
        EXPECT_CALL(adapter, write("toneFrequencyHertz"));
        EXPECT_CALL(adapter, write(" = "));
        EXPECT_CALL(adapter, writeUnsignedInt(1220));
        EXPECT_CALL(adapter, write("\n"));

        EXPECT_CALL(adapter, write("  "));
        EXPECT_CALL(adapter, write("keyboardEnabled"));
        EXPECT_CALL(adapter, write(" = "));
        EXPECT_CALL(adapter, writeBool(true));
        EXPECT_CALL(adapter, write("\n"));

        EXPECT_CALL(adapter, write("  "));
        EXPECT_CALL(adapter, write("keyboardLayout"));
        EXPECT_CALL(adapter, write(" = "));
        EXPECT_CALL(adapter, writeUnsignedInt(0));
        EXPECT_CALL(adapter, write("\n"));
    }
    underTest.tick();
}

TEST(UpdateSetting, UnknownSettingKey) {
    MockSerialAdapter adapter;
    MockSettingsStorage storage;
    initToKnownValues(storage.get());
    SettingsCli underTest(&adapter, &storage);
    const char* command = "set";
    const char* settingKey = "unknown_key";

    {
        InSequence seq;
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(true));
        EXPECT_CALL(adapter, readWord()).WillOnce(Return(command)).WillOnce(Return(settingKey));
        EXPECT_CALL(adapter, skipRestOfLine()).WillOnce(Return(true));
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(false));
    }
    {
        InSequence seq;
        EXPECT_CALL(adapter, write("ERR: Unknown setting key '"));
        EXPECT_CALL(adapter, write("unknown_key"));
        EXPECT_CALL(adapter, write("'\n"));
    }

    underTest.tick();
}

TEST(UpdateSetting, InvalidBoolValue) {
    MockSerialAdapter adapter;
    MockSettingsStorage storage;
    initToKnownValues(storage.get());
    SettingsCli underTest(&adapter, &storage);
    const char* command = "set";
    const char* settingKey = "loggingEnabled";
    bool newValue = false;
    bool originalValue = storage.get()->loggingEnabled;

    {
        InSequence seq;
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(true));
        EXPECT_CALL(adapter, readWord()).WillOnce(Return(command)).WillOnce(Return(settingKey));
        EXPECT_CALL(adapter, readBool()).WillOnce(Return(nullptr));
        EXPECT_CALL(adapter, skipRestOfLine()).WillOnce(Return(true));
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(false));
    }

    EXPECT_CALL(adapter, write("ERR: Invalid value received.\n"));

    underTest.tick();

    EXPECT_EQ(storage.get()->loggingEnabled, originalValue);
}

TEST(UpdateSetting, InvalidUnsignedLongValue) {
    MockSerialAdapter adapter;
    MockSettingsStorage storage;
    initToKnownValues(storage.get());
    SettingsCli underTest(&adapter, &storage);
    const char* command = "set";
    const char* settingKey = "tickDurationMillis";
    unsigned int newValue = 42;
    unsigned int originalValue = storage.get()->tickDurationMillis;

    {
        InSequence seq;
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(true));
        EXPECT_CALL(adapter, readWord()).WillOnce(Return(command)).WillOnce(Return(settingKey));
        EXPECT_CALL(adapter, readUnsignedInt()).WillOnce(Return(nullptr));
        EXPECT_CALL(adapter, skipRestOfLine()).WillOnce(Return(true));
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(false));
    }

    EXPECT_CALL(adapter, write("ERR: Invalid value received.\n"));

    underTest.tick();

    EXPECT_EQ(storage.get()->tickDurationMillis, originalValue);
}

TEST(UpdateSetting, InvalidFloatValue) {
    MockSerialAdapter adapter;
    MockSettingsStorage storage;
    initToKnownValues(storage.get());
    SettingsCli underTest(&adapter, &storage);
    const char* command = "set";
    const char* settingKey = "ticksBeforeNewLetter";
    float newValue = 99.99;
    float originalValue = storage.get()->ticksBeforeNewLetter;

    {
        InSequence seq;
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(true));
        EXPECT_CALL(adapter, readWord()).WillOnce(Return(command)).WillOnce(Return(settingKey));
        EXPECT_CALL(adapter, readFloat()).WillOnce(Return(nullptr));
        EXPECT_CALL(adapter, skipRestOfLine()).WillOnce(Return(true));
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(false));
    }

    EXPECT_CALL(adapter, write("ERR: Invalid value received.\n"));

    underTest.tick();

    EXPECT_EQ(storage.get()->ticksBeforeNewLetter, originalValue);
}

void expectSettings(StoredSettings* actual, StoredSettings* expected) {
    EXPECT_EQ(actual->loggingEnabled, expected->loggingEnabled);
    EXPECT_EQ(actual->tickDurationMillis, expected->tickDurationMillis);
    EXPECT_EQ(actual->debounceMillis, expected->debounceMillis);
    EXPECT_EQ(actual->ticksBeforeNewLetter, expected->ticksBeforeNewLetter);
    EXPECT_EQ(actual->ticksBeforeNewWord, expected->ticksBeforeNewWord);
    EXPECT_EQ(actual->ticksBeforeDash, expected->ticksBeforeDash);
    EXPECT_EQ(actual->ticksBeforeFirstBackspace, expected->ticksBeforeFirstBackspace);
    EXPECT_EQ(actual->ticksBeforeSecondBackspace, expected->ticksBeforeSecondBackspace);
    EXPECT_EQ(actual->ticksBeforeRepeatBackspace, expected->ticksBeforeRepeatBackspace);
    EXPECT_EQ(actual->toneEnabled, expected->toneEnabled);
    EXPECT_EQ(actual->toneVolumePercent, expected->toneVolumePercent);
    EXPECT_EQ(actual->toneFrequencyHertz, expected->toneFrequencyHertz);
    EXPECT_EQ(actual->keyboardEnabled, expected->keyboardEnabled);
    EXPECT_EQ(actual->keyboardLayout, expected->keyboardLayout);
}

void setBoolTest(const char* settingKey, bool newValue, StoredSettings* expectedSettings) {
    MockSerialAdapter adapter;
    MockSettingsStorage storage;
    initToKnownValues(storage.get());
    SettingsCli underTest(&adapter, &storage);
    const char* command = "set";

    {
        InSequence seq;
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(true));
        EXPECT_CALL(adapter, readWord()).WillOnce(Return(command)).WillOnce(Return(settingKey));
        EXPECT_CALL(adapter, readBool()).WillOnce(Return(&newValue));
        EXPECT_CALL(adapter, skipRestOfLine()).WillOnce(Return(true));
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(false));
    }

    EXPECT_CALL(storage, save());
    EXPECT_CALL(adapter, write("OK\n"));

    underTest.tick();

    expectSettings(storage.get(), expectedSettings);
}

void setUnsignedIntTest(const char* settingKey, unsigned int newValue, StoredSettings* expectedSettings) {
    MockSerialAdapter adapter;
    MockSettingsStorage storage;
    initToKnownValues(storage.get());
    SettingsCli underTest(&adapter, &storage);
    const char* command = "set";

    {
        InSequence seq;
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(true));
        EXPECT_CALL(adapter, readWord()).WillOnce(Return(command)).WillOnce(Return(settingKey));
        EXPECT_CALL(adapter, readUnsignedInt()).WillOnce(Return(&newValue));
        EXPECT_CALL(adapter, skipRestOfLine()).WillOnce(Return(true));
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(false));
    }

    EXPECT_CALL(storage, save());
    EXPECT_CALL(adapter, write("OK\n"));

    underTest.tick();

    expectSettings(storage.get(), expectedSettings);
}

void setFloatTest(const char* settingKey, float newValue, StoredSettings* expectedSettings) {
    MockSerialAdapter adapter;
    MockSettingsStorage storage;
    initToKnownValues(storage.get());
    SettingsCli underTest(&adapter, &storage);
    const char* command = "set";

    {
        InSequence seq;
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(true));
        EXPECT_CALL(adapter, readWord()).WillOnce(Return(command)).WillOnce(Return(settingKey));
        EXPECT_CALL(adapter, readFloat()).WillOnce(Return(&newValue));
        EXPECT_CALL(adapter, skipRestOfLine()).WillOnce(Return(true));
        EXPECT_CALL(adapter, isLineReady()).WillOnce(Return(false));
    }

    EXPECT_CALL(storage, save());
    EXPECT_CALL(adapter, write("OK\n"));

    underTest.tick();

    expectSettings(storage.get(), expectedSettings);
}

TEST(UpdateSetting, LoggingEnabled) {
    StoredSettings expectedSettings;
    initToKnownValues(&expectedSettings);
    expectedSettings.loggingEnabled = false;
    setBoolTest("loggingEnabled", false, &expectedSettings);
}

TEST(UpdateSetting, TickDurationMillis) {
    StoredSettings expectedSettings;
    initToKnownValues(&expectedSettings);
    expectedSettings.tickDurationMillis = 42;
    setUnsignedIntTest("tickDurationMillis", 42, &expectedSettings);
}

TEST(UpdateSetting, DebounceMillis) {
    StoredSettings expectedSettings;
    initToKnownValues(&expectedSettings);
    expectedSettings.debounceMillis = 43;
    setUnsignedIntTest("debounceMillis", 43, &expectedSettings);
}

TEST(UpdateSetting, TicksBeforeNewLetter) {
    StoredSettings expectedSettings;
    initToKnownValues(&expectedSettings);
    expectedSettings.ticksBeforeNewLetter = 99.99;
    setFloatTest("ticksBeforeNewLetter", 99.99, &expectedSettings);
}

TEST(UpdateSetting, TicksBeforeNewWord) {
    StoredSettings expectedSettings;
    initToKnownValues(&expectedSettings);
    expectedSettings.ticksBeforeNewWord = 99.99;
    setFloatTest("ticksBeforeNewWord", 99.99, &expectedSettings);
}

TEST(UpdateSetting, TicksBeforeDash) {
    StoredSettings expectedSettings;
    initToKnownValues(&expectedSettings);
    expectedSettings.ticksBeforeDash = 99.99;
    setFloatTest("ticksBeforeDash", 99.99, &expectedSettings);
}

TEST(UpdateSetting, TicksBeforeFirstBackspace) {
    StoredSettings expectedSettings;
    initToKnownValues(&expectedSettings);
    expectedSettings.ticksBeforeFirstBackspace = 99.99;
    setFloatTest("ticksBeforeFirstBackspace", 99.99, &expectedSettings);
}

TEST(UpdateSetting, TicksBeforeSecondBackspace) {
    StoredSettings expectedSettings;
    initToKnownValues(&expectedSettings);
    expectedSettings.ticksBeforeSecondBackspace = 99.99;
    setFloatTest("ticksBeforeSecondBackspace", 99.99, &expectedSettings);
}

TEST(UpdateSetting, TicksBeforeRepeatBackspace) {
    StoredSettings expectedSettings;
    initToKnownValues(&expectedSettings);
    expectedSettings.ticksBeforeRepeatBackspace = 99.99;
    setFloatTest("ticksBeforeRepeatBackspace", 99.99, &expectedSettings);
}

TEST(UpdateSetting, ToneEnabled) {
    StoredSettings expectedSettings;
    initToKnownValues(&expectedSettings);
    expectedSettings.toneEnabled = false;
    setBoolTest("toneEnabled", false, &expectedSettings);
}

TEST(UpdateSetting, ToneVolumePercent) {
    StoredSettings expectedSettings;
    initToKnownValues(&expectedSettings);
    expectedSettings.toneVolumePercent = 100;
    setUnsignedIntTest("toneVolumePercent", 100, &expectedSettings);
}

TEST(UpdateSetting, ToneFrequencyHertz) {
    StoredSettings expectedSettings;
    initToKnownValues(&expectedSettings);
    expectedSettings.toneFrequencyHertz = 1000;
    setUnsignedIntTest("toneFrequencyHertz", 1000, &expectedSettings);
}

TEST(UpdateSetting, KeyboardEnabled) {
    StoredSettings expectedSettings;
    initToKnownValues(&expectedSettings);
    expectedSettings.keyboardEnabled = false;
    setBoolTest("keyboardEnabled", false, &expectedSettings);
}

TEST(UpdateSetting, KeyboardLayout) {
    StoredSettings expectedSettings;
    initToKnownValues(&expectedSettings);
    expectedSettings.keyboardLayout = 1;
    setUnsignedIntTest("keyboardLayout", 1, &expectedSettings);
}
