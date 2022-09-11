
# ESP32_ATC_MiThermometer_Library

[![Build Status](https://app.travis-ci.com/matthias-bs/ESP32_ATC_MiThermometer_Library.svg?branch=main)](https://app.travis-ci.com/matthias-bs/ESP32_ATC_MiThermometer_Library)
[![GitHub release](https://img.shields.io/github/release/matthias-bs/ESP32_ATC_MiThermometer_Library?maxAge=3600)](https://github.com/matthias-bs/ESP32_ATC_MiThermometer_Library/releases)
[![License: MIT](https://img.shields.io/badge/license-MIT-green)](https://github.com/matthias-bs/ESP32_ATC_MiThermometer_Library/blob/main/LICENSE)

ESP32 BLE Client Library for receiving ATC_MiThermometer Data (as Advertising Data) - currently only the "custom" format is supported, **not** the original atc1441 format!

This project allows to receive data from a battery-powered bluetooth low energy thermometer/hygrometer like the Xiaomi Mijia (LYWSD03MMC) running the custom firmware [ATC_MiThermometer](https://github.com/pvvx/ATC_MiThermometer). The software runs in the Arduino environment on an ESP32.

The [ATC_MiThermometer](https://github.com/pvvx/ATC_MiThermometer) firmware sends the sensor and status data as BLE advertisements, i.e. multiple clients can receive and use the sensor data.

**Note:**

There is a known bug in the Arduino BLE library (fix available):
[ESP32 BLE scan, example works but devices found is always 0](https://forum.arduino.cc/t/esp32-ble-scan-example-works-but-devices-found-is-always-0/876703)

**You must apply this fix for using this library!**

If you have the Arduino esp32 library >= 2.0.4 installed, you don't need to apply the the fix.
