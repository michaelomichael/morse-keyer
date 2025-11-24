#pragma once

#include <inttypes.h>

// #include <stdio.h>  // for size_t

#if defined ARDUINO
#include <HardwareSerial.h>

#else
#define HardwareSerial void
// #define size_t long unsigned int
#endif

#define MAX_READ_BUFFER_SIZE 100

class SerialAdapter {
   public:
    inline SerialAdapter(HardwareSerial* delegate) { this->delegate = delegate; }

    /**
     * Returns false until a complete line (ending with \n) is available.
     */
    virtual bool isLineReady();

    /**
     * You must use these read results immediately, or copy them,
     * before any other methods are called on this adapter.
     */
    virtual const char* readWord();
    virtual const bool* readBool();
    virtual const float* readFloat();
    virtual const unsigned long* readUnsignedLong();

    /**
     * Returns true if there were unread characters in the line.
     * Subsequent calls to isLineReady() will return false until
     * a new line is available.
     */
    virtual bool skipRestOfLine();
    virtual void write(const char* str);
    virtual void writeBool(bool value);
    virtual void writeFloat(float value);
    virtual void writeUnsignedLong(unsigned long value);

   private:
    HardwareSerial* delegate;
    char readBuffer[MAX_READ_BUFFER_SIZE];
};
