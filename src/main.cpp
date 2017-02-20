#include <string>

#include <ArduinoOTA.h>

#include "../lib/Relay/Relay.h"
#include "../lib/LED/LED.h"
#include "../lib/Button/Button.h"
#include "../lib/WifiMQTTManager/WifiMQTTManager.h"


//#################### FW DATA ####################

#define FW "sonoff-danimtb"
#define FW_VERSION "0.0.5"

//#################### ======= ####################



//################## NETWORK DATA #################

#include "../data/NetworkData.h"

//################## ============ #################



//################## DEVICE DATA ##################

#include "../data/DeviceData.h"

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

WifiMQTTManager wifiMQTTManager;
Relay relay;
Button button;
LED led;

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

void shortPress()
{
    Serial.println("button.shortPress()");
    relay.commute();

    if (wifiMQTTManager.connected())
    {
        wifiMQTTManager.publishMQTT(statusTopic, relay.getState() ? "ON" : "OFF");
    }
}

void longPress()
{
    Serial.println("button.longPress()");

    if (wifiMQTTManager.connected())
    {
        Serial.println("Secondary topic: TOGGLE");
        wifiMQTTManager.publishMQTT(secondaryTopic, "TOGGLE");
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

    // Configure MQTT server
    wifiMQTTManager.setup(WIFI_SSID, WIFI_PASS, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD, DEVICE_NAME, IP_NUMBER, DEVICE_TYPE, FW, FW_VERSION);
    wifiMQTTManager.addStatusTopic(statusTopic);
    wifiMQTTManager.addSubscribeTopic(setTopic);
    wifiMQTTManager.setCallback(MQTTcallback);
}

void loop()
{
    // Process Button events
    button.loop();

    // Process Wifi and MQTT events
    wifiMQTTManager.loop();

    // Handle OTA FW updates
    ArduinoOTA.handle();

#ifdef LED_PIN
    // LED Status
    if (wifiMQTTManager.connected())
    {
        led.on();
    }
    else
    {
        led.off();
    }
#endif
}
