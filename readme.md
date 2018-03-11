sonoff-danimtb
==============

[![Build Status](https://travis-ci.org/danimtb/sonoff-danimtb.svg?branch=master)](https://travis-ci.org/danimtb/sonoff-danimtb)

This is an alternative firmware for some sonoff devices to use them over Wifi with MQTT communication protocol. The main purpose of this firmware is to perform as best with [Home Assistant](home-assistant.io).

It is designed with these features in mind:
- Simple low coupled clases with all functionality ([esp8266-utils](https://github.com/danimtb/esp8266-utils) -Relay, LED, Button, WifiManager, MqttManager, DataManager (EEPROM)...).
- Easy to use and reusable to create firmware for other devices.
- C++ code and Arduino interface approach (``setup()``, ``loop()``).

It's capabilities are:
- Support for devices with similar pins and configuration -custom esp8266 or esp8285 based boards-. Currently suporting sonoff, sonoff touch, sonoff S20 and sonoff POW.
- Fast connection to Wifi network and MQTT broker.
- Working button toggling relay even when disconnected from Wifi and MQTT.
- Publish device information and state topics to MQTT at connection start and periodically.
- ArduinoOTA to target devices flashing a new firmware over Wifi.
- Automatic Updates from devices calling an Update Server with new releases.
- Support for external switch or button using the additional GPIO (Only for sonoff). 

# How to flash it
This project is built with [platformio](https://platformio.org) to manage third party libraries such as AsyncMqtt, Embedis, ArduinoJson... so you will need to install it. The best way to install it is using pip (only compatible with python 2.7):

```
$ pip install platformio
```

To flash a device, clone this repo and run the following commands with the device connected to a USB port (remember your device should be in flash mode -usually GPIO0 to GND-). Keywords for supported devices are **sonoff**, **sonoff-touch**, **sonoff-s20**, **sonoff-touch-esp01**, **sonoff-switch**, **sonoff-button**:

- First flash the file system (SPIFFS) where the HTML web is sotored (contents of *data* folder in this repo):
`$ platformio run -e <device-type> -t uploadfs`
- Then flash the firmware:
`$ platformio run -e <device-type> -t upload`

For example, in case you want to flash a **sonoff** device:

```
$ platformio run -e sonoff -t uploadfs
...
...
$ platformio run -e sonoff -t upload
```

Or in case your device is already connected to a Wifi running an old version of this firmare, you can it flash with Arduino OTA over Wifi:

- First flash the file system (SPIFFS):
`$ platformio run -e sonoff -t uploadfs --upload-port your-device-IP`
- Then flash the firmware:
`$ platformio run -e sonoff -t upload --upload-port your-device-IP`

And that's all! :D

# How to configure for your device
The first time you flash a new device, you need to configure some parameters to connect the devie to your Wifi network and MQTT server.

To do that, press and hold for 10 seconds or more (then release) the button and the device will start flashing the LED (if any) and will create a Wifi HotSpot with the name of the device type. Connect to the wifi hotspot and open browser with IP *192.168.4.1*. It will bring you to a web to configure the parameters of your device.
When you are done, press *Save Settings* button and the device will try to connect to the Wifi and MQTT network indicated.

Parameter description:

**Network Settings**
- WIFI SSID : Name of wifi network to connect the device to.
- WIFI PASSWORD: Password of wifi network to connect the device to.
- IP: Static IP address of the device (Usually 192.168.1.XXX)
- MASK: Mask of the device in the network (Usually 255.255.0.0)
- GATEWAY: Gateway of the device iun the network (Usually 192.168.1.1)
- OTA Update Server: IP address + port of the [arduinoUpdateServer](https://github.com/danimtb/arduinoUpdateServer). E.G. "http://192.168.0.10:8266"

**MQTT settings**
- MQTT Server: MQTT server to connect the device to
- MQTT Port: Port of the MQTT server
- MQTT Username: Username of the device in the MQTT network
- MQTT Password: Password of the device in the MQTT network

**Device Data**
- Device Name
- MQTT Status topic: Status topic of the the device
- MQTT Command topic: Topic to command the device (Payloads accepted: ON, OFF, TOGGLE)
- MQTT Secondary Command Topic: Long press topic to send TOGGLE (in case you want to command other devices doing a long press of the button -if not, leave it empty or the same as MQTT Command Topic-)
 
