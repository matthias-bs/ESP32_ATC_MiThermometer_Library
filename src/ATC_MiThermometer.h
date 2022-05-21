/*
   ATC_MiThermometer.h
   by matthias-bs

   https://github.com/matthias-bs/ESP32_ATC_MiThermometer_Client
   
   Based on https://bitbucket.org/dstacer/workspace/snippets/jBoazB/esp32-arduino-ide-ble-scanner-for-xiaomi by David Stacer
   which in turn is
   Based on Neil Kolban example for Arduino IDE: https://github.com/nkolban/ESP32_BLE_Arduino/tree/master/examples/BLE_scan
   Ported to Arduino ESP32 by Evandro Copercini
 
   Dumps the ServiceData for 0x1a18 BLE advertisement.
   For the wonderful firmware located https://github.com/atc1441/ATC_MiThermometer
   uses BLE library https://github.com/nkolban/ESP32_BLE_Arduino

   Decoding the data was done with help from https://github.com/JsBergbau/MiTemperature2/blob/master/LYWSD03MMC.py

   N.B.:
   Meanwhile the BLE library by Neil Kolban (https://github.com/nkolban/ESP32_BLE_Arduino) is now part of
   the Arduino core for ESP32 (https://github.com/espressif/arduino-esp32).
   
*/


//#define DEBUG
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>


// MiThermometer data struct / type
struct MiThData_S {
        bool        valid;          // data valid
        int16_t     temperature;    // temperature x 100°C
        uint16_t    humidity;       // humidity x 100%
        uint16_t    batt_voltage;   // battery voltage [mv]
        uint8_t     batt_level;     // battery level   [%]
        int16_t     rssi;           // RSSI [dBm]
};

typedef struct MiThData_S MiThData_t;

void ATC_MiThermometer_Setup(void);
void ATC_MiThermometer_Clear(int no);
