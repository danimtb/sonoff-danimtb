#include <string>
#include <map>

#include <ArduinoOTA.h>

#include "../lib/Relay/Relay.h"
#include "../lib/LED/LED.h"
#include "../lib/Button/Button.h"
#include "../lib/MqttManager/MqttManager.h"
#include "../lib/WifiManager/WifiManager.h"
#include "../lib/WebServer/WebServer.h"


//#################### FW DATA ####################

#define FW "sonoff-danimtb"
#define FW_VERSION "0.0.6"

//#################### ======= ####################



//################## NETWORK DATA #################

#include "../config/NetworkData.h"

//################## ============ #################



//################## DEVICE DATA ##################

#include "../config/DeviceData.h"

//################## ============ ##################



//################## DEVICE TYPE ##################

#ifdef ENABLE_SONOFF_TOUCH_ESP01
#define DEVICE_TYPE "sonoff-touch-esp01"
#define BUTTON_PIN 0
#define RELAY_PIN 2
#endif

#ifdef ENABLE_SONOFF_TOUCH
#define DEVICE_TYPE "sonoff-touch"
#define BUTTON_PIN 0
#define RELAY_PIN 12
#define LED_PIN 13
#endif

#ifdef ENABLE_SONOFF_S20
#define DEVICE_TYPE "sonoff-s20"
#define BUTTON_PIN 0
#define RELAY_PIN 12
#define LED_PIN 13
#endif

#ifdef ENABLE_SONOFF
#define DEVICE_TYPE "sonoff"
#define BUTTON_PIN 0
#define RELAY_PIN 12
#define LED_PIN 13
#endif

//################## ============ ##################



std::string setTopic = SET_TOPIC;
std::string statusTopic = STATUS_TOPIC;
std::string secondaryTopic = SECONDARY_TOPIC;

WifiManager wifiManager;
MqttManager mqttManager;
Relay relay;
Button button;
LED led;

void webServerSubmitCallback(std::map<std::string, std::string> inputFieldsContent)
{
    //Save config to dataManager
    Serial.println("webServerSubmitCallback");
    Serial.println(inputFieldsContent["wifi_password"].c_str());

    ESP.restart(); // Restart device with new config
}

void MQTTcallback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Message arrived from topic [");
    Serial.print(topic);
    Serial.println("] ");

    //ALWAYS DO THIS: Set end of payload string to length
    payload[length] = '\0'; //Do not delete

    if (!strcmp(topic, setTopic.c_str()))
    {
        if (!strcasecmp((char *)payload, "ON"))
        {
            Serial.println("ON");
            relay.on();
            mqttManager.publishMQTT(statusTopic, "ON");
        }
        else if (!strcasecmp((char *)payload, "OFF"))
        {
            Serial.println("OFF");
            relay.off();
            mqttManager.publishMQTT(statusTopic, "OFF");
        }
        else if (!strcasecmp((char *)payload, "TOGGLE"))
        {
            Serial.println("TOGGLE");
            relay.commute();
            mqttManager.publishMQTT(statusTopic, relay.getState() ? "ON" : "OFF");
        }
        else
        {
            Serial.print("MQTT payload unknown: ");
            Serial.println((char *)payload);
        }
    }
    else
    {
        Serial.print("MQTT topic unknown:");
        Serial.println(topic);
    }
}

void shortPress()
{
    Serial.println("button.shortPress()");
    relay.commute();

    if (mqttManager.connected())
    {
        mqttManager.publishMQTT(statusTopic, relay.getState() ? "ON" : "OFF");
    }
}

void longPress()
{
    Serial.println("button.longPress()");

    if (mqttManager.connected())
    {
        Serial.println("Secondary topic: TOGGLE");
        mqttManager.publishMQTT(secondaryTopic, "TOGGLE");
    }
}

void longlongPress()
{
    Serial.println("button.longlongPress()");

    if(wifiManager.apModeEnabled())
    {
        WebServer::getInstance().stop();
        wifiManager.destroyApWifi();

        ESP.restart(); // Restart device
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

    // Configure Button
    button.setup(BUTTON_PIN, PULLDOWN);
    button.setShortPressCallback(shortPress);
    button.setLongPressCallback(longPress);
    button.setLongLongPressCallback(longlongPress);

    #ifdef LED_PIN
        led.setup(LED_PIN, LED_LOW_LVL);
        led.on();
        delay(300);
        led.off();
    #endif

    // OTA setup
    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.setHostname(DEVICE_NAME);
    ArduinoOTA.setPassword(OTA_PASS);
    ArduinoOTA.begin();

    // Configure Wifi
    wifiManager.setup(WIFI_SSID, WIFI_PASS, DEVICE_IP, DEVICE_MASK, DEVICE_GATEWAY, DEVICE_NAME);
    wifiManager.connectStaWifi();

    // Configure MQTT
    mqttManager.setup(MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD);
    mqttManager.setDeviceData(DEVICE_NAME, DEVICE_TYPE, DEVICE_IP, FW, FW_VERSION);
    mqttManager.addStatusTopic(statusTopic);
    mqttManager.addSubscribeTopic(setTopic);
    mqttManager.setCallback(MQTTcallback);
    mqttManager.startConnection();

    //Configure WebServer
    WebServer::getInstance().setup("/index.html.gz", webServerSubmitCallback);
}

void loop()
{
    // Process Button events
    button.loop();

    // Check Wifi status
    wifiManager.loop();

    // Check MQTT status
    if (wifiManager.connected())
    {
        mqttManager.loop();
    }

    // Handle OTA FW updates
    ArduinoOTA.handle();

    // Handle WebServer connections
    if(wifiManager.apModeEnabled())
    {
        WebServer::getInstance().loop();
    }

    #ifdef LED_PIN
        // LED Status
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
}
