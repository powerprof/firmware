#include "Wireless.h"

#if defined(ESP8266) || defined(ESP32)

#if defined(ESP8266)
ESP8266WiFiMulti Wireless::multi = ESP8266WiFiMulti();
#elif defined(ESP32)
WiFiMulti Wireless::multi = WiFiMulti();
#endif

WebSocketsServer Wireless::server = WebSocketsServer(WEBSOCKET_PORT);
bool Wireless::hasMessage = false;
CommandMessage Wireless::commandMessage = CommandMessage();

void Wireless::init() {
  MsgPack::sendDebugMessage(Serial, "Connecting to %s", WIRELESS_SSID);
  multi.addAP(WIRELESS_SSID, WIRELESS_PSK);

  MsgPack::sendDebugMessage(Serial, F("Connecting to WiFi..."));
  while (multi.run() != WL_CONNECTED) {
    delay(100);
  }
  MsgPack::sendDebugMessage(Serial, F("Connected to WiFi!"));

  server.begin();
  server.onEvent(Wireless::handler);
}

void Wireless::handler(uint8_t num, WStype_t type, const uint8_t* payload,
                       size_t length) {
  if (hasMessage) {
    MsgPack::sendDebugMessage(Serial, F("Command message overflow!"));
    return;
  }

  MsgPack::sendDebugMessage(Serial, F("Parsing message from socket..."));
  commandMessage = CommandMessage();
  hasMessage = MsgPack::readCommandMessage(&commandMessage, payload);
}

bool Wireless::readCommandMessage(CommandMessage* message) {
  if (hasMessage) {
    hasMessage = false;
    message->command = commandMessage.command;
    message->sampleRate = commandMessage.sampleRate;
    return true;
  }

  return false;
}

#endif
