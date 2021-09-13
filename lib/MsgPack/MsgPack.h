#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

// Serial bitrate (14.4Kbps)
#define SERIAL_BAUD 115200

// JSON message constants
#define MSG_KEY_CURRENT F("i")
#define MSG_KEY_POWER F("p")
#define MSG_KEY_TIMESTAMP F("t")
#define MSG_KEY_VOLTAGE F("v")
#define MSG_KEY_MESSAGE F("message")
#define MSG_KEY_DEBUG F("debug")
#define MSG_KEY_COMMAND F("command")
#define MSG_KEY_SAMPLE_RATE F("sampleRate")
#define MSG_VAL_DEBUG true

#define MSG_COMMAND_SETUP F("setup")
#define MSG_COMMAND_START F("start")
#define MSG_COMMAND_STOP F("stop")

#define DEBUG_MSG_DESERIALIZE_FAILED F("Failed to deserialize command!")
#define DEBUG_MSG_UNKNOWN_COMMAND F("Unknown command!")

// see https://arduinojson.org/v6/assistant/ for estimates
#define JSON_BUFFER_SIZE 64
#define JSON_BUFFER_SIZE_DEBUG 96
#define JSON_BUFFER_SIZE_COMMAND 128

struct Readings {
  double timestamp = 0;
  double voltage = 0;
  double current = 0;
  double power = 0;
};

enum Command {
  None,
  Setup,
  Start,
  Stop
};

struct CommandMessage {
  Command command = Command::None;
  uint32_t sampleRate = 0;
};

struct MsgPack {
  static void sendReadings(const Readings readings, Stream& dest);
  static void sendDebugMessage(const String message, Stream& dest);
  static bool readCommandMessage(CommandMessage* message, Stream& src);
};
