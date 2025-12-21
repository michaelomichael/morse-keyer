#pragma once

#include <inttypes.h>

#if defined ARDUINO
#include <USBAPI.h>
#define RealSerial Serial_
#else
#define RealSerial void
#endif

#define MAX_READ_BUFFER_SIZE 100

/// @brief A wrapper around Arduino's HardwareSerial. It's useful for mocking in tests and has more robust parsing.
class SerialAdapter {
   public:
    inline SerialAdapter(RealSerial* delegate) { this->_delegate = delegate; }

    virtual void begin();

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
    virtual const unsigned int* readUnsignedInt();

    /**
     * Returns true if there were unread characters in the line.
     * Subsequent calls to isLineReady() will return false until
     * a new line is available.
     */
    virtual bool skipRestOfLine();
    virtual void write(const char* str);
    virtual void writeBool(bool value);
    virtual void writeFloat(float value);
    virtual void writeUnsignedInt(unsigned int value);
    virtual void writeUnsignedLong(unsigned long value);

   private:
    virtual bool checkConnection();
    virtual int read();
    virtual int available();
    RealSerial* _delegate;
    bool isConnectionEstablished = false;
    char _lineBuffer[MAX_READ_BUFFER_SIZE];
    int _lineBufferWritePos = 0;
    int _lineBufferReadPos = 0;
    char _wordBuffer[MAX_READ_BUFFER_SIZE];
    bool _boolBuffer;
    float _floatBuffer;
    unsigned int _unsignedIntBuffer;
};
