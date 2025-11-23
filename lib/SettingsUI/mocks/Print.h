#pragma once

#include <inttypes.h>
#include <stdio.h>  // for size_t

// #include "WString.h"

#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN  // Prevent warnings if BIN is previously defined in "iotnx4.h" or similar
#undef BIN
#endif
#define BIN 2

class Print {
   public:
    Print() {};

    virtual size_t write(uint8_t) = 0;
    size_t write(const char* str);
    virtual size_t write(const uint8_t* buffer, size_t size);
    size_t write(const char* buffer, size_t size);

    virtual int availableForWrite() { return 0; }

    // size_t print(const String&);
    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);
    // size_t print(const Printable&);

    // size_t println(const String& s);
    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);
    // size_t println(const Printable&);
    size_t println(void);

    virtual void flush() { /* Empty implementation for backward compatibility */ }
};
