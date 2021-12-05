#include "main.hpp"

/**
 * Initialize hardware.
 */
void setup() {
  // Initialize the LED
  Blinkenlight::init();

  // Initialize Serial output and log hardware info.
#if ENABLE_USB
  Usb::init(true);
#else
  Usb::init(false);
#endif

  // Initialize WiFi and/or Bluetooth on compatible devices.
#if ENABLE_WIFI
  Wireless::init();
#endif
#if ENABLE_BLE
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
  auto commandReceived = false;

  commandReceived = Usb::readCommandMessage(&command);
  if (commandReceived) {
    Sensor::executeCommandMessage(command);
  }

#if ENABLE_WIFI
  commandReceived = Wireless::readCommandMessage(&command);
  if (commandReceived) {
    Sensor::executeCommandMessage(command);
  }
#endif

#if ENABLE_BLE
  commandReceived = Bluetooth::readCommandMessage(&command);
  if (commandReceived) {
    Sensor::executeCommandMessage(command);
  }
#endif

  // Check for alerts, send data over serial if available.
  auto readings = Readings();
  if (Sensor::read(&readings)) {
    MsgPack::sendReadings(Serial, readings);

    // Toggle LED on and off
    Blinkenlight::update();
  }
}
