#ifndef WIFIMQTTMANAGER_H
#define WIFIMQTTMANAGER_H

// Requires MQTT_USERNAME, MQTT_PASSWORD
#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "SimpleTimer.h"

#undef min
#undef max

#include <map>
#include <string>
#include <vector>



#define WIFI_SSID "Trotacaminos2"
#define WIFI_PASS "A847184718"

#define MQTT_SERVER "192.168.1.101"
#define MQTT_PORT 8883
#define MQTT_USERNAME "mosquitto"
#define MQTT_PASSWORD "mosquitto"

#define OTA_PORT 8266
#define OTA_PASS "789456123"

#define DEVICE_NAME "touch3"
#define DEVICE_TYPE "sonoff-touch-esp01"

#define IP_NUMBER 113

#define SET_TOPIC "home/daniel-bedroom/light-main/set"
#define STATUS_TOPIC "home/daniel-bedroom/light-main"
#define SECONDARY_TOPIC "home/daniel-bedroom/light-corridor/set"



class WifiMQTTManager
{
private:
	std::string m_wifiSSID;
	std::string m_wifiPASS;
	std::string m_mqttServer;
    uint16_t m_mqttPort;
	std::string m_mqttUsername;
	std::string m_mqttPassword;
    uint16_t m_ipNumber;

	IPAddress* m_ip;
	IPAddress* m_gateway;
	IPAddress* m_subnet;

	std::string m_deviceName;
	std::string m_deviceType;
	std::string m_fw;
	std::string m_fwVersion;

	std::string m_deviceIpTopic;
	std::string m_deviceNameTopic;
	std::string m_deviceTypeTopic;
	std::string m_fwTopic;
	std::string m_fwVersionTopic;

	std::map<std::string, std::string> m_statusTopics;
	std::vector<std::string> m_subscribeTopics;

	WiFiClient m_wifiClient;
	PubSubClient* m_pubSubClient;

	bool m_connected;
	std::string m_tempTopic;
	std::string m_tempPayload;
	bool m_publishMQTT;

    unsigned long m_deviceStatusInfoTime;
	SimpleTimer m_deviceStatusInfoTimer;

    unsigned long m_checkConnectivityTime;
    SimpleTimer m_checkConnectivityTimer;

	void initWifi();
	void connectWifi();
	void publishDeviceStatusInfo();
	void checkConnectivity();
	void refreshStatusTopics();

public:
    void setup(std::string wifiSSID, std::string wifiPASS, std::string mqttServer, uint16_t mqttPort, std::string mqttUsername, std::string mqttPassword,
                         std::string deviceName, uint16_t ipNumber, std::string deviceType, std::string fw, std::string fwVersion);

	~WifiMQTTManager();

	void setCallback(void (*callback)(char*, uint8_t*, unsigned int));

	void addSubscribeTopic(std::string subscribeTopic);
	void eraseSubscribeTopic(std::string statusTopic);

	void addStatusTopic(std::string statusTopic);
	void eraseStatusTopic(std::string statusTopic);

	void publishMQTT(std::string topic, std::string payload);

	bool connected();

	void loop();

};

#endif
