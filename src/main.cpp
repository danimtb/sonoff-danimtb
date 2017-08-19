#include <string>
#include <map>
#include <cstdint>

#include <ArduinoOTA.h>

#include "../lib/Relay/Relay.h"
#include "../lib/LED/LED.h"
#include "../lib/Button/Button.h"
#include "../lib/DataManager/DataManager.h"
#include "../lib/MqttManager/MqttManager.h"
#include "../lib/WifiManager/WifiManager.h"
#include "../lib/WebServer/WebServer.h"
#include "../lib/UpdateManager/UpdateManager.h"
#include "../lib/TimeWatchdog/TimeWatchdog.h"

#ifdef ENABLE_SONOFF_POW
    #include "ArduinoJson.h"

    #include "../lib/PowManager/PowManager.h"
#endif

#ifdef ENABLE_SONOFF_SWITCH
    #include "../lib/ToggleSwitch/ToggleSwitch.h"
#endif


//#################### FIRMWARE ####################

#define FIRMWARE "sonoff-danimtb"
#define FIRMWARE_VERSION "0.3.0"

//#################### ======= ####################


//################## HARDWARE ##################

#ifdef ENABLE_SONOFF_POW
#define HARDWARE "sonoff-pow"
#define BUTTON_PIN 0
#define RELAY_PIN 12
#define LED_PIN 15
#define LED_MODE LED_HIGH_LVL
#endif

#ifdef ENABLE_SONOFF_TOUCH_ESP01
#define HARDWARE "sonoff-touch-esp01"
#define BUTTON_PIN 0
#define RELAY_PIN 2
#endif

#ifdef ENABLE_SONOFF_TOUCH
#define HARDWARE "sonoff-touch"
#define BUTTON_PIN 0
#define RELAY_PIN 12
#define LED_PIN 13
#define LED_MODE LED_LOW_LVL
#endif

#ifdef ENABLE_SONOFF_S20
#define HARDWARE "sonoff-s20"
#define BUTTON_PIN 0
#define RELAY_PIN 12
#define LED_PIN 13
#define LED_MODE LED_LOW_LVL
#endif

#ifdef ENABLE_SONOFF_SWITCH
#define HARDWARE "sonoff-switch"
#define BUTTON_PIN 0
#define RELAY_PIN 12
#define LED_PIN 13
#define LED_MODE LED_LOW_LVL
#define SWITCH_PIN 14
#endif

#ifdef ENABLE_SONOFF_BUTTON
#define HARDWARE "sonoff-button"
#define BUTTON_PIN 0
#define RELAY_PIN 12
#define LED_PIN 13
#define LED_MODE LED_LOW_LVL
#define EXTERNAL_BUTTON_PIN 14
#endif

#ifdef ENABLE_SONOFF
#define HARDWARE "sonoff"
#define BUTTON_PIN 0
#define RELAY_PIN 12
#define LED_PIN 13
#define LED_MODE LED_LOW_LVL
#endif

//################## ============ ##################



#ifdef ENABLE_SONOFF_POW
    TimeWatchdog powWatchdog;
    PowManager powManager;
#endif

#ifdef ENABLE_SONOFF_SWITCH
    ToggleSwitch toggleSwitch;
#endif

#ifdef ENABLE_SONOFF_BUTTON
    Button externalButton;
#endif

UpdateManager updateManager;
DataManager dataManager;
WifiManager wifiManager;
MqttManager mqttManager;
Relay relay;
Button button;
LED led;
TimeWatchdog connectionWatchdog;

String wifi_ssid = dataManager.get("wifi_ssid");
String wifi_password = dataManager.get("wifi_password");
String ip = dataManager.get("ip");
String mask = dataManager.get("mask");
String gateway = dataManager.get("gateway");
String ota_server = dataManager.get("ota_server");
String mqtt_server = dataManager.get("mqtt_server");
String mqtt_port = dataManager.get("mqtt_port");
String mqtt_username = dataManager.get("mqtt_username");
String mqtt_password = dataManager.get("mqtt_password");
String device_name = dataManager.get("device_name");
String mqtt_status = dataManager.get("mqtt_status");
String mqtt_command = dataManager.get("mqtt_command");
String mqtt_secondary = dataManager.get("mqtt_secondary");


#ifdef ENABLE_SONOFF_POW
    String mqtt_pow = mqtt_status + "/pow";

    StaticJsonBuffer<500> powJsonBuffer;
    JsonObject& powJsonObject = powJsonBuffer.createObject();
    String powJsonString;


    void powWatchdogCallback()
    {
        powJsonObject["current"] = powManager.getCurrent();
        powJsonObject["voltage"] = powManager.getVoltage();
        powJsonObject["activePower"] = powManager.getActivePower();
        powJsonObject["apparentPower"] = powManager.getApparentPower();
        powJsonObject["powerFactor"] = powManager.getPowerFactor();
        powJsonObject["reactivePower"] = powManager.getReactivePower();

        powJsonObject.printTo(powJsonString);

        mqttManager.publishMQTT(mqtt_pow, powJsonString.c_str());

        powWatchdog.feed();
    }
