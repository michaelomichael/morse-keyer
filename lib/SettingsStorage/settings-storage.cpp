#include "settings-storage.h"

#define MAGIC_NUMBER 42

bool SettingsStorage::load() {
    uint8_t magic;
    int address = 0;
    this->readEeprom(address, &magic, sizeof(magic));
    address += sizeof(magic);

    if (magic != MAGIC_NUMBER) {
        // EEPROM not initialized, write default settings
        _settings = StoredSettings();
        save();
        return false;
    }

    uint8_t version;
    this->readEeprom(address, &version, sizeof(version));
    address += sizeof(magic);

    if (version != 1) {
        // Invalid version, just leave what we have already
        return false;
    }

    // TODO: read all but the last N bytes of eeprom, then compare the CRC of that to the value stored in the last N
    // bytes

    this->readEeprom(address, (uint8_t*)(&_settings), sizeof(_settings));
    return true;
}

void SettingsStorage::save() {
    uint8_t magic = MAGIC_NUMBER;
    int address = 0;

    this->writeEeprom(address, &magic, sizeof(magic));
    address += sizeof(magic);

    uint8_t version = 1;
    this->writeEeprom(address, &version, sizeof(version));
    address += sizeof(version);

    // TODO: Move the bools together into groups of 4 to get 4-byte alignment
    struct x {
        bool someValue = true;
        bool someValue2 = true;
        bool someValue3 = true;
        bool someValue4 = false;
        unsigned int someValue5 = 70;
        float someValue6 = 12.45;
    };
    x y;
    // this->writeEeprom(address, (uint8_t*)(&y), sizeof(x));
    this->writeEeprom(address, (uint8_t*)(&(this->_settings)), sizeof(StoredSettings));
}

void SettingsStorage::print(SerialAdapter* serial) {
    serial->write("Stored Settings:");
    serial->write("\n  loggingEnabled: ");
    serial->writeBool(this->_settings.loggingEnabled);
    serial->write("\n  tickDurationMillis: ");
    serial->writeUnsignedInt(this->_settings.tickDurationMillis);
    serial->write("\n  debounceMillis: ");
    serial->writeUnsignedInt(this->_settings.debounceMillis);
    serial->write("\n  ticksBeforeNewLetter: ");
    serial->writeFloat(this->_settings.ticksBeforeNewLetter);
    serial->write("\n  ticksBeforeNewWord: ");
    serial->writeFloat(this->_settings.ticksBeforeNewWord);
    serial->write("\n  ticksBeforeDash: ");
    serial->writeFloat(this->_settings.ticksBeforeDash);
    serial->write("\n  ticksBeforeFirstBackspace: ");
    serial->writeFloat(this->_settings.ticksBeforeFirstBackspace);
    serial->write("\n  ticksBeforeSecondBackspace: ");
    serial->writeFloat(this->_settings.ticksBeforeSecondBackspace);
    serial->write("\n  ticksBeforeRepeatBackspace: ");
    serial->writeFloat(this->_settings.ticksBeforeRepeatBackspace);
    serial->write("\n  toneVolumePercent: ");
    serial->writeUnsignedInt(this->_settings.toneVolumePercent);
    serial->write("\n  toneFrequencyHertz: ");
    serial->writeUnsignedInt(this->_settings.toneFrequencyHertz);
    serial->write("\n");
}

void SettingsStorage::readEeprom(int address, uint8_t* dest, int length) {
    for (int i = 0; i < length; ++i) {
        dest[i] = _hardwareAdapter->readEeprom(address + i);
    }
}

void SettingsStorage::writeEeprom(int address, uint8_t* src, int length) {
    for (int i = 0; i < length; ++i) {
        _hardwareAdapter->writeEeprom(address + i, src[i]);
    }
}

/* TODO
unsigned long eeprom_crc(int length) {
    const unsigned long crc_table[16] = {0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190, 0x6b6b51f4,
                                         0x4db26158, 0x5005713c, 0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
                                         0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c};

    unsigned long crc = ~0L;

    for (int index = 0; index < length; ++index) {
        crc = crc_table[(crc ^ EEPROM[index]) & 0x0f] ^ (crc >> 4);

        crc = crc_table[(crc ^ (EEPROM[index] >> 4)) & 0x0f] ^ (crc >> 4);

        crc = ~crc;
    }

    return crc;
}
*/