#include "settings-storage.h"

#ifdef ARDUINO
#include <EEPROM.h>
#endif

#define MAGIC_NUMBER 0x42

bool SettingsStorage::load() {
    uint8_t magic = 0;
    int address = 0;
    this->readEeprom(address, &magic, sizeof(magic));

    if (magic != MAGIC_NUMBER) {
        // EEPROM not initialized, write default settings
        _settings = StoredSettings();
        this->save();
        return false;
    }

    // TODO: read all but the last N bytes of eeprom, then compare the CRC of that to the value stored in the last N
    // bytes

    address += sizeof(magic);

    StoredSettings tempSettings;
    this->readEeprom(address, (uint8_t*)(&tempSettings), sizeof(tempSettings));

    return true;
}

void SettingsStorage::save() {
    uint8_t magic = MAGIC_NUMBER;
    int address = 0;

    this->writeEeprom(address, &magic, sizeof(magic));
    address += sizeof(magic);

    this->writeEeprom(address, (uint8_t*)(&(this->_settings)), sizeof(StoredSettings));
}

void SettingsStorage::readEeprom(int address, uint8_t* dest, int length) {
    for (int i = 0; i < length; ++i) {
        dest[i] = this->readEeprom(address + i);
    }
}

void SettingsStorage::writeEeprom(int address, uint8_t* src, int length) {
    for (int i = 0; i < length; ++i) {
        this->writeEeprom(address + i, src[i]);
    }
}

#ifdef ARDUINO
uint8_t SettingsStorage::readEeprom(int address) { return EEPROM.read(address); }
void SettingsStorage::writeEeprom(int address, uint8_t value) { EEPROM.write(address, value); }
#else
#define EEPROM_SIZE 128
namespace {
uint8_t fakeEeprom[EEPROM_SIZE] = {0};
}
uint8_t SettingsStorage::readEeprom(int address) { return fakeEeprom[address]; }
void SettingsStorage::writeEeprom(int address, uint8_t value) { fakeEeprom[address] = value; }
#endif

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