#endif

std::vector<std::pair<String, String>> getWebServerData()
{
    std::vector<std::pair<String, String>> webServerData;

    std::pair<String, String> generic_pair;

    generic_pair.first = "wifi_ssid";
    generic_pair.second = wifi_ssid;
    webServerData.push_back(generic_pair);

    generic_pair.first = "wifi_password";
    generic_pair.second = wifi_password;
    webServerData.push_back(generic_pair);

    generic_pair.first = "ip";
    generic_pair.second = ip;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mask";
    generic_pair.second = mask;
    webServerData.push_back(generic_pair);

    generic_pair.first = "gateway";
    generic_pair.second = gateway;
    webServerData.push_back(generic_pair);

    generic_pair.first = "ota_server";
    generic_pair.second = ota_server;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mqtt_server";
    generic_pair.second = mqtt_server;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mqtt_port";
    generic_pair.second = mqtt_port;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mqtt_username";
    generic_pair.second = mqtt_username;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mqtt_password";
    generic_pair.second = mqtt_password;
    webServerData.push_back(generic_pair);

    generic_pair.first = "device_name";
    generic_pair.second = device_name;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mqtt_status";
    generic_pair.second = mqtt_status;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mqtt_command";
    generic_pair.second = mqtt_command;
    webServerData.push_back(generic_pair);

    generic_pair.first = "mqtt_secondary";
    generic_pair.second = mqtt_secondary;
    webServerData.push_back(generic_pair);

    generic_pair.first = "firmware_version";
    generic_pair.second = FIRMWARE_VERSION;
    webServerData.push_back(generic_pair);

    generic_pair.first = "hardware";
    generic_pair.second = HARDWARE;
    webServerData.push_back(generic_pair);

    return webServerData;
}

void webServerSubmitCallback(std::map<String, String> inputFieldsContent)
{
    //Save config to dataManager
    Serial.println("webServerSubmitCallback()");

    dataManager.set("wifi_ssid", inputFieldsContent["wifi_ssid"]);
    dataManager.set("wifi_password", inputFieldsContent["wifi_password"]);
    dataManager.set("ip", inputFieldsContent["ip"]);
    dataManager.set("mask", inputFieldsContent["mask"]);
    dataManager.set("gateway", inputFieldsContent["gateway"]);
    dataManager.set("ota_server", inputFieldsContent["ota_server"]);
    dataManager.set("mqtt_server", inputFieldsContent["mqtt_server"]);
    dataManager.set("mqtt_port", inputFieldsContent["mqtt_port"]);
    dataManager.set("mqtt_username", inputFieldsContent["mqtt_username"]);
    dataManager.set("mqtt_password", inputFieldsContent["mqtt_password"]);
    dataManager.set("device_name", inputFieldsContent["device_name"]);
    dataManager.set("mqtt_status", inputFieldsContent["mqtt_status"]);
    dataManager.set("mqtt_command", inputFieldsContent["mqtt_command"]);
    dataManager.set("mqtt_secondary", inputFieldsContent["mqtt_secondary"]);

    ESP.restart(); // Restart device with new config
}

void MQTTcallback(String topicString, String payloadString)
{
    if (topicString == mqtt_command)
    {
        if (payloadString == "ON")
        {
            Serial.println("ON");
            relay.on();
            mqttManager.publishMQTT(mqtt_status, "ON");
        }
        else if (payloadString == "OFF")
        {
            Serial.println("OFF");
            relay.off();
            mqttManager.publishMQTT(mqtt_status, "OFF");
        }
        else if (payloadString == "TOGGLE")
        {
            Serial.println("TOGGLE");
            relay.commute();
            mqttManager.publishMQTT(mqtt_status, relay.getState() ? "ON" : "OFF");
        }
        else
        {
            Serial.print("MQTT payload unknown: ");
            Serial.println(payloadString.c_str());
        }
    }
    else
    {
        Serial.print("MQTT topic unknown:");
        Serial.println(topicString.c_str());
    }
}

void shortPress()
{
    Serial.println("button.shortPress()");
    relay.commute();

    mqttManager.publishMQTT(mqtt_status, relay.getState() ? "ON" : "OFF");
    mqttManager.publishMQTT(mqtt_command, relay.getState() ? "ON" : "OFF");
}

void longPress()
{
    Serial.println("button.longPress()");

    Serial.println("Secondary topic: TOGGLE");
    mqttManager.publishMQTT(mqtt_secondary, "TOGGLE");
}

void veryLongPress()
{
    //Disconnect and Restart device
    mqttManager.stopConnection();
    wifiManager.disconnectStaWifi();
    ESP.restart();
}

