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

#ifdef ENABLE_SONOFF_POW
    #include "ArduinoJson.h"

    #include "../lib/SimpleTimer/SimpleTimer.h"
    #include "../lib/PowManager/PowManager.h"
#endif



//#################### FW DATA ####################

#define FIRMWARE "sonoff-danimtb"
#define FIRMWARE_VERSION "0.0.9"

//#################### ======= ####################


//################## DEVICE TYPE ##################

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

#ifdef ENABLE_SONOFF
#define HARDWARE "sonoff"
#define BUTTON_PIN 0
#define RELAY_PIN 12
#define LED_PIN 13
#define LED_MODE LED_LOW_LVL
#endif

//################## ============ ##################

#ifdef ENABLE_SONOFF_POW
    SimpleTimer powTimer;
    PowManager powManager;
#endif

UpdateManager updateManager;
DataManager dataManager;
WifiManager wifiManager;
MqttManager mqttManager;
Relay relay;
Button button;
LED led;

std::string wifi_ssid = dataManager.getWifiSSID();
std::string wifi_password = dataManager.getWifiPass();
std::string ip = dataManager.getIP();
std::string mask = dataManager.getMask();
std::string gateway = dataManager.getGateway();
std::string ota = dataManager.getOta();
std::string mqtt_server = dataManager.getMqttServer();
std::string mqtt_port = dataManager.getMqttPort();
std::string mqtt_username = dataManager.getMqttUser();
std::string mqtt_password = dataManager.getMqttPass();
std::string device_name = dataManager.getDeviceName();
std::string mqtt_status = dataManager.getMqttTopic(0);
std::string mqtt_command = dataManager.getMqttTopic(1);
std::string mqtt_secondary = dataManager.getMqttTopic(2);


#ifdef ENABLE_SONOFF_POW
    std::string mqtt_pow = mqtt_status + "/pow";

    void powHandlePublish()
    {
        if (powTimer.check())
        {
            StaticJsonBuffer<500> powJsonBuffer;
            JsonObject& powJsonObject = powJsonBuffer.createObject();
            String powJsonString;

            powJsonObject["current"] = powManager.getCurrent();
            powJsonObject["voltage"] = powManager.getVoltage();
            powJsonObject["activePower"] = powManager.getActivePower();
            powJsonObject["apparentPower"] = powManager.getApparentPower();
            powJsonObject["powerFactor"] = powManager.getPowerFactor();
            powJsonObject["reactivePower"] = powManager.getReactivePower();

            powJsonObject.printTo(powJsonString);

            mqttManager.publishMQTT(mqtt_pow, std::string(powJsonString.c_str()));

            powTimer.start();
        }
    }
#endif

std::vector<std::pair<std::string, std::string>> getWebServerData()
{
    std::vector<std::pair<std::string, std::string>> webServerData;

    std::pair<std::string, std::string> generic_pair;

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
    generic_pair.second = ota;
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

void webServerSubmitCallback(std::map<std::string, std::string> inputFieldsContent)
{
    //Save config to dataManager
    Serial.println("webServerSubmitCallback");

    dataManager.setWifiSSID(inputFieldsContent["wifi_ssid"]);
    dataManager.setWifiPass(inputFieldsContent["wifi_password"]);
    dataManager.setIP(inputFieldsContent["ip"]);
    dataManager.setMask(inputFieldsContent["mask"]);
    dataManager.setGateway(inputFieldsContent["gateway"]);
    dataManager.setOta(inputFieldsContent["ota_server"]);
    dataManager.setMqttServer(inputFieldsContent["mqtt_server"]);
    dataManager.setMqttPort(inputFieldsContent["mqtt_port"]);
    dataManager.setMqttUser(inputFieldsContent["mqtt_username"]);
    dataManager.setMqttPass(inputFieldsContent["mqtt_password"]);
    dataManager.setDeviceName(inputFieldsContent["device_name"]);
    dataManager.setMqttTopic(0, inputFieldsContent["mqtt_status"]);
    dataManager.setMqttTopic(1, inputFieldsContent["mqtt_command"]);
    dataManager.setMqttTopic(2, inputFieldsContent["mqtt_secondary"]);

    ESP.restart(); // Restart device with new config
}

void MQTTcallback(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t length, size_t index, size_t total)
{
    Serial.print("Message arrived from topic [");
    Serial.print(topic);
    Serial.println("] ");

    //ALWAYS DO THIS: Set end of payload string to length
    payload[length] = '\0'; //Do not delete

    std::string topicString(topic);
    std::string payloadString((char *)payload);

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
}

void longPress()
{
    Serial.println("button.longPress()");

    Serial.println("Secondary topic: TOGGLE");
    mqttManager.publishMQTT(mqtt_secondary, "TOGGLE");
}

void longlongPress()
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

void setup()
{
    // Init serial comm
    Serial.begin(115200);

    // Configure Relay
    relay.setup(RELAY_PIN, RELAY_HIGH_LVL);
    relay.off();

    // Configure Button
    button.setup(BUTTON_PIN, PULLDOWN);
    button.setShortPressCallback(shortPress);
    button.setLongPressCallback(longPress);
    button.setLongLongPressCallback(longlongPress);

    #ifdef LED_PIN
        led.setup(LED_PIN, LED_MODE);
        led.on();
        delay(300);
        led.off();
    #endif

    #ifdef ENABLE_SONOFF_POW
        powManager.setup();
        powTimer.setup(RT_ON, 20000);
    #endif

    // Configure Wifi
    wifiManager.setup(wifi_ssid, wifi_password, ip, mask, gateway, HARDWARE);
    wifiManager.connectStaWifi();

    // Configure MQTT
    mqttManager.setup(mqtt_server, mqtt_port.c_str(), mqtt_username, mqtt_password);
    mqttManager.setDeviceData(device_name, HARDWARE, ip, FIRMWARE, FIRMWARE_VERSION);
    mqttManager.setLastWillMQTT(mqtt_status, "OFF");
    mqttManager.addStatusTopic(mqtt_status);
    mqttManager.addSubscribeTopic(mqtt_command);
    mqttManager.setCallback(MQTTcallback);
    mqttManager.startConnection();

    //Configure WebServer
    WebServer::getInstance().setup("/index.html.gz", webServerSubmitCallback);
    WebServer::getInstance().setData(getWebServerData());

    // OTA setup
    ArduinoOTA.setHostname(device_name.c_str());
    ArduinoOTA.begin();

    // UpdateManager setup
    updateManager.setup(ota, FIRMWARE, FIRMWARE_VERSION, HARDWARE);
}

void loop()
{
    // Process Button events
    button.loop();

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
    if(wifiManager.apModeEnabled())
    {
        WebServer::getInstance().loop();
    }

    // LED Status
    #ifdef LED_PIN
        if (mqttManager.connected())
        {
            led.on();
        }
        else if(wifiManager.apModeEnabled())
        {
            led.blink(1000);
        }
        else
        {
            led.off();
        }
    #endif

    // Pow Status
    #ifdef ENABLE_SONOFF_POW
        powManager.loop();
        powHandlePublish();
    #endif
}
