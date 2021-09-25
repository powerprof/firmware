#pragma once

#ifdef ARDUINO_ESP8266_WEMOS_D1MINI

#define WIRELESS_SSID "bar"
#define WIRELESS_PSK "wi9NNYara"

#define WEBSOCKET_PORT 8081        // WS port
#define WEBSOCKETS_NETWORK_TYPE 0  // async

#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>

#include "MsgPack.h"

class Wireless {
  static ESP8266WiFiMulti multi;
  static WebSocketsServer server;

 public:
  static void handler(uint8_t num, WStype_t type, uint8_t* payload,
                      size_t length);
  static void init();
};

#endif