void ultraLongPress()
{
    Serial.println("button.longlongPress()");

    if(wifiManager.apModeEnabled())
    {
        WebServer::getInstance().stop();
        wifiManager.destroyApWifi();

        ESP.restart();
    }
    else
    {
        mqttManager.stopConnection();
        wifiManager.createApWifi();
        WebServer::getInstance().start();
    }
}

void connectionWatchdogCallback()
{
    ESP.restart();
}


void setup()
{
    // Init serial comm
    Serial.begin(115200);

    // Configure Relay
    relay.setup(RELAY_PIN, RELAY_HIGH_LVL);
    relay.off();

    // Configure Button
    button.setup(BUTTON_PIN, ButtonType::PULLUP);
    button.setShortPressCallback(shortPress);
    button.setLongPressCallback(longPress);
    button.setVeryLongPressCallback(veryLongPress);
    button.setUltraLongPressCallback(ultraLongPress);

    // Configure external button
    #ifdef ENABLE_SONOFF_BUTTON
        externalButton.setup(EXTERNAL_BUTTON_PIN, ButtonType::PULLUP_INTERNAL);
        externalButton.setShortPressCallback(shortPress);
        externalButton.setLongPressCallback(longPress);
        externalButton.setVeryLongPressCallback(veryLongPress);
        externalButton.setUltraLongPressCallback(ultraLongPress);
    #endif

    #ifdef LED_PIN
        led.setup(LED_PIN, LED_MODE);
        led.on();
        delay(300);
        led.off();
    #endif

    #ifdef ENABLE_SONOFF_POW
        powManager.setup();
        powWatchdog.setup(80000, powWatchdogCallback);
        powWatchdog.init();
        powWatchdog.feed();
    #endif

    #ifdef ENABLE_SONOFF_SWITCH
        toggleSwitch.setup(SWITCH_PIN, ToggleSwitchType::PULLUP_INTERNAL);
        toggleSwitch.setSingleToggleCallback(shortPress);
        toggleSwitch.setDoubleToggleCallback(longPress);
        toggleSwitch.setTripleToggleCallback(veryLongPress);
        toggleSwitch.setQuadrupleToggleCallback(ultraLongPress);
    #endif

    // Configure Wifi
    wifiManager.setup(wifi_ssid, wifi_password, ip, mask, gateway, HARDWARE);
    wifiManager.connectStaWifi();

    // Configure MQTT
    mqttManager.setCallback(MQTTcallback);
    mqttManager.setup(mqtt_server, mqtt_port.c_str(), mqtt_username, mqtt_password);
    mqttManager.setLastWillMQTT(mqtt_status, "OFF");
    mqttManager.setDeviceData(device_name, HARDWARE, ip, FIRMWARE, FIRMWARE_VERSION);
    mqttManager.addStatusTopic(mqtt_status);
    mqttManager.addSubscribeTopic(mqtt_command);
    mqttManager.startConnection();

    //Configure WebServer
    WebServer::getInstance().setup("/index.html.gz", webServerSubmitCallback);
    WebServer::getInstance().setData(getWebServerData());

    // OTA setup
    ArduinoOTA.setHostname(device_name.c_str());
    ArduinoOTA.begin();

    // UpdateManager setup
    updateManager.setup(ota_server, FIRMWARE, FIRMWARE_VERSION, HARDWARE);

    // ConnectionWatchdog setup
    connectionWatchdog.setup(1200000, connectionWatchdogCallback); //20 min
}

void loop()
{
    // Process Button events
    button.loop();

    // Process External Button events
    #ifdef ENABLE_SONOFF_BUTTON
        externalButton.loop();
    #endif

    // Process ToggleSwitch events
    #ifdef ENABLE_SONOFF_SWITCH
        toggleSwitch.loop();
    #endif

    // Check Wifi status
    wifiManager.loop();

    // Check MQTT status and OTA Updates
    if (wifiManager.connected())
    {
        mqttManager.loop();
        updateManager.loop();
        ArduinoOTA.handle();
    }

    // Handle WebServer connections
    if (wifiManager.apModeEnabled())
    {
        WebServer::getInstance().loop();
    }

    // LED Status and ConnectionWatchdog
    connectionWatchdog.loop();

    if (mqttManager.connected())
    {
        connectionWatchdog.init();
        connectionWatchdog.feed();

        #ifdef LED_PIN
            led.on();
        #endif
    }
    else if(wifiManager.apModeEnabled())
    {
        connectionWatchdog.deinit();

        #ifdef LED_PIN
            led.blink(1000);
        #endif
    }
    else
    {
        #ifdef LED_PIN
            led.off();
        #endif
    }

    // Pow Status
    #ifdef ENABLE_SONOFF_POW
        powManager.loop();
        powWatchdog.loop();
    #endif
}
