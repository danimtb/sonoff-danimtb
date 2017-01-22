#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

#include "Relay.h"
#include "LED.h"

#include "../info/DeviceInfo.h"
#include "../info/NetworkInfo.h"

#define FW "sonoff-touch-custom"
#define FW_VERSION "0.0.4"

IPAddress ip(192, 168, 1, IP_NUMBER);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

const char *deviceNameTopic = "/" DEVICE_NAME;
const char *deviceIpTopic = "/" DEVICE_NAME "/ip";
const char *deviceTypeTopic = "/" DEVICE_NAME "/type";
const char *fwTopic = "/" DEVICE_NAME "/fw";
const char *fwVersionTopic = "/" DEVICE_NAME "/fw/version";

const char *setTopic = SET_TOPIC;
const char *statusTopic = STATUS_TOPIC;
const char *secondaryTopic = SECONDARY_TOPIC;

bool connected = false;
bool desiredRelayState = false;
bool longPressStatus = false;

unsigned long millisSinceChange = 0;
unsigned long lastConnectivityCheck = 0;

#define BUTTON_PIN 0
#define RELAY_PIN 2

WiFiClient espClient;
PubSubClient client(espClient);

Relay relay;

void initWifi()
{
    Serial.println();
    Serial.print("Wifi SSID: ");
    Serial.println(WIFI_SSID);
    WiFi.config(ip, gateway, subnet);
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
}

void connectWifi()
{
  Serial.println("Connecting...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  delay(1000);
}

void publishDeviceInfo()
{
  Serial.println("Publishing device information...");

  char ip[3];
  sprintf(ip, "%d", IP_NUMBER);

  client.publish(deviceNameTopic, DEVICE_NAME);
  client.publish(deviceIpTopic, ip);
  client.publish(deviceTypeTopic, DEVICE_TYPE);
  client.publish(fwTopic, FW);
  client.publish(fwVersionTopic, FW_VERSION);

  Serial.println("Publishing device information done!");
}

void checkConnectivity()
{
    lastConnectivityCheck = millis();
    Serial.println("Checking connectivity...");

    if(WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Wifi not connected");
        connected = false;

        connectWifi();
    }
    else
    {
        Serial.println("Wifi connection OK");

        if (!client.connected())
        {
            if (client.connect(DEVICE_NAME, MQTT_USERNAME, MQTT_PASSWORD))
            {
                Serial.println("MQTT connected!");

                client.subscribe(setTopic);
                publishDeviceInfo();
                connected = true;
            }
            else
            {
                Serial.print("MQTT connection failed: ");
                Serial.println(client.state());
            }
        }
        else
        {
            Serial.println("MQTT connection OK");
            publishDeviceInfo();
            connected = true;
        }
    }
}

void MQTTcallback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Message arrived from topic [");
    Serial.print(topic);
    Serial.println("] ");

    //Set end of payload string to length
    payload[length] = '\0';

    if (!strcmp(topic, setTopic))
    {
        payload[length] = '\0';
        if (!strcasecmp((char *)payload, "ON"))
        {
            Serial.println("ON");
            desiredRelayState = true;
        }
        else if (!strcasecmp((char *)payload, "OFF"))
        {
            Serial.println("OFF");
            desiredRelayState = false;
        }
        else if (!strcasecmp((char *)payload, "TOGGLE"))
        {
            Serial.println("TOGGLE");
            desiredRelayState = !desiredRelayState;
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
  desiredRelayState = !desiredRelayState;
}

void longPress()
{
  longPressStatus = true;
}

void buttonChangeCallback()
{
    if (digitalRead(0) == 1)
    {
        //Button has been released, trigger one of the two possible options.

        if (millis() - millisSinceChange > 500)
        {
            longPress();
        }
        else if (millis() - millisSinceChange > 100)
        {
            //Short press
            shortPress();
        }
        else
        {
            //Too short to register as a press
        }
    }
    else
    {
        millisSinceChange = millis();
    }
}



void setup() {
    // Init serial comm
    Serial.begin(115200);

    // Print Device Info
    Serial.println("Initializing device...");
    Serial.println("Device Info:");
    Serial.print("\tFW: ");
    Serial.println(FW);
    Serial.print("\tFW Version: ");
    Serial.println(FW_VERSION);
    Serial.println();
    Serial.print("\tDevice type: ");
    Serial.println(DEVICE_TYPE);
    Serial.print("\tDevice name: ");
    Serial.println(DEVICE_NAME);
    Serial.print("\tIP: ");
    Serial.println(IP_NUMBER);
    Serial.print("\tMQTT Status Topic: ");
    Serial.println(STATUS_TOPIC);
    Serial.print("\tMQTT Set Topic: ");
    Serial.println(SET_TOPIC);
    Serial.print("\tMQTT Secondary Topic: ");
    Serial.println(SECONDARY_TOPIC);

    // Configure pins
    relay.setup(RELAY_PIN, RELAY_HIGH_LVL);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Enable interrupt for button press
    Serial.println("Enabling switch interrupt");
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonChangeCallback, CHANGE);

    // OTA setup
    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.setHostname(DEVICE_NAME);
    ArduinoOTA.setPassword(OTA_PASS);
    ArduinoOTA.begin();

    // Configure MQTT server.
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(MQTTcallback);

    // Init WIFI
    initWifi();

    // Connect to WIFI and MQTT
    checkConnectivity();
}

void loop()
{

    if (millis() - lastConnectivityCheck >= 5000)
    {
        checkConnectivity();
    }

    //Process MQTT client events
    client.loop();

    //Handle OTA FW updates.
    ArduinoOTA.handle();


    //Change relay state if needed
    if (desiredRelayState != relay.getState())
    {
      if(desiredRelayState)
      {
        relay.on();

        if (connected)
        {
          Serial.println("Status topic: ON");
          client.publish(statusTopic, "ON");
        }
      }
      else
      {
        relay.off();

        if (connected)
        {
          Serial.println("Status topic: OFF");
          client.publish(statusTopic, "OFF");
        }
      }
    }

    //Publish to secondary topic
    if (longPressStatus)
    {
      if (connected)
      {
        Serial.println("Secondary topic: TOGGLE");
        client.publish(secondaryTopic, "TOGGLE");
      }
      longPressStatus = false;
    }

    delay(50);
}
