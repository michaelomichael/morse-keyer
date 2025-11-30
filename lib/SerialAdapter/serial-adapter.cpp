#include "serial-adapter.h"

// #include <cstdlib.h>
#include <stdlib.h>

#if defined ARDUINO

void SerialAdapter::begin() {
    Serial.begin(115200);

    while (!Serial) {
        ;  // Wait for the USB CDC serial connection to be established
    }
}

int SerialAdapter::available() { return this->_delegate->available(); }
int SerialAdapter::read() { return this->_delegate->read(); }

void SerialAdapter::write(const char* str) { this->_delegate->print(str); }

void SerialAdapter::writeBool(bool value) { this->_delegate->print(value ? "true" : "false"); }

void SerialAdapter::writeFloat(float value) { this->_delegate->print(value); }

void SerialAdapter::writeUnsignedInt(unsigned int value) { this->_delegate->print(value); }

void SerialAdapter::writeUnsignedLong(unsigned long value) { this->_delegate->print(value); }

#else
#include <string.h>
void SerialAdapter::begin() {}
int SerialAdapter::available() { return 0; }
int SerialAdapter::read() { return -1; };
void SerialAdapter::write(const char* str) {}
void SerialAdapter::writeBool(bool value) {}
void SerialAdapter::writeFloat(float value) {}
void SerialAdapter::writeUnsignedInt(unsigned int value) {}
void SerialAdapter::writeUnsignedLong(unsigned long value) {}
#endif

// TODO - this feels massively over-engineered, can we not just rely on the client sending
// only complete lines?
bool SerialAdapter::isLineReady() {
    while (this->available() > 0) {
        int result = this->read();
        if (result == -1) {
            return false;
        }

        if (result == '\n') {
            this->_lineBuffer[_lineBufferWritePos++] = '\0';
            this->_lineBufferReadPos = 0;
            return true;
        }

        if (_lineBufferWritePos >= MAX_READ_BUFFER_SIZE - 1) {
            // Buffer overflow; reset buffer
            _lineBufferWritePos = 0;
        }

        _lineBuffer[_lineBufferWritePos++] = (char)result;
    }

    return false;
}

const char* SerialAdapter::readWord() {
    /* "a b\n"
         ^
         lbrp: 2
         out:  "a"
         i:    1
         */
    int i = 0;
    while (_lineBufferReadPos < MAX_READ_BUFFER_SIZE - 1) {
        char c = this->_lineBuffer[this->_lineBufferReadPos];

        if (c == '\0' || c == '\n') {
            break;
        }
        _lineBufferReadPos++;

        if (c == ' ') {
            break;
        }

        this->_wordBuffer[i++] = c;
    }

    if (i == 0) {
        return nullptr;
    } else {
        this->_wordBuffer[i] = '\0';
        return this->_wordBuffer;
    }
}

const bool* SerialAdapter::readBool() {
    const char* word = this->readWord();
    if (word == nullptr) {
        return nullptr;
    } else if (strcasecmp(word, "true") == 0) {
        this->_boolBuffer = true;
    } else if (strcasecmp(word, "false") == 0) {
        this->_boolBuffer = false;
    } else {
        return nullptr;
    }
    return &this->_boolBuffer;
}

const float* SerialAdapter::readFloat() {
    const char* word = this->readWord();

    if (word == nullptr) {
        return nullptr;
    }

    char* pointerToFinalChar;
    this->_floatBuffer = (float)strtod(word, &pointerToFinalChar);

    if (*pointerToFinalChar == '\0') {
        return &this->_floatBuffer;
    } else {
        return nullptr;
    }
}

const unsigned int* SerialAdapter::readUnsignedInt() {
    const char* word = this->readWord();

    if (word == nullptr) {
        return nullptr;
    }

    char* pointerToFinalChar;
    this->_unsignedIntBuffer = strtoul(word, &pointerToFinalChar, 10);

    if (*pointerToFinalChar == '\0' && this->_unsignedIntBuffer >= (unsigned int)0) {
        return &this->_unsignedIntBuffer;
    } else {
        return nullptr;
    }
}

bool SerialAdapter::skipRestOfLine() {
    bool hadUnreadChars = this->_lineBufferReadPos < this->_lineBufferWritePos - 1;
    this->_lineBufferReadPos = 0;
    this->_lineBufferWritePos = 0;
    return hadUnreadChars;
}