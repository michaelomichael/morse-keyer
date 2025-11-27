#include "serial-adapter.h"

// #include <cstdlib.h>
#include <stdlib.h>

#if defined ARDUINO

int SerialAdapter::available() { return this->delegate->available(); }
int SerialAdapter::read() { return this->delegate->read(); }

void SerialAdapter::write(const char* str) { this->delegate->print(str); }

void SerialAdapter::writeBool(bool value) { this->delegate->print(value ? "true" : "false"); }

void SerialAdapter::writeFloat(float value) { this->delegate->print(value); }

void SerialAdapter::writeUnsignedLong(unsigned long value) { this->delegate->print(value); }

#else
#include <string.h>
int SerialAdapter::available() { return 0; }
int SerialAdapter::read() { return -1; };
void SerialAdapter::write(const char* str) {}
void SerialAdapter::writeBool(bool value) {}
void SerialAdapter::writeFloat(float value) {}
void SerialAdapter::writeUnsignedLong(unsigned long value) {}
#endif

// TODO: Unit tests
bool SerialAdapter::isLineReady() {
    while (this->available() > 0) {
        int result = this->read();
        if (result == -1) {
            return false;
        }

        if (result == '\n') {
            this->lineBuffer[lineBufferWritePos++] = '\0';
            this->lineBufferReadPos = 0;
            return true;
        }

        if (lineBufferWritePos >= MAX_READ_BUFFER_SIZE - 1) {
            // Buffer overflow; reset buffer
            lineBufferWritePos = 0;
        }

        lineBuffer[lineBufferWritePos++] = (char)result;
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
    while (lineBufferReadPos < MAX_READ_BUFFER_SIZE - 1) {
        char c = this->lineBuffer[this->lineBufferReadPos];

        if (c == '\0' || c == '\n') {
            break;
        }
        lineBufferReadPos++;

        if (c == ' ') {
            break;
        }

        this->wordBuffer[i++] = c;
    }

    if (i == 0) {
        return nullptr;
    } else {
        this->wordBuffer[i] = '\0';
        return this->wordBuffer;
    }
}

const bool* SerialAdapter::readBool() {
    const char* word = this->readWord();
    if (word == nullptr) {
        return nullptr;
    } else if (strcasecmp(word, "true") == 0) {
        this->boolBuffer = true;
    } else if (strcasecmp(word, "false") == 0) {
        this->boolBuffer = false;
    } else {
        return nullptr;
    }
    return &this->boolBuffer;
}

const float* SerialAdapter::readFloat() {
    const char* word = this->readWord();

    if (word == nullptr) {
        return nullptr;
    }

    char* pointerToFinalChar;
    this->floatBuffer = (float)strtod(word, &pointerToFinalChar);

    if (*pointerToFinalChar == '\0') {
        return &this->floatBuffer;
    } else {
        return nullptr;
    }
}

const unsigned long* SerialAdapter::readUnsignedLong() {
    const char* word = this->readWord();

    if (word == nullptr) {
        return nullptr;
    }

    char* pointerToFinalChar;
    this->unsignedLongBuffer = strtoul(word, &pointerToFinalChar, 10);

    if (*pointerToFinalChar == '\0' && this->unsignedLongBuffer >= (unsigned long)0) {
        return &this->unsignedLongBuffer;
    } else {
        return nullptr;
    }
}

bool SerialAdapter::skipRestOfLine() {
    bool hadUnreadChars = this->lineBufferReadPos < this->lineBufferWritePos;
    this->lineBufferReadPos = 0;
    this->lineBufferWritePos = 0;
    return hadUnreadChars;
}