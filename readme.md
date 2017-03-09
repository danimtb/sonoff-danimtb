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


# How to flash it
This project is built with [platformio](platformio.org) to manage third party libraries such as PubSubClient, so you'll need to install it.

To flash your devices, clone this repo and run this command to flash a device connected to a USB port (remember your device should be in flash mode -GPIO0 to GND-). Keywords for supported devices are *sonoff*, *sonoff-touch*, *sonoff-s20*, *sonoff-touch-esp01*:

- First flash SPIFFS:
`$ platformio -e sonoff -t uploadfs`
- Then flash the firmware:
`$ platformio -e sonoff -t upload`

Or to flash via Arduino OTA you'll need to indicate OTA password for the device in *platformio.ini* file and then execute (ensure your device is already connected to wifi):

- First flash SPIFFS:
`$ platformio -e sonoff -t uploadfs --upload-port your-device-IP`
- Then flash the firmware:
`$ platformio -e sonoff -t upload --upload-port your-device-IP`

And that's all! :D

# How to configure for your device
The first time you flash a new device, you need to configure some parameters to connect the devie to your wifi network and mqtt server.

To do that, press and hold for 10 seconds (then release) the button and the device will create a wifi hotspot. Connect to the wifi hotspot and open browser with IP *192.168.1.4*. I will bring you to a web to configure the parameters of your device.
When you are done, press *Save Settings* button and the device will try to connect to the Wifi and MQTT network indicated.

Parameter description:

**Network Settings**
- WIFI SSID (32): Name of wifi network to connect the device to.
- WIFI PASS (32): Password of wifi network to connect the device to.
- IP (15): Static IP address of the device (Usually 192.168.1.XXX)
- MASK (15): Mask of the device in the network (Usually 255.255.0.0)
- GATEWAY (15): Gateway of the device iun the network (Usually 192.168.1.1)

**MQTT settings**
- MQTT Server (32): MQTT server to connect the device to
- MQTT Port (4): Port of the MQTT server
- MQTT Username (32): Username of the device in the MQTT network
- MQTT Password (32): Password of the device in the MQTT network

**Device Data**
- Device Name (15)
- OTA Password (32): Password for OTA updates
- MQTT Status topic (40): Status topic of the the device
- MQTT Command topic (40): Topic to command the device (Payloads accepted: ON, OFF, TOGGLE)
- MQTT Secondary Command Topic (40): Long press topic to send TOGGLE (in case you want to command other devices doing a long press of the button)
 
