#include "serial-adapter.h"

#if defined ARDUINO

bool SerialAdapter::isLineReady() { return this->delegate->available() > 0 && this->delegate->peek() != '\n'; }

void SerialAdapter::write(const char* str) { this->delegate->print(str); }

void SerialAdapter::writeBool(bool value) { this->delegate->print(value ? "true" : "false"); }

void SerialAdapter::writeFloat(float value) { this->delegate->print(value); }

void SerialAdapter::writeUnsignedLong(unsigned long value) { this->delegate->print(value); }

const char* SerialAdapter::readWord() {
    String s = this->delegate->readStringUntil(' ');
    if (s == NULL) {
        return nullptr;
    } else {
        strncpy(this->readBuffer, s.c_str(), MAX_READ_BUFFER_SIZE);
        return readBuffer;
    }
}

#else
bool SerialAdapter::isLineReady() { return false; }
const char* SerialAdapter::readWord() { return NULL; }
const bool* SerialAdapter::readBool() { return NULL; }
const float* SerialAdapter::readFloat() { return NULL; }
const unsigned long* SerialAdapter::readUnsignedLong() { return NULL; }
bool SerialAdapter::skipRestOfLine() { return false; }
void SerialAdapter::write(const char* str) {}
void SerialAdapter::writeBool(bool value) {}
void SerialAdapter::writeFloat(float value) {}
void SerialAdapter::writeUnsignedLong(unsigned long value) {}
#endif
