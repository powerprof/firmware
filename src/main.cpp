#include "main.hpp"

/**
 * Initialize hardware.
 */
void setup() {
  // Initialize the LED
  Blinkenlight::init();

  // Initialize Serial output and log hardware info.
#if defined(ARDUINO_TEENSY)
  Usb::init(true);
#else
  Usb::init(false);
#endif

  // Initialize WiFi and/or Bluetooth on compatible devices.
#if defined(ESP8266)
  Wireless::init();
#elif defined(ESP32)
  Wireless::init();
  Bluetooth::init();
#endif

  // Initialize the INA260
  Sensor::init();
}

/**
 * Check for fatal errors and send readings when they are avaialble.
 */
void loop() {
  // Check for commands, execute if one is waiting.
  auto command = CommandMessage();
  bool commandReceived = false;

#if defined(ESP8266)
  commandReceived = Wireless::readCommandMessage(&command);
#elif defined(ESP32)
  commandReceived = Bluetooth::readCommandMessage(&command);
#elif defined(ARDUINO_TEENSY)
  commandReceived = Usb::readCommandMessage(&command);
#endif

  if (commandReceived) {
    Sensor::executeCommandMessage(command);
  }

  // Check for alerts, send data over serial if available.
  auto readings = Readings();
  if (Sensor::read(&readings)) {
    MsgPack::sendReadings(readings, Serial);

    // Toggle LED on and off
    Blinkenlight::update();
  }
}
