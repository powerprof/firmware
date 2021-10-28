#include "MsgPack.h"

void MsgPack::sendReadings(Stream& dest, Readings readings) {
  auto doc = ReadingsDocument();

  doc[MSG_KEY_CURRENT] = readings.current;
  doc[MSG_KEY_POWER] = readings.power;
  doc[MSG_KEY_TIMESTAMP] = readings.timestamp;
  doc[MSG_KEY_VOLTAGE] = readings.voltage;

  serializeMsgPack(doc, dest);
}

void MsgPack::sendDebugMessage(Stream& dest, const String message) {
  auto doc = MessageDocument();

  doc[MSG_KEY_DEBUG] = MSG_VAL_DEBUG;
  doc[MSG_KEY_MESSAGE] = message;

  serializeMsgPack(doc, dest);
}

void MsgPack::sendDebugMessage(Stream& dest, const char* format...) {
  char messageBuffer[JSON_BUFFER_SIZE_MESSAGE];

  sprintf(messageBuffer, format);

  sendDebugMessage(dest, messageBuffer);
}

bool MsgPack::readCommandMessage(CommandMessage* message, CommandDocument doc) {
  const String command = String((const char*)doc[MSG_KEY_COMMAND]);

  if (command == MSG_COMMAND_START) {
    message->command = Command::Start;
  } else if (command == MSG_COMMAND_STOP) {
    message->command = Command::Stop;
  } else if (command == MSG_COMMAND_SETUP) {
    message->command = Command::Setup;
    message->sampleRate = doc[MSG_KEY_SAMPLE_RATE];
  } else {
    return false;
  }

  return true;
}

bool MsgPack::readCommandMessage(Stream& src, CommandMessage* message) {
  auto doc = CommandDocument();
  auto result = deserializeMsgPack(doc, Serial);

  if (result != DeserializationError::Ok) {
    sendDebugMessage(src, DEBUG_MSG_DESERIALIZE_FAILED);
    return false;
  }

  return readCommandMessage(message, doc);
}

bool MsgPack::readCommandMessage(CommandMessage* message,
                                 const uint8_t* payload) {
  auto doc = CommandDocument();
  auto result = deserializeMsgPack(doc, payload);

  if (result != DeserializationError::Ok) {
    return false;
  }

  return readCommandMessage(message, doc);
}
