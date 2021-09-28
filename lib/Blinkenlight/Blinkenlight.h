#pragma once

#include <Arduino.h>

#if defined(ARDUINO_TEENSY36)
#define LED_PIN 13
#define LED_ON_STATE HIGH
#elif defined(ARDUINO_ESP8266_WEMOS_D1MINI)
#define LED_PIN LED_BUILTIN
#define LED_ON_STATE LOW
#endif

#define LED_BLINK_INTERVAL 500

class Counter {
 public:
  uint32_t prevTrigger;
  Counter() { reset(); };
  uint32_t getTime() { return (uint32_t)(millis()); };
  uint32_t getElapsed() { return getTime() - prevTrigger; }
  bool ready() {
    const bool isReady = (getElapsed() >= LED_BLINK_INTERVAL);
    if (isReady) {
      reset();
    }
    return isReady;
  }
  void reset() { prevTrigger = getTime(); };
  void trigger() { prevTrigger = getTime() - LED_BLINK_INTERVAL; };
  operator bool() { return ready(); }
};

struct Blinkenlight {
  static void init();
  static void update();
};
