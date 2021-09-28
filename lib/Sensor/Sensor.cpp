#include "Sensor.h"

Adafruit_INA260 Sensor::sensor = Adafruit_INA260();
bool Sensor::ready = false;
bool Sensor::active = false;
volatile bool Sensor::alerted = false;
volatile bool Sensor::overflowed = false;

double Sensor::fromMillis(double value) { return value * 1e-3; }

double Sensor::fromMicros(double value) { return value * 1e-6; }

#ifdef ARDUINO_ESP8266_WEMOS_D1MINI
void IRAM_ATTR Sensor::handler() {
#else
void Sensor::handler() {
#endif
  if (alerted) {
    overflowed = true;
    return;
  } else {
    overflowed = false;
  }

  // Signal alert
  alerted = true;

  // Reset sensor alert latch
  sensor.MaskEnable->read();
}

void Sensor::init() {
  // In case there is no 10k resistor on the ALERT line
  pinMode(ALERT_PIN, INPUT_PULLUP);

  // Configure I2C bus speed
  Wire.setClock(INA260_I2C_BUS_SPEED);

  uint8_t retries = INA260_INIT_ATTEMPTS;
  while (retries > 0) {
    if (!sensor.begin()) {
      MsgPack::sendDebugMessage(DEBUG_MSG_INA260_RETRY, Serial);
      retries--;
    } else {
      MsgPack::sendDebugMessage(DEBUG_MSG_INA260_CONNECTED, Serial);
      ready = true;
      break;
    }
  }

  if (!ready) {
    MsgPack::sendDebugMessage(DEBUG_MSG_INA260_FAILED, Serial);
    delay(5000);
    POWER_ON_RESET();
  }

  // Set alert and latch options
  sensor.setAlertType(INA260_ALERT_TYPE);
  sensor.setAlertLatch(INA260_ALERT_LATCH);
  configure(INA260_DEFAULT_SAMPLE_RATE);
}

void Sensor::configure(uint32_t sampleRate) {
  // Set sample rate parameters
  sensor.setVoltageConversionTime(INA260_CONVERSION_TIME);
  sensor.setCurrentConversionTime(INA260_CONVERSION_TIME);
  sensor.setAveragingCount(INA260_AVG_COUNT);
}

void Sensor::start() {
  if (active) {
    return;
  }

  // Attach ISR to "alert" pin from the INA260 to handle reading of the data
  attachInterrupt(ALERT_INTERRUPT, handler, FALLING);

  // Clear any pending alerts
  sensor.MaskEnable->read();

  // Enable alert on "Conversion Ready"
  sensor.MaskEnable->write(INA260_CONVERSION_MASK);

  active = true;
}

void Sensor::stop() {
  if (!active) {
    return;
  }

  // Detach interrupt to prevent spurious data transmission
  detachInterrupt(ALERT_INTERRUPT);
  active = false;
}

bool Sensor::read(Readings* readings) {
  if (!ready || !active || !alerted) {
    return false;
  }

  readings->current = fromMillis(sensor.readCurrent());
  readings->power = fromMillis(sensor.readPower());
  readings->timestamp = fromMicros(micros());
  readings->voltage = fromMillis(sensor.readBusVoltage());

  alerted = false;

  return true;
}
