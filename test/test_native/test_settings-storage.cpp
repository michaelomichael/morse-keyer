#include <gmock/gmock.h>

#include "settings-storage.h"

using ::testing::InSequence;
using ::testing::Return;

class MockHardwareAdapter : public HardwareAdapter {
   public:
    MockHardwareAdapter() : HardwareAdapter() {}

    MOCK_METHOD(uint8_t, readEeprom, (int address), (override));
    MOCK_METHOD(void, writeEeprom, (int address, uint8_t value), (override));
};

void setStandardValues(SettingsStorage* storage) {
    storage->get()->loggingEnabled = true;
    storage->get()->toneEnabled = true;
    storage->get()->keyboardEnabled = true;
    storage->get()->tickDurationMillis = 70;
    storage->get()->debounceMillis = 50;
    storage->get()->ticksBeforeNewLetter = 7;
    storage->get()->ticksBeforeNewWord = 8;
    storage->get()->ticksBeforeDash = 2.5;
    storage->get()->ticksBeforeFirstBackspace = 30;
    storage->get()->ticksBeforeSecondBackspace = 13;
    storage->get()->ticksBeforeRepeatBackspace = 3;
    storage->get()->toneVolumePercent = 50;
    storage->get()->toneFrequencyHertz = 1220;
    storage->get()->keyboardLayout = 0;
}

void setNonstandardValues(SettingsStorage* storage) {
    storage->get()->loggingEnabled = false;
    storage->get()->toneEnabled = false;
    storage->get()->keyboardEnabled = false;
    storage->get()->tickDurationMillis = 100;
    storage->get()->debounceMillis = 101;
    storage->get()->ticksBeforeNewLetter = 102;
    storage->get()->ticksBeforeNewWord = 103;
    storage->get()->ticksBeforeDash = 104;
    storage->get()->ticksBeforeFirstBackspace = 105;
    storage->get()->ticksBeforeSecondBackspace = 106;
    storage->get()->ticksBeforeRepeatBackspace = 107;
    storage->get()->toneVolumePercent = 108;
    storage->get()->toneFrequencyHertz = 109;
    storage->get()->keyboardLayout = 110;
}

TEST(Init, ShouldInitialiseWithKnownValues) {
    MockHardwareAdapter adapter;
    SettingsStorage underTest(&adapter);

    StoredSettings* result = underTest.get();

    EXPECT_NE(nullptr, result);
    EXPECT_EQ((bool)true, result->loggingEnabled);
    EXPECT_EQ((unsigned int)70, result->tickDurationMillis);
    EXPECT_EQ((unsigned int)50, result->debounceMillis);
    EXPECT_EQ((float)7, result->ticksBeforeNewLetter);
    EXPECT_EQ((float)8, result->ticksBeforeNewWord);
    EXPECT_EQ((float)2.5, result->ticksBeforeDash);
    EXPECT_EQ((float)30, result->ticksBeforeFirstBackspace);
    EXPECT_EQ((float)13, result->ticksBeforeSecondBackspace);
    EXPECT_EQ((float)3, result->ticksBeforeRepeatBackspace);
    EXPECT_EQ((bool)true, result->toneEnabled);
    EXPECT_EQ((unsigned int)50, result->toneVolumePercent);
    EXPECT_EQ((unsigned int)1220, result->toneFrequencyHertz);
    EXPECT_EQ((bool)true, result->keyboardEnabled);
    EXPECT_EQ((unsigned int)0, result->keyboardLayout);
}

