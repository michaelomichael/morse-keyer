#pragma once

#ifdef ARDUINO
#include <HardwareSerial.h>
#else
#include "mocks/HardwareSerial.h"
#endif

void init(HardwareSerial* serial);

void tick();