#pragma once

#define ENABLE_BLE defined(ESP32)

#if ENABLE_BLE

#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BluetoothSerial.h>
#include <MsgPack.h>

#define BLE_DEVICE_NAME "PowerProf"
#define BLE_SERVICE_UUID "08592452-d06d-45f8-9791-2282da21defd"
#define BLE_VOLTAGE_CHAR_HANDLE 0
#define BLE_VOLTAGE_CHAR_UUID "ab6fcda5-8754-4301-95cc-7ee92bb2987b"
#define BLE_CURRENT_CHAR_HANDLE 1
#define BLE_CURRENT_CHAR_UUID "59ad55ee-0fad-4b79-87d8-3109e1cb3bc0"
#define BLE_TIMESTAMP_CHAR_HANDLE 2
#define BLE_TIMESTAMP_CHAR_UUID "2c17c279-7d93-4bc6-95cb-e84c6e834e9c"
#define BLE_POWER_CHAR_HANDLE 3
#define BLE_POWER_CHAR_UUID "19e391c5-e3bd-438d-973f-6b8283f993c9"

class Bluetooth {
  static BluetoothSerial *serialPort;
  // static BLEAdvertising *advertiser;
  // static BLEServer *server;
  // static BLEService *service;
  // static BLECharacteristicMap *characteristics;

 public:
  static void init();
  // static bool connected();
  // static void update(Readings *readings);
  static bool readCommandMessage(CommandMessage *message);
  static void writeReadings(const Readings readings);
};

#endif