TEST(Save, ShouldSaveValues) {
    MockHardwareAdapter adapter;
    SettingsStorage underTest(&adapter);

    setStandardValues(&underTest);

    {
        InSequence seq;
        int i = 0;
        // uint8_t Magic number
        EXPECT_CALL(adapter, writeEeprom(i++, 42));

        // uint8_t Version
        EXPECT_CALL(adapter, writeEeprom(i++, 1));

        // bool loggingEnabled = true;
        EXPECT_CALL(adapter, writeEeprom(i++, 1));

        // bool toneEnabled = true;
        EXPECT_CALL(adapter, writeEeprom(i++, 1));

        // bool keyboardEnabled = true;
        EXPECT_CALL(adapter, writeEeprom(i++, 1));

        // bool _dummy1 = false;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));

        // unsigned int tickDurationMillis = 70;
        EXPECT_CALL(adapter, writeEeprom(i++, 70));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));

        // unsigned int debounceMillis = 50;
        EXPECT_CALL(adapter, writeEeprom(i++, 50));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));

        // float ticksBeforeNewLetter = 7;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0xE0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x40));

        // float ticksBeforeNewWord = 8;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x41));

        // float ticksBeforeDash = 2.5;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x20));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x40));

        // float ticksBeforeFirstBackspace = 30;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0xF0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x41));

        // float ticksBeforeSecondBackspace = 13;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x50));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x41));

        // float ticksBeforeRepeatBackspace = 3;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x40));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x40));

        // unsigned int toneVolumePercent = 50;
        EXPECT_CALL(adapter, writeEeprom(i++, 50));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));

        // unsigned int toneFrequencyHertz = 1220;
        EXPECT_CALL(adapter, writeEeprom(i++, 0xc4));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x04));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));

        // unsigned int keyboardLayout = 0;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
    }

    underTest.save();
}

TEST(Load, ShouldLoadValuesFromEepromWhenValid) {
    MockHardwareAdapter adapter;
    SettingsStorage underTest(&adapter);

    // Set the values to non-default
    setNonstandardValues(&underTest);

    int i = 0;
    // uint8_t Magic number
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(42));

    // uint8_t Version
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(1));

    // bool loggingEnabled = true;
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(1));

    // bool toneEnabled = true;
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(1));

    // bool keyboardEnabled = true;
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(1));

    // bool _dummy1 = false;
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));

    // unsigned int tickDurationMillis = 70;
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(70));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));

    // unsigned int debounceMillis = 50;
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(50));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));

    // float ticksBeforeNewLetter = 7;
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0xE0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0x40));

    // float ticksBeforeNewWord = 8;
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0x41));

    // float ticksBeforeDash = 2.5;
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0x20));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0x40));

    // float ticksBeforeFirstBackspace = 30;
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0xF0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0x41));

    // float ticksBeforeSecondBackspace = 13;
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0x50));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0x41));

    // float ticksBeforeRepeatBackspace = 3;
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0x40));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0x40));

    // unsigned int toneVolumePercent = 50;
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(50));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));

    // unsigned int toneFrequencyHertz = 1220;
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0xc4));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0x04));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));

    // unsigned int keyboardLayout = 0;
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));
    EXPECT_CALL(adapter, readEeprom(i++)).WillOnce(Return(0));

    bool result = underTest.load();

    EXPECT_EQ(true, result);

    EXPECT_EQ(true, underTest.get()->loggingEnabled);
    EXPECT_EQ(true, underTest.get()->toneEnabled);
    EXPECT_EQ(true, underTest.get()->keyboardEnabled);
    EXPECT_EQ(70, underTest.get()->tickDurationMillis);
    EXPECT_EQ(50, underTest.get()->debounceMillis);
    EXPECT_EQ(7, underTest.get()->ticksBeforeNewLetter);
    EXPECT_EQ(8, underTest.get()->ticksBeforeNewWord);
    EXPECT_EQ(2.5, underTest.get()->ticksBeforeDash);
    EXPECT_EQ(30, underTest.get()->ticksBeforeFirstBackspace);
    EXPECT_EQ(13, underTest.get()->ticksBeforeSecondBackspace);
    EXPECT_EQ(3, underTest.get()->ticksBeforeRepeatBackspace);
    EXPECT_EQ(50, underTest.get()->toneVolumePercent);
    EXPECT_EQ(1220, underTest.get()->toneFrequencyHertz);
    EXPECT_EQ(0, underTest.get()->keyboardLayout);
}

