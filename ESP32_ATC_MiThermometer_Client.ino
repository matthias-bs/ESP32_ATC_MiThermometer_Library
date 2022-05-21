/*
   ESP32_ATC_MiThermometer_Client
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

// No. of known sensors
#define NO_OF_SENSORS 2

// BLE adresses of known sensors 
std::string knownBLEAddresses[] = {"a4:c1:38:b8:1f:7f", "a4:c1:38:bf:e1:bc", ""};

#include "src/ATC_MiThermometer.h"
#include <string>

const int scanTime = 5; // In seconds
BLEScan* pBLEScan;

// MiThermometer data - sensor address determins array index
MiThData_t miThData[NO_OF_SENSORS];


void setup() {
    Serial.begin(115200);
    Serial.println("Scanning...");
    //esp_log_level_set("*", ESP_LOG_DEBUG);

    ATC_MiThermometer_Setup();
}

void loop() {
    ATC_MiThermometer_Clear(NO_OF_SENSORS);
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false /* is_continue */);

    for (int i=0; i < NO_OF_SENSORS; i++) {    
        if (miThData[i].valid) {
            Serial.println();
            Serial.printf("Sensor%d:\n", i);
            Serial.printf("%.2f°C\n", miThData[i].temperature/100.0);
            Serial.printf("%.2f%%\n", miThData[i].humidity/100.0);
            Serial.printf("%.3fV\n", miThData[i].batt_voltage/1000.0);
            Serial.printf("%d%%\n", miThData[i].batt_level);
            Serial.printf("%ddBm\n", miThData[i].rssi);
            Serial.println();
         }
    }
    Serial.print("Devices found: ");
    // Known bug in Arduino BLE library - fix available:
    // "ESP32 BLE scan, example works but devices found is always 0"
    // https://forum.arduino.cc/t/esp32-ble-scan-example-works-but-devices-found-is-always-0/876703
    Serial.print(foundDevices.getCount());
    Serial.println("\nScan done!");
    Serial.println();

    // delete results fromBLEScan buffer to release memory
    pBLEScan->clearResults();
    delay(5000);
}
