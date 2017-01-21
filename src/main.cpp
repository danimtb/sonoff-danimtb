#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

#include "Relay.h"
#include "LED.h"

#include "../info/DeviceInfo.h"
#include "../info/NetworkInfo.h"

#define DEVICE "sonoff-touch"
#define FW_VERSION "0.0.1"

IPAddress ip(192, 168, 1, IP_NUMBER);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

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
    Serial.println(SSID);
    WiFi.config(ip, gateway, subnet);
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
}

void connectWifi()
{
  Serial.println("Connecting...");
  WiFi.begin(SSID, PASS);
  delay(1000);
}

void checkConnectivity()
{
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
            if (client.connect(NAME, MQTT_USERNAME, MQTT_PASSWORD))
            {
                Serial.println("MQTT connected!");

                connected = true;

                client.subscribe(setTopic);
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
    //Configure pins
    relay.setup(RELAY_PIN, RELAY_HIGH_LVL);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    //Init serial comm
    Serial.begin(115200);

    //Print Device Info
    Serial.println("Initializing device...");
    Serial.println("Device Info:");
    Serial.print("\tDevice: ");
    Serial.println(DEVICE);
    Serial.print("FW Version: ");
    Serial.println(FW_VERSION);
    Serial.println();
    Serial.print("Name: ");
    Serial.println(NAME);
    Serial.print("IP: ");
    Serial.println(IP_NUMBER);
    Serial.print("MQTT Status Topic: ");
    Serial.println(STATUS_TOPIC);
    Serial.print("MQTT Set Topic: ");
    Serial.println(SET_TOPIC);
    Serial.print("MQTT Secondary Topic: ");
    Serial.println(SECONDARY_TOPIC);

    //OTA setup
    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.setHostname(NAME);
    //ArduinoOTA.setPassword(OTA_PASS);
    ArduinoOTA.begin();

    //Enable interrupt for button press
    Serial.println("Enabling switch interrupt");
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonChangeCallback, CHANGE);

    //Init wifi
    initWifi();

    //Connect to Wifi and MQTT server.
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(MQTTcallback);
    checkConnectivity();
    lastConnectivityCheck = millis();
}

void loop()
{

    if (millis() - lastConnectivityCheck >= 5000)
    {
        checkConnectivity();
        lastConnectivityCheck = millis();
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

        client.publish(statusTopic, "TOGGLE");
      }
      longPressStatus = false;
    }

    delay(50);
}
