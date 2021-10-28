#pragma once

#if defined(ESP8266) || defined(ESP32)

#define WIRELESS_SSID "bar"
#define WIRELESS_PSK "wi9NNYara"

#define WEBSOCKET_PORT 8081  // WS port

#if defined(ESP8266)
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#define WEBSOCKETS_NETWORK_TYPE 0  // async
#elif defined(ESP32)
#include <WiFiMulti.h>
#include <mdns.h>
#define WEBSOCKETS_NETWORK_TYPE 4  // esp32
#endif
#include <WebSocketsServer.h>

#include "MsgPack.h"

class Wireless {
#if defined(ESP8266)
  static ESP8266WiFiMulti multi;
#elif defined(ESP32)
  static WiFiMulti multi;
#endif
  static WebSocketsServer server;
  static bool hasMessage;
  static CommandMessage commandMessage;
  static void reconnect();

 public:
  static void handler(uint8_t num, WStype_t type, const uint8_t* payload,
                      size_t length);
  static void init();
  static bool readCommandMessage(CommandMessage* message);
};

#endif
