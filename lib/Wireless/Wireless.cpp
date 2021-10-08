#include "Wireless.h"

#if defined(ESP8266)

void Wireless::init() {
  multi = ESP8266WiFiMulti();
  server = WebSocketsServer(WEBSOCKET_PORT);
  multi.addAP(WIRELESS_SSID, WIRELESS_PSK);

  while (multi.run() != WL_CONNECTED) {
    delay(50);
  }

  server.begin();
  server.onEvent(Wireless::handler);
}

void Wireless::handler(uint8_t num, WStype_t type, uint8_t* payload,
                       size_t length) {}

#endif
