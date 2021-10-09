#include "Bluetooth.h"

#if defined(ESP32)

BluetoothSerial* Bluetooth::serialPort = new BluetoothSerial();

void Bluetooth::init() {
  serialPort->begin(BLE_DEVICE_NAME, true);
  serialPort->connect("slave");

  while (!serialPort->connected(1000)) {
    delay(100);
  }
  //   BLEDevice::init(BLE_DEVICE_NAME);
  //   server = BLEDevice::createServer();
  //   service = server->createService(BLE_SERVICE_UUID);
  //   characteristics = new BLECharacteristicMap();

  //   auto timeChar = service->createCharacteristic(
  //       BLE_TIMESTAMP_CHAR_UUID, BLECharacteristic::PROPERTY_READ);
  //   auto voltChar = service->createCharacteristic(
  //       BLE_VOLTAGE_CHAR_UUID, BLECharacteristic::PROPERTY_READ);
  //   auto currChar = service->createCharacteristic(
  //       BLE_CURRENT_CHAR_UUID, BLECharacteristic::PROPERTY_READ);
  //   auto powChar = service->createCharacteristic(
  //       BLE_POWER_CHAR_UUID, BLECharacteristic::PROPERTY_READ);

  //   timeChar->addDescriptor(new BLE2902());
  //   voltChar->addDescriptor(new BLE2902());
  //   currChar->addDescriptor(new BLE2902());
  //   powChar->addDescriptor(new BLE2902());

  //   characteristics->setByHandle(BLE_TIMESTAMP_CHAR_HANDLE, timeChar);
  //   characteristics->setByHandle(BLE_VOLTAGE_CHAR_HANDLE, voltChar);
  //   characteristics->setByHandle(BLE_CURRENT_CHAR_HANDLE, currChar);
  //   characteristics->setByHandle(BLE_POWER_CHAR_HANDLE, powChar);

  //   advertiser = BLEDevice::getAdvertising();
  //   advertiser->addServiceUUID(BLE_SERVICE_UUID);
  //   advertiser->setScanResponse(false);

  //   BLEDevice::startAdvertising();
}

// bool Bluetooth::connected() { return serial->connected(1000); }

// void Bluetooth::update(Readings* readings) {
//   characteristics->getByHandle(BLE_TIMESTAMP_CHAR_HANDLE)
//       ->setValue(readings->timestamp);
//   characteristics->getByHandle(BLE_VOLTAGE_CHAR_HANDLE)
//       ->setValue(readings->voltage);
//   characteristics->getByHandle(BLE_CURRENT_CHAR_HANDLE)
//       ->setValue(readings->current);
//   characteristics->getByHandle(BLE_POWER_CHAR_HANDLE)
//       ->setValue(readings->power);
// }

bool Bluetooth::readCommandMessage(CommandMessage* message) {
  if (serialPort->available() > 0) {
    return MsgPack::readCommandMessage(message, *serialPort);
  }

  return false;
}

void Bluetooth::writeReadings(const Readings readings) {
  MsgPack::sendReadings(readings, *serialPort);
}

#endif
