#pragma once

#include <Adafruit_INA260.h>
#include <Arduino.h>

#include <map>

#include "Blinkenlight.h"
#include "MsgPack.h"

#define INA260_ALERT_LATCH INA260_ALERT_LATCH_ENABLED
#define INA260_ALERT_TYPE INA260_ALERT_CONVERSION_READY
#define INA260_CONVERSION_MASK 0x400
#define INA260_DEFAULT_AVERAGE_COUNT INA260_COUNT_16
#define INA260_DEFAULT_CONVERSION_TIME INA260_TIME_558_us
#define INA260_DEFAULT_RAW_AVERAGE_COUNT 16
#define INA260_DEFAULT_RAW_CONVERSION_TIME 558
// in Hz
#define INA260_DEFAULT_SAMPLE_RATE 112
#define INA260_MIN_SAMPLE_RATE 100U
#define INA260_MAX_SAMPLE_RATE 6500U
// 400 kbit/s
#define INA260_I2C_BUS_SPEED 4e5
// 2 mbit/s
// #define INA260_I2C_BUS_SPEED 2e6
#define INA260_INIT_ATTEMPTS 5

#define DEBUG_MSG_INA260_CONNECTED F("INA260 connected!")
#define DEBUG_MSG_INA260_FAILED F("INA260 connection failed, restarting!")
#define DEBUG_MSG_BUFFER_OVERFLOW F("INA260 alerted before it was reset!")
#define DEBUG_MSG_INA260_RETRY F("INA260 connection retry...")

// Function to trigger ARM power on reset
#if defined(ARDUINO_TEENSY36)
#define ALERT_PIN A7

#define RESTART_ADDR 0xE000ED0C
#define RESTART_VAL 0x5FA0004

#define POWER_ON_RESET() ((*(volatile uint32_t*)RESTART_ADDR) = (RESTART_VAL))
#elif defined(ESP8266) || defined(ESP32)
#if defined(ESP8266)
#define ALERT_PIN D0
#else
#define ALERT_PIN 34
#endif

#define POWER_ON_RESET() ESP.restart();
#endif

#define ALERT_INTERRUPT digitalPinToInterrupt(ALERT_PIN)

class Sensor {
  static Adafruit_INA260 sensor;
  static bool ready;
  static bool active;
  static volatile bool alerted;
  static volatile bool overflowed;
  static double fromMillis(double value);
  static double fromMicros(double value);
  static void handler();

 public:
  static void init();
  static void setup(uint32_t sampleRate);
  static void start();
  static void stop();
  static bool read(Readings* readings);
  static void executeCommandMessage(const CommandMessage message);
};
