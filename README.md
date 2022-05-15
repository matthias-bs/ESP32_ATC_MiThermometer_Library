# ESP32_ATC_MiThermometer_Client
ESP32 BLE Client for receiving ATC_MiThermometer Data (as Advertising Data)

This project allows to receive data from a battery-powered bluetooth low energy thermometer/hygrometer like the Xiaomi Mijia (LYWSD03MMC) running the custom firmware [ATC_MiThermometer](https://github.com/pvvx/ATC_MiThermometer). The software runs in the Arduino environment on an ESP32.

The [ATC_MiThermometer](https://github.com/pvvx/ATC_MiThermometer) firmware sends the sensor and status data as BLE advertisements, i.e. multiple clients can receive and use the sensor data.
