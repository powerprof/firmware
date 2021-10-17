#include "Sensor.h"

std::map<uint16_t, INA260_ConversionTime> sampleRateMap = {
    {140, INA260_TIME_140_us},    {204, INA260_TIME_204_us},
    {332, INA260_TIME_332_us},    {558, INA260_TIME_558_us},
    {1100, INA260_TIME_1_1_ms},   {2116, INA260_TIME_2_116_ms},
    {4156, INA260_TIME_4_156_ms}, {8244, INA260_TIME_8_244_ms}};
std::map<uint16_t, INA260_AveragingCount> averageCountMap = {
    {1, INA260_COUNT_1},     {4, INA260_COUNT_4},      {16, INA260_COUNT_16},
    {64, INA260_COUNT_64},   {128, INA260_COUNT_128},  {256, INA260_COUNT_256},
    {512, INA260_COUNT_512}, {1024, INA260_COUNT_1024}};

Adafruit_INA260 Sensor::sensor = Adafruit_INA260();
bool Sensor::ready = false;
bool Sensor::active = false;
volatile bool Sensor::alerted = false;
volatile bool Sensor::overflowed = false;

double Sensor::fromMillis(double value) { return value * 1e-3; }

double Sensor::fromMicros(double value) { return value * 1e-6; }

#if defined(ESP8266) || defined(ESP32)
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
  setup(INA260_DEFAULT_SAMPLE_RATE);
}

void Sensor::setup(uint32_t sampleRate) {
  uint32_t clamped = min(max(sampleRate, 110U), 6500U);
  float targetTime = 1 / clamped;

  char buffer[32];
  sprintf(buffer, "Target %.4fs\n", targetTime);
  MsgPack::sendDebugMessage(buffer, Serial);

  for (auto averageIterator = averageCountMap.end();
       averageIterator != averageCountMap.begin(); --averageIterator) {
    for (auto sampleIterator = sampleRateMap.end();
         sampleIterator != sampleRateMap.begin(); --sampleIterator) {
      float sampleTime = (sampleIterator->first * averageIterator->first) / 1e6;

      sprintf(buffer, "Sample %.4fs\n", sampleTime);
      MsgPack::sendDebugMessage(buffer, Serial);

      if (max(sampleTime, targetTime) - min(sampleTime, targetTime) < 0.5) {
        MsgPack::sendDebugMessage("Choosing that\n", Serial);
        sensor.setVoltageConversionTime(sampleIterator->second);
        sensor.setCurrentConversionTime(sampleIterator->second);
        sensor.setAveragingCount(averageIterator->second);
        return;
      }
    }
  }
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

  Blinkenlight::init();

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

void Sensor::executeCommandMessage(const CommandMessage message) {
  switch (message.command) {
    case Command::Setup:
      return Sensor::setup(message.sampleRate);
    case Command::Start:
      return Sensor::start();
    case Command::Stop:
      return Sensor::stop();
    case Command::None:
    default:
      return;
  }
}
