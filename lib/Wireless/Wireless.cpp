#include "Wireless.h"

#if defined(ESP8266) || defined(ESP32)

#if defined(ESP8266)
ESP8266WiFiMulti Wireless::multi = ESP8266WiFiMulti();
#elif defined(ESP32)
WiFiMulti Wireless::multi = WiFiMulti();
#endif

WebSocketsServer Wireless::server = WebSocketsServer(WEBSOCKET_PORT);

void Wireless::init() {
  multi.addAP(WIRELESS_SSID, WIRELESS_PSK);

  while (multi.run() != WL_CONNECTED) {
    delay(50);
  }

  server.begin();
  server.onEvent(Wireless::handler);
}

void Wireless::handler(uint8_t num, WStype_t type, uint8_t* payload,
                       size_t length) {}

bool Wireless::readCommandMessage(CommandMessage* message) { return false; }

#endif
