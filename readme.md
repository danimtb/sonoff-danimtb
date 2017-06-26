sonoff-danimtb
==============

[![Build Status](https://travis-ci.org/danimtb/sonoff-danimtb.svg?branch=master)](https://travis-ci.org/danimtb/sonoff-danimtb)

This is an alternative firmware for some sonoff devices to use them over Wifi with MQTT communication protocol. The main purpose of this firmware is to perform as best with [Home Assistant](home-assistant.io).

It is design with these features in mind:
- Simple low couple clases with all functionality ([esp8266-utils](https://github.com/danimtb/esp8266-utils) -Relay, LED, Button, WifiManager, MqttManager, DataManager (EEPROM)...).
- Easy to use and reusable to create firmware for other devices.
- C++ code and Arduino interface approach (setup, loop).

It's capabilities are:
- Support for devices with similar pins and configuration -custom esp8266 or esp8285 based boards with just one output pin (relay) and 1 input pin (button)-. CUrrently suporting sonoff, sonoff touch, sonoff S20 and sonoff POW;
- Fast connection to Wifi network and MQTT broker.
- Working button toggling relay even when disconnected from Wifi and MQTT.
- Publish device information and state topics to MQTT at connection start and periodically.
- ArduinoOTA to target devices flashing a new firmware over wifi.
- Automatic Updates from devices calling an Update Server with new releases.


# How to flash it
This project is built with [platformio](platformio.org) to manage third party libraries such as AsyncMqtt, Embedis, ArduinoJson... so you will need to install it.

To flash a device, clone this repo and run this command with the device connected to a USB port (remember your device should be in flash mode -usually GPIO0 to GND-). Keywords for supported devices are *sonoff*, *sonoff-touch*, *sonoff-s20*, *sonoff-touch-esp01*:

- First flash SPIFFS:
`$ platformio run -e sonoff -t uploadfs`
- Then flash the firmware:
`$ platformio run -e sonoff -t upload`

Or to flash with Arduino OTA over Wifi:

- First flash SPIFFS:
`$ platformio run -e sonoff -t uploadfs --upload-port your-device-IP`
- Then flash the firmware:
`$ platformio run -e sonoff -t upload --upload-port your-device-IP`

And that's all! :D

# How to configure for your device
The first time you flash a new device, you need to configure some parameters to connect the devie to your wifi network and mqtt server.

To do that, press and hold for 10 seconds (then release) the button and the device will create a wifi hotspot with its name. Connect to the wifi hotspot and open browser with IP *192.168.1.4*. I will bring you to a web to configure the parameters of your device.
When you are done, press *Save Settings* button and the device will try to connect to the Wifi and MQTT network indicated.

Parameter description:

**Network Settings**
- WIFI SSID (32): Name of wifi network to connect the device to.
- WIFI PASS (32): Password of wifi network to connect the device to.
- IP (15): Static IP address of the device (Usually 192.168.1.XXX)
- MASK (15): Mask of the device in the network (Usually 255.255.0.0)
- GATEWAY (15): Gateway of the device iun the network (Usually 192.168.1.1)
- OTA Update Server (32): IP address + port of the [arduinoUpdateServer](https://github.com/danimtb/arduinoUpdateServer). E.G. "http://192.168.0.10:8266"

**MQTT settings**
- MQTT Server (32): MQTT server to connect the device to
- MQTT Port (4): Port of the MQTT server
- MQTT Username (32): Username of the device in the MQTT network
- MQTT Password (32): Password of the device in the MQTT network

**Device Data**
- Device Name (15)
- MQTT Status topic (40): Status topic of the the device
- MQTT Command topic (40): Topic to command the device (Payloads accepted: ON, OFF, TOGGLE)
- MQTT Secondary Command Topic (40): Long press topic to send TOGGLE (in case you want to command other devices doing a long press of the button -if not, leave it empty or the same as MQTT Command Topic-)
 
