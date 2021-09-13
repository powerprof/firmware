#include "MsgPack.h"

void MsgPack::sendReadings(Readings readings, Stream& dest) {
  StaticJsonDocument<JSON_BUFFER_SIZE_DEBUG> doc;
  doc[MSG_KEY_CURRENT] = readings.current;
  doc[MSG_KEY_POWER] = readings.power;
  doc[MSG_KEY_TIMESTAMP] = readings.timestamp;
  doc[MSG_KEY_VOLTAGE] = readings.voltage;
  serializeMsgPack(doc, dest);
}

void MsgPack::sendDebugMessage(const String message, Stream& dest) {
  StaticJsonDocument<JSON_BUFFER_SIZE_DEBUG> doc;
  doc[MSG_KEY_DEBUG] = MSG_VAL_DEBUG;
  doc[MSG_KEY_MESSAGE] = message;
  serializeMsgPack(doc, dest);
}

bool MsgPack::readCommandMessage(CommandMessage* message, Stream& src) {
  auto doc = StaticJsonDocument<JSON_BUFFER_SIZE_COMMAND>();
  auto result = deserializeMsgPack(doc, Serial);

  if (result != DeserializationError::Ok) {
    sendDebugMessage(DEBUG_MSG_DESERIALIZE_FAILED, src);

    return false;
  }

  const String command = String((const char*)doc[MSG_KEY_COMMAND]);

  if (command == MSG_COMMAND_START) {
    message->command = Command::Start;
  } else if (command == MSG_COMMAND_STOP) {
    message->command = Command::Stop;
  } else if (command == MSG_COMMAND_SETUP) {
    message->command = Command::Setup;
    message->sampleRate = doc[MSG_KEY_SAMPLE_RATE];
  } else {
    sendDebugMessage(DEBUG_MSG_UNKNOWN_COMMAND, src);

    return false;
  }

  return true;
}