TEST(Load, ShouldSaveCurrentValuesToEepromWhenMagicNumberIsWrong) {
    MockHardwareAdapter adapter;
    SettingsStorage underTest(&adapter);

    // Set the values to non-default
    setStandardValues(&underTest);

    EXPECT_CALL(adapter, readEeprom(0)).WillOnce(Return(99));

    setStandardValues(&underTest);

    {
        InSequence seq;
        int i = 0;
        // Magic number
        EXPECT_CALL(adapter, writeEeprom(i++, 42));

        // Version
        EXPECT_CALL(adapter, writeEeprom(i++, 1));

        // bool loggingEnabled = true;
        EXPECT_CALL(adapter, writeEeprom(i++, 1));

        // bool toneEnabled = true;
        EXPECT_CALL(adapter, writeEeprom(i++, 1));

        // bool keyboardEnabled = true;
        EXPECT_CALL(adapter, writeEeprom(i++, 1));

        // bool _dummy1 = false;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));

        // unsigned int tickDurationMillis = 70;
        EXPECT_CALL(adapter, writeEeprom(i++, 70));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));

        // unsigned int debounceMillis = 50;
        EXPECT_CALL(adapter, writeEeprom(i++, 50));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));

        // float ticksBeforeNewLetter = 7;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0xE0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x40));

        // float ticksBeforeNewWord = 8;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x41));

        // float ticksBeforeDash = 2.5;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x20));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x40));

        // float ticksBeforeFirstBackspace = 30;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0xF0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x41));

        // float ticksBeforeSecondBackspace = 13;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x50));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x41));

        // float ticksBeforeRepeatBackspace = 3;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x40));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x40));

        // unsigned int toneVolumePercent = 50;
        EXPECT_CALL(adapter, writeEeprom(i++, 50));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));

        // unsigned int toneFrequencyHertz = 1220;
        EXPECT_CALL(adapter, writeEeprom(i++, 0xc4));
        EXPECT_CALL(adapter, writeEeprom(i++, 0x04));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));

        // unsigned int keyboardLayout = 0;
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
        EXPECT_CALL(adapter, writeEeprom(i++, 0));
    }

    bool result = underTest.load();

    EXPECT_EQ(false, result);

    EXPECT_EQ(true, underTest.get()->loggingEnabled);
    EXPECT_EQ(true, underTest.get()->toneEnabled);
    EXPECT_EQ(true, underTest.get()->keyboardEnabled);
    EXPECT_EQ(70, underTest.get()->tickDurationMillis);
    EXPECT_EQ(50, underTest.get()->debounceMillis);
    EXPECT_EQ(7, underTest.get()->ticksBeforeNewLetter);
    EXPECT_EQ(8, underTest.get()->ticksBeforeNewWord);
    EXPECT_EQ(2.5, underTest.get()->ticksBeforeDash);
    EXPECT_EQ(30, underTest.get()->ticksBeforeFirstBackspace);
    EXPECT_EQ(13, underTest.get()->ticksBeforeSecondBackspace);
    EXPECT_EQ(3, underTest.get()->ticksBeforeRepeatBackspace);
    EXPECT_EQ(50, underTest.get()->toneVolumePercent);
    EXPECT_EQ(1220, underTest.get()->toneFrequencyHertz);
    EXPECT_EQ(0, underTest.get()->keyboardLayout);
}

TEST(Load, ShouldReturnFalseWhenVersionIsUnknown) {
    MockHardwareAdapter adapter;
    SettingsStorage underTest(&adapter);

    // Set the values to non-default
    setNonstandardValues(&underTest);

    // Magic number
    EXPECT_CALL(adapter, readEeprom(0)).WillOnce(Return(42));

    // Version
    EXPECT_CALL(adapter, readEeprom(1)).WillOnce(Return(0));

    bool result = underTest.load();

    EXPECT_EQ(false, result);

    EXPECT_EQ(false, underTest.get()->loggingEnabled);
    EXPECT_EQ(false, underTest.get()->toneEnabled);
    EXPECT_EQ(false, underTest.get()->keyboardEnabled);
    EXPECT_EQ(100, underTest.get()->tickDurationMillis);
    EXPECT_EQ(101, underTest.get()->debounceMillis);
    EXPECT_EQ(102, underTest.get()->ticksBeforeNewLetter);
    EXPECT_EQ(103, underTest.get()->ticksBeforeNewWord);
    EXPECT_EQ(104, underTest.get()->ticksBeforeDash);
    EXPECT_EQ(105, underTest.get()->ticksBeforeFirstBackspace);
    EXPECT_EQ(106, underTest.get()->ticksBeforeSecondBackspace);
    EXPECT_EQ(107, underTest.get()->ticksBeforeRepeatBackspace);
    EXPECT_EQ(108, underTest.get()->toneVolumePercent);
    EXPECT_EQ(109, underTest.get()->toneFrequencyHertz);
    EXPECT_EQ(110, underTest.get()->keyboardLayout);
}