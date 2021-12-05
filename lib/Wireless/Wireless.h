#pragma once

#include <iostream>

#define ENABLE_WIFI defined(ESP8266) || defined(ESP32)

#if ENABLE_WIFI

#ifndef WIRELESS_SSID
#error "Must supply WIRELESS_SSID!"
#endif
#ifndef WIRELESS_PSK
#error "Must supply WIRELESS_PSK!"
#endif

#define WEBSOCKET_PORT 8081

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
