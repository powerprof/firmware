#include "main.h"

/**
 * Initialize hardware.
 */
void setup() {
  // Initialize the LED
  Blinkenlight::init();

  // Initialize Serial output and log hardware info
  // Configure serial port
  Serial.begin(SERIAL_BAUD);

  // Wait until serial port is opened
  while (!Serial) {
    delay(10);
  }
  MsgPack::sendDebugMessage(DEBUG_MSG_APP_ID, Serial);

  // Initialize the INA260
  Sensor::init();
}

/**
 * Check for fatal errors and send readings when they are avaialble.
 */
void loop() {
  // Reboot if we lost serial connection.
  if (!Serial) {
    POWER_ON_RESET();
  }

  // Toggle LED on and off
  Blinkenlight::update();

  // Check for commands, update sensor state
  if (Serial.available() > 0) {
    auto command = CommandMessage();
    if (MsgPack::readCommandMessage(&command, Serial)) {
      switch (command.command) {
        case Command::Setup:
          return Sensor::configure(command.sampleRate);
        case Command::Start:
          return Sensor::start();
        case Command::Stop:
          return Sensor::stop();
        case Command::None:
          return;
      }
    }
  }

  // Check for alerts, send data over serial if available.
  auto readings = Readings();
  if (Sensor::read(&readings)) {
    MsgPack::sendReadings(readings, Serial);
  }
}
