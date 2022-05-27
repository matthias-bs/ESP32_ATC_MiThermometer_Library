///////////////////////////////////////////////////////////////////////////////////////////////////
// ATC_MiThermometer.cpp
//
// Bresser 5-in-1/6-in-1 868 MHz Weather Sensor Radio Receiver 
// based on CC1101 or SX1276/RFM95W and ESP32/ESP8266
//
// https://github.com/matthias-bs/ESP32_ATC_MiThermometer_Library
//
// Based on:
// ---------
// ESP32 BLE for Arduino (https://github.com/espressif/arduino-esp32/tree/master/libraries/BLE)
// LYWSD03MMC.py by JsBergbau (https://github.com/JsBergbau/MiTemperature2)
//
// created: 05/2022
//
//
// MIT License
//
// Copyright (c) 2022 Matthias Prinke
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// History:
//
// 20220521 Created
// 20220527 Changed to a class/into a library
//
// ToDo: 
// - Pass list of known sensor's addresses to object
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "ATC_MiThermometer.h"

//extern std::string knownBLEAddresses[];

// Callback for advertised device found during scan
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {      
        if (advertisedDevice.haveServiceData()) {
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
                datalen = advertisedDevice.getServiceData().length();
                memcpy(buff, advertisedDevice.getServiceData().c_str(), datalen);
                #ifdef DEBUG
                    Serial.println("Found a ServiceDataUUID for Temperature");
                    Serial.printf(">>>>  Service Data ");
                    for (int i = 0; i < datalen; i++) {
                        //Serial.printf("%02.2x  ", buff[i]);
                        Serial.printf("%0x ",advertisedDevice.getServiceData().c_str()[i]);
                    }

                    if (advertisedDevice.haveName()) {
                        Serial.printf("Name: %s\n", advertisedDevice.getName().c_str());
                    }
                    if (advertisedDevice.haveManufacturerData()) {
                        Serial.printf("Manufacturer Data: %s\n", advertisedDevice.getManufacturerData().c_str());
                    }
                    Serial.printf("Device Address: %s\n", advertisedDevice.getAddress().toString().c_str());
               #endif
           } else {
               #ifdef DEBUG
                   Serial.println(" ServiceDataUUID doesn't match");
               #endif
           }
        } // if (advertisedDevice.haveServiceData())
    } // void onResult(BLEAdvertisedDevice advertisedDevice)
};


// Set up BLE scanning
void ATC_MiThermometer::begin(void)
{
    BLEDevice::init("");
    _pBLEScan = BLEDevice::getScan(); //create new scan
    _pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    _pBLEScan->setActiveScan(false); //active scan uses more power, but get results faster
    _pBLEScan->setInterval(100);
    _pBLEScan->setWindow(99);  // less or equal setInterval value
}

unsigned ATC_MiThermometer::getData(uint32_t duration) {
    BLEScanResults foundDevices = _pBLEScan->start(duration, false /* is_continue */);

    // Known bug in Arduino BLE library - fix available:
    // "ESP32 BLE scan, example works but devices found is always 0"
    // https://forum.arduino.cc/t/esp32-ble-scan-example-works-but-devices-found-is-always-0/876703    
    for (unsigned i=0; i< foundDevices.getCount(); i++) {
        for (unsigned n = 0; n < NO_OF_SENSORS; n++) {
            
            #ifdef DEBUG
                Serial.printf("Device Address: %s\n", foundDevices.getDevice(i).getAddress().toString().c_str());
                Serial.printf("Comp.  Address: %s\n", BLEAddress(knownBLEAddresses[n]).toString().c_str());
            #endif
            if (foundDevices.getDevice(i).getAddress() == BLEAddress(knownBLEAddresses[n])) {

                data[n].valid = true;
            
                // Temperature
                int temp_msb = foundDevices.getDevice(i).getServiceData().c_str()[7];
                int temp_lsb = foundDevices.getDevice(i).getServiceData().c_str()[6];
                data[n].temperature = (temp_msb << 8) | temp_lsb;

                // Humidity
                int hum_msb = foundDevices.getDevice(i).getServiceData().c_str()[9];
                int hum_lsb = foundDevices.getDevice(i).getServiceData().c_str()[8];
                data[n].humidity = (hum_msb << 8) | hum_lsb;

                // Battery voltage
                int volt_msb = foundDevices.getDevice(i).getServiceData().c_str()[11];
                int volt_lsb = foundDevices.getDevice(i).getServiceData().c_str()[10];
                data[n].batt_voltage = (volt_msb << 8) | volt_lsb;

                // Battery state [%]
                data[n].batt_level = foundDevices.getDevice(i).getServiceData().c_str()[12];         
            
                // Received Signal Strength Indicator [dBm]
                data[n].rssi = foundDevices.getDevice(i).getRSSI();
            }
        }
    }
    return foundDevices.getCount();
}

        
// Set all array members invalid
void ATC_MiThermometer::resetData(void)
{
    for (int i=0; i < NO_OF_SENSORS; i++) {
        data[i].valid = false;
    }
}

