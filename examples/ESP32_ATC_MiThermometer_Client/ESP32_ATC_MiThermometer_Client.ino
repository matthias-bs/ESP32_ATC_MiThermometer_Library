///////////////////////////////////////////////////////////////////////////////////////////////////
// ATC_MiThermometer_Client.cpp
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

const int scanTime = 5; // BLE scan time in seconds

ATC_MiThermometer miThermometer;


void setup() {
    Serial.begin(115200);
    
    // Initialization
    miThermometer.begin();
}

void loop() {
    // Set sensor data invalid
    miThermometer.resetData();
    
    // Get sensor data - run BLE scan for <scanTime>
    unsigned found = miThermometer.getData(scanTime);

    for (int i=0; i < NO_OF_SENSORS; i++) {  
        if (miThermometer.data[i].valid) {
            Serial.println();
            Serial.printf("Sensor%d:\n", i);
            Serial.printf("%.2f°C\n", miThermometer.data[i].temperature/100.0);
            Serial.printf("%.2f%%\n", miThermometer.data[i].humidity/100.0);
            Serial.printf("%.3fV\n",  miThermometer.data[i].batt_voltage/1000.0);
            Serial.printf("%d%%\n",   miThermometer.data[i].batt_level);
            Serial.printf("%ddBm\n",  miThermometer.data[i].rssi);
            Serial.println();
         }
    }
    Serial.print("Devices found: ");
    Serial.println(found);
    Serial.println();

    // Delete results fromBLEScan buffer to release memory
    miThermometer.clearScanResults();
    delay(5000);
}
