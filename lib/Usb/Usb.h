#pragma once

#include <Arduino.h>
#include <MsgPack.h>

#if defined(ARDUINO_TEENSY)
#define SERIAL_BAUD 2e6  // 2MBit/s
#else
#define SERIAL_BAUD 1e6  // 1MBit/s
#endif

#define DEBUG_MSG_APP_ID F("gufu_gud v0.3")

struct Usb {
  static void init(bool waitForConnection);
  static bool readCommandMessage(CommandMessage* message);
};
