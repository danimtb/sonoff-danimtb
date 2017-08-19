win32 {
    HOMEDIR += $$(USERPROFILE)
}
else {
    HOMEDIR += $$(HOME)
}

INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/tools/sdk/include"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/tools/sdk/lwip/include"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/cores/esp8266"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/variants/generic"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266WiFi/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266httpUpdate/src"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/.piolibdeps/ArduinoJson_ID64"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/SimpleTimer"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266mDNS"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/WifiManager"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/MqttManager"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/LED"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/UpdateManager"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ArduinoOTA"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/Relay"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/Button"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/src"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/Button"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/LED"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/MqttManager"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/Relay"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/SimpleTimer"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/ToggleSwitch"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/UpdateManager"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/WifiManager"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/PowManager"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/lib/TimeWatchdog"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/.piolibdeps/ArduinoJson_ID64"
INCLUDEPATH += "$${HOMEDIR}/repos/sonoff-danimtb/.piolibdeps/ArduinoJson_ID64"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ArduinoOTA"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/DNSServer/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/EEPROM"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266AVRISP/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266HTTPClient/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266HTTPUpdateServer/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266SSDP"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266WebServer/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266WiFi/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266WiFiMesh/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266httpUpdate/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266mDNS"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/Ethernet/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/GDBStub/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/Hash/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/SD/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/SPI"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/Servo/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/SoftwareSerial"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/TFT_Touch_Shield_V2"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/Ticker"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/Wire"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/esp8266/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/toolchain-xtensa/xtensa-lx106-elf/include"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/toolchain-xtensa/lib/gcc/xtensa-lx106-elf/4.8.2/include"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/toolchain-xtensa/lib/gcc/xtensa-lx106-elf/4.8.2/include-fixed"

DEFINES += "ARDUINO=20300"
DEFINES += "LWIP_OPEN_SRC"
DEFINES += "F_CPU=80000000L"
DEFINES += "__ets__"
DEFINES += "ICACHE_FLASH"
DEFINES += "PLATFORMIO=30201"
DEFINES += "ESP8266"
DEFINES += "ARDUINO_ARCH_ESP8266"
DEFINES += "ARDUINO_ESP8266_ESP01"

OTHER_FILES += platformio.ini

SOURCES += src/main.cpp \
           lib/Button/Button.cpp \
           lib/DataManager/DataManager.cpp \
           lib/LED/LED.cpp \
           lib/MqttManager/MqttDiscoveryComponent.cpp \
           lib/MqttManager/MqttDiscoveryLight.cpp \
           lib/MqttManager/MqttDiscoverySensor.cpp \
           lib/MqttManager/MqttManager.cpp \
           lib/PowManager/PowManager.cpp \
           lib/Relay/Relay.cpp \
           lib/SimpleTimer/SimpleTimer.cpp \
           lib/ToggleSwitch/ToggleSwitch.cpp \
           lib/TimeWatchdog/TimeWatchdog.cpp \
           lib/UpdateManager/UpdateManager.cpp \
           lib/WebServer/WebServer.cpp \
           lib/WifiManager/WifiManager.cpp

DISTFILES += \
    lib/readme.txt

HEADERS += \
           lib/Button/Button.h \
           lib/DataManager/DataManager.h \
           lib/LED/LED.h \
           lib/MqttManager/MqttDiscoveryComponent.h \
           lib/MqttManager/MqttDiscoveryLight.h \
           lib/MqttManager/MqttDiscoverySensor.h \
           lib/MqttManager/MqttManager.h \
           lib/PowManager/PowManager.h \
           lib/Relay/Relay.h \
           lib/SimpleTimer/SimpleTimer.h \
           lib/ToggleSwitch/ToggleSwitch.h \
           lib/TimeWatchdog/TimeWatchdog.h \
           lib/UpdateManager/UpdateManager.h \
           lib/WebServer/WebServer.h \
           lib/WifiManager/WifiManager.h
