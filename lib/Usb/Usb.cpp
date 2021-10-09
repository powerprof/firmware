#include "Usb.h"

void Usb::init(bool waitForConnection) {
  Serial.begin(SERIAL_BAUD);

  while (waitForConnection && !Serial) {
    delay(100);
  }

  MsgPack::sendDebugMessage(DEBUG_MSG_APP_ID, Serial);
}

bool Usb::readCommandMessage(CommandMessage* message) {
  if (Serial.available() > 0) {
    return MsgPack::readCommandMessage(message, Serial);
  }

  return false;
}
