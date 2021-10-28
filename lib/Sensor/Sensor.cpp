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
      MsgPack::sendDebugMessage(Serial, DEBUG_MSG_INA260_RETRY);
      retries--;
    } else {
      MsgPack::sendDebugMessage(Serial, DEBUG_MSG_INA260_CONNECTED);
      ready = true;
      break;
    }
  }

  if (!ready) {
    MsgPack::sendDebugMessage(Serial, DEBUG_MSG_INA260_FAILED);
    delay(5000);
    POWER_ON_RESET();
  }

  // Set alert and latch options
  sensor.setAlertType(INA260_ALERT_TYPE);
  sensor.setAlertLatch(INA260_ALERT_LATCH);
  // Set default sample rate
  sensor.setVoltageConversionTime(INA260_DEFAULT_CONVERSION_TIME);
  sensor.setCurrentConversionTime(INA260_DEFAULT_CONVERSION_TIME);
  sensor.setAveragingCount(INA260_DEFAULT_AVERAGE_COUNT);
}

void Sensor::setup(uint32_t sampleRate) {
  if (active) {
    return;
  }

  uint32_t clamped =
      min(max(sampleRate, INA260_MAX_SAMPLE_RATE), INA260_MIN_SAMPLE_RATE);
  float targetTimeMs = (1.0 / clamped) * 1e3;
  INA260_ConversionTime selectedTime = INA260_DEFAULT_CONVERSION_TIME;
  INA260_AveragingCount selectedAverageCount = INA260_DEFAULT_AVERAGE_COUNT;
  float lowestDifference =
      (INA260_DEFAULT_RAW_AVERAGE_COUNT * INA260_DEFAULT_RAW_CONVERSION_TIME) /
      1e3;

  for (auto averageIterator = averageCountMap.end();
       averageIterator != averageCountMap.begin(); --averageIterator) {
    for (auto sampleIterator = sampleRateMap.begin();
         sampleIterator != sampleRateMap.end(); ++sampleIterator) {
      float sampleTimeMs =
          (sampleIterator->first * averageIterator->first) / 1e3;
      float timeDifference = abs(targetTimeMs - sampleTimeMs);

      if (timeDifference < lowestDifference) {
        lowestDifference = timeDifference;
        selectedTime = sampleIterator->second;
        selectedAverageCount = averageIterator->second;
      }
    }
  }

  sensor.setVoltageConversionTime(selectedTime);
  sensor.setCurrentConversionTime(selectedTime);
  sensor.setAveragingCount(selectedAverageCount);
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
