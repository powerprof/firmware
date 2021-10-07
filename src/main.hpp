#pragma once

#include "Blinkenlight.h"
#include "Sensor.h"
#include "Stream.h"
#include "Wireless.h"

#define DEBUG_MSG_APP_ID F("gufu_gud v0.3")

#if defined(ARDUINO_TEENSY)
#define SERIAL_BAUD 2e6  // 2MBit/s
#else
#define SERIAL_BAUD 1e6  // 1MBit/s
#endif
