# esp_abus

[![Library Manager: esp_abus](https://www.ardu-badge.com/badge/esp_abus.svg)](https://www.ardu-badge.com/esp_abus)

ABUS Socket Communication library for PLCs (Cybro-2 and Cybro-3) from Cybrotech and ESP32 / ESP8266 over WiFi

## Description

It uses the UDP-Protocol to communicate to the PLC with so called ABUS Sockets.
For the communication you need to specify at least one abus socket in the PLC program (for using the examples you can specify a new socket in the CyPro Environment with the following configuration: socket id 3, one bool tag, one integer tag, one long tag and one real tag).
You can add up to 32 receive sockets with callbacks in the ESP program.

## Usage

For a detailed usage check the examples folder

## License

This library is free software
