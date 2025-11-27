#pragma once

#include <inttypes.h>

// #include <stdio.h>  // for size_t

#if defined ARDUINO
// #include <HardwareSerial.h>
#include <USBAPI.h>
#define RealSerial Serial_
#else
// #define HardwareSerial void
#define RealSerial void
#endif

#define MAX_READ_BUFFER_SIZE 100

class SerialAdapter {
   public:
    inline SerialAdapter(RealSerial* delegate) { this->delegate = delegate; }

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
    virtual int read();
    virtual int available();
    RealSerial* delegate;
    char lineBuffer[MAX_READ_BUFFER_SIZE];
    int lineBufferWritePos = 0;
    int lineBufferReadPos = 0;
    char wordBuffer[MAX_READ_BUFFER_SIZE];
    bool boolBuffer;
    float floatBuffer;
    unsigned long unsignedLongBuffer;
};
