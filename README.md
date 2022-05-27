# ESP32_ATC_MiThermometer_Library
ESP32 BLE Client Library for receiving ATC_MiThermometer Data (as Advertising Data)

This project allows to receive data from a battery-powered bluetooth low energy thermometer/hygrometer like the Xiaomi Mijia (LYWSD03MMC) running the custom firmware [ATC_MiThermometer](https://github.com/pvvx/ATC_MiThermometer). The software runs in the Arduino environment on an ESP32.

The [ATC_MiThermometer](https://github.com/pvvx/ATC_MiThermometer) firmware sends the sensor and status data as BLE advertisements, i.e. multiple clients can receive and use the sensor data.

**Note:**

There is a known bug in the Arduino BLE library (fix available):
[ESP32 BLE scan, example works but devices found is always 0](https://forum.arduino.cc/t/esp32-ble-scan-example-works-but-devices-found-is-always-0/876703)

**You must apply this fix for using this library!**
