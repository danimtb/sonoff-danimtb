#include <string>

#include <ArduinoOTA.h>

#include "Relay.h"
#include "LED.h"
#include "Button.h"
#include "WifiMQTTManager.h"

//#################### FW DATA ####################

#define FW "sonoff-danimtb"
#define FW_VERSION "0.0.4"

//#################### ####### ####################


//################## DEVICE DATA ##################

#include "../info/DeviceInfo.h"
#include "../info/NetworkInfo.h"

// #define ENABLE_SONOFF_TOUCH_ESP01 for "sonoff-touch-esp01"
// #define ENABLE_SONOFF_TOUCH for "sonoff-touch"
// #define ENABLE_SONOFF_S20 for "sonoff-s20"
// #define ENABLE_SONOFF for "sonoff"

#ifdef DEVICE_TYPE_SONOFF_TOUCH_ESP01
    #define BUTTON_PIN 0
    #define RELAY_PIN 2
#endif

#ifdef DEVICE_TYPE_SONOFF_TOUCH
    #define BUTTON_PIN 0
    #define RELAY_PIN 12
    #define LED_PIN 13
#endif

#ifdef ENABLE_SONOFF_S20
    #define BUTTON_PIN 0
    #define RELAY_PIN 12
    #define LED_PIN 13
#endif

#ifdef ENABLE_SONOFF
    #define BUTTON_PIN 0
    #define RELAY_PIN 12
    #define LED_PIN 13
#endif

//################## ########### ##################

std::string setTopic = SET_TOPIC;
std::string statusTopic = STATUS_TOPIC;
std::string secondaryTopic = SECONDARY_TOPIC;

WifiMQTTManager wifiMQTTManager;
Relay relay;
Button button;

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
            wifiMQTTManager.publishMQTT(statusTopic, "ON");
        }
        else if (!strcasecmp((char *)payload, "OFF"))
        {
            Serial.println("OFF");
            relay.off();
            wifiMQTTManager.publishMQTT(statusTopic, "OFF");
        }
        else if (!strcasecmp((char *)payload, "TOGGLE"))
        {
            Serial.println("TOGGLE");
            relay.commute();
            wifiMQTTManager.publishMQTT(statusTopic, relay.getState() ? "ON" : "OFF");
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

void setup()
{
    // Init serial comm
    Serial.begin(115200);

    // Configure pins
    relay.setup(RELAY_PIN, RELAY_HIGH_LVL);
    button.setup(BUTTON_PIN, PULLDOWN);

    // OTA setup
    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.setHostname(DEVICE_NAME);
    ArduinoOTA.setPassword(OTA_PASS);
    ArduinoOTA.begin();

    // Configure MQTT server
    wifiMQTTManager.setup(WIFI_SSID, WIFI_PASS, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD, DEVICE_NAME, IP_NUMBER, DEVICE_TYPE, FW, FW_VERSION);
    wifiMQTTManager.addStatusTopic(statusTopic);
    wifiMQTTManager.addSubscribeTopic(setTopic);
    wifiMQTTManager.setCallback(MQTTcallback);
}

void loop()
{
    //Process Button events
    button.loop();

    //Process Wifi and MQTT events
    wifiMQTTManager.loop();

    //Handle OTA FW updates
    ArduinoOTA.handle();

    //Button press
    if (button.shortPress())
    {
        Serial.println("button.shortPress()");
        relay.commute();

        if (wifiMQTTManager.connected())
        {
            wifiMQTTManager.publishMQTT(statusTopic, relay.getState() ? "ON" : "OFF");
        }
    }
    if (button.longPress())
    {
        Serial.println("button.longPress()");

        if (wifiMQTTManager.connected())
        {
            Serial.println("Secondary topic: TOGGLE");
            wifiMQTTManager.publishMQTT(secondaryTopic, "TOGGLE");
        }
    }
}
