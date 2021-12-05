#pragma once

#include <Arduino.h>

#if defined(ARDUINO_TEENSY36)
#define LED_PIN 13
#elif defined(ESP8266)
#define LED_PIN LED_BUILTIN
#elif defined(ESP32)
#define LED_PIN 2
#endif
#define LED_ON_STATE HIGH
#define LED_BLINK_INTERVAL 500

// Adapted from FastLED source code
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
