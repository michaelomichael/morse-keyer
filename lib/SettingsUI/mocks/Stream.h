#pragma once

#include <inttypes.h>

#include "Print.h"
#include "WString.h"

enum LookaheadMode {
    SKIP_ALL,        // All invalid characters are ignored.
    SKIP_NONE,       // Nothing is skipped, and the stream is not touched unless the first waiting character is valid.
    SKIP_WHITESPACE  // Only tabs, spaces, line feeds & carriage returns are skipped.
};

#define NO_IGNORE_CHAR '\x01'  // a char not found in a valid ASCII numeric field

class Stream : public Print {
   public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;

    Stream() {}

    bool find(char* target);  // reads data from the stream until the target string is found
    bool find(uint8_t* target) { return find((char*)target); }
    // returns true if target string is found, false if timed out (see setTimeout)

    bool find(char* target,
              size_t length);  // reads data from the stream until the target string of given length is found
    bool find(uint8_t* target, size_t length) { return find((char*)target, length); }
    // returns true if target string is found, false if timed out

    bool find(char target) { return find(&target, 1); }

    bool findUntil(char* target, char* terminator);  // as find but search ends if the terminator string is found
    bool findUntil(uint8_t* target, char* terminator) { return findUntil((char*)target, terminator); }

    bool findUntil(char* target, size_t targetLen, char* terminate,
                   size_t termLen);  // as above but search ends if the terminate string is found
    bool findUntil(uint8_t* target, size_t targetLen, char* terminate, size_t termLen) {
        return findUntil((char*)target, targetLen, terminate, termLen);
    }

    long parseInt(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR);

    float parseFloat(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR);

    size_t readBytes(char* buffer, size_t length);  // read chars from stream into buffer
    size_t readBytes(uint8_t* buffer, size_t length) { return readBytes((char*)buffer, length); }

    size_t readBytesUntil(char terminator, char* buffer, size_t length);  // as readBytes with terminator character
    size_t readBytesUntil(char terminator, uint8_t* buffer, size_t length) {
        return readBytesUntil(terminator, (char*)buffer, length);
    }
    String readString();
    String readStringUntil(char terminator);
};

#undef NO_IGNORE_CHAR
