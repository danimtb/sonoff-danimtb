sonoff-danimtb
==============

[![Build Status](https://travis-ci.org/danimtb/sonoff-danimtb.svg?branch=master)](https://travis-ci.org/danimtb/sonoff-danimtb)

This FW is an alternative FW for some sonoff devices to use them with MQTT communication protocol. The main purpose for this firmware is to perform as best with [Home Assistant](home-assistant.io).

It is design with these features in mind:
- Simple low couple clases with all functionality (Relay, LED, Button, WifiMQTTManager...).
- Easy to use and reusable for other FW.
- C++ code and Arduino interface approach (setup, loop).

It's capabilities are:
- Support for sonoff, sonoff touch, sonoff S20, and devices with similar pins and configuration -custom esp8266 or esp8285 based boards with just one output pin (relay) and 1 input pin (button)-.
- Fast connection to Wifi network and MQTT broker.
- Working button toggling relay even when disconnected from wifi/mqtt.
- Publish device information and state topics to MQTT at connection start and periodically.
- ArduinoOTA  flashing over wifi.


# How to configure for your device
To flash a new device you need to configure some parameters for your setup gathered in two files in *data* folder:

- DeviceData.h: All device data information is placed in this file. You'll new to define:
  - DEVICE_NAME: name of your device used in device infomartion topics and to register as a client in MQTT broker.
  - DEVICE_TYPE: name of the kind of hardware device you are using (sonoff, sonoff-touch, sonoff-s20, sonoff-touch-esp01)
  - IP_NUMBER: last 3 numbers for the device ip number (currently only 192.168.1.XXX networks).
  - SET_TOPIC: MQTT topic that the devices subscribes to change its state.
  - STATUS_TOPIC: MQTT topic where the devices publishes its state.
  - SECONDARY_TOPIC: MQTT additional topic to command other device with long press of button. It is used to togle other device state (if you don't need this option set it to the same as SET_TOPIC).

- NetworkData.h: All network information of wifi, mqtt and ArduinoOTA.
  - WIFI_SSID
  - WIFI_PASS
  - MQTT_SERVER: MQTT broker IP.
  - MQTT_PORT: MQTT broker port.
  - MQTT_USERNAME: Username to register in MQTT broker.
  - MQTT_PASSWORD: Password to register in MQTT broker.
  - OTA_PORT: Port to receive arduino OTA.
  - OTA_PASS: Password to flash an OTA.
  
# How to flash it!
This project is built with [platformio](platformio.org) to manage third party libraries such as PubSubClient, so you'll need to install it.

To flash your devices, clone this repo and fill your device and network data. Then run this command to flash a device connected to your COM3 (remember your device should be in flash mode -GPIO0 to GND-):

`$ platformio run --target upload --upload-port COM3`

Or to flash via Arduino OTA you'll need to indicate IP and OTA password for the device in *platformio.ini* file and then execute (ensure your device is already connected to wifi):

`$ platformio run --target upload`

And that's all! :D
