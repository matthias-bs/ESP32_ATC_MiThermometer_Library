/*
   ESP32_ATC_MiThermometer_Client
   by matthias-bs
   
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

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

const int scanTime = 5; // In seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      //      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      //    //Serial.printf("getServiceDataUUID() %s\n",  advertisedDevice.getServiceDataUUID());
      //    if ( advertisedDevice.haveServiceUUID()) {
      //      Serial.println("has service UUID");
      //    }
      //    if ( advertisedDevice.haveName()) {
      //      Serial.println("has Name");
      //    }
      
      if ( advertisedDevice.haveServiceData()) {

        char buff[20];
        int datalen;

        //Serial.printf(">>>> ServiceDataUUID 0x");
        datalen = (*advertisedDevice.getServiceDataUUID().getNative()).len;
        memcpy(buff, &(*advertisedDevice.getServiceDataUUID().getNative()).uuid, datalen);
        //Serial.printf("UUID Len  %d \n", (*advertisedDevice.getServiceDataUUID().getNative()).len);
        //Serial.printf("Service Data UUID length %d\n", datalen);
        #ifdef DEBUG
          for (int i = 0; i < datalen; i++) {
            Serial.printf("%0x", buff[i]);
          }
          Serial.println();
        #endif
        char tempUUID[] = { 0x1a, 0x18, 0x0} ;
        
        if ( strncmp( buff, tempUUID, datalen) == 0 ) {
          Serial.println("Found a ServiceDataUUID for Temperature");
          datalen = advertisedDevice.getServiceData().length();
          memcpy(buff, advertisedDevice.getServiceData().c_str(), datalen);
          #ifdef DEBUG
            Serial.printf(">>>>  Service Data ");
            for (int i = 0; i < datalen; i++) {
              //Serial.printf("%02.2x  ", buff[i]);
              Serial.printf("%0x ",advertisedDevice.getServiceData().c_str()[i]);
            }
          #endif

          if (advertisedDevice.haveName()) {
            Serial.printf("Name: %s\n", advertisedDevice.getName().c_str());
          }
          if (advertisedDevice.haveManufacturerData()) {
            Serial.printf("Manufacturer Data: %s\n", advertisedDevice.getManufacturerData().c_str());
          }
          Serial.printf("Device Address: %s\n", advertisedDevice.getAddress().toString().c_str());

          // Temperature
          int temp_msb = advertisedDevice.getServiceData().c_str()[7];
          int temp_lsb = advertisedDevice.getServiceData().c_str()[6];
          int16_t temperature = (temp_msb << 8) | temp_lsb;

          // Humidity
          int hum_msb = advertisedDevice.getServiceData().c_str()[9];
          int hum_lsb = advertisedDevice.getServiceData().c_str()[8];
          uint16_t humidity = (hum_msb << 8) | hum_lsb;

          // Battery voltage
          int volt_msb = advertisedDevice.getServiceData().c_str()[11];
          int volt_lsb = advertisedDevice.getServiceData().c_str()[10];
          uint16_t voltage = (volt_msb << 8) | volt_lsb;

          // Battery state [%]
          uint8_t battery = advertisedDevice.getServiceData().c_str()[12];
          
          Serial.println();
          Serial.println("Service Data:");
          Serial.printf("%.2f°C\n", temperature/100.0);
          Serial.printf("%.2f%%\n", humidity/100.0);
          Serial.printf("%.3fV\n", voltage/1000.0);
          Serial.printf("%d%%\n", battery);
          Serial.printf("%ddBm\n", advertisedDevice.getRSSI());
          Serial.println();
        }
        else {
          #ifdef DEBUG
            Serial.println(" ServiceDataUUID doesn't match");
          #endif
        }
      }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");
  //esp_log_level_set("*", ESP_LOG_DEBUG);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(false); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false /* is_continue */);
  Serial.print("Devices found: ");
  Serial.print(foundDevices.getCount());
  Serial.println("\nScan done!");
  Serial.println();
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(5000);
}
