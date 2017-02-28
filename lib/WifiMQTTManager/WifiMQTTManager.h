#ifndef WIFIMQTTMANAGER_H
#define WIFIMQTTMANAGER_H

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

class WifiMQTTManager
{
private:
    std::string m_wifiSSID;
    std::string m_wifiPASS;
    std::string m_mqttServer;
    uint16_t m_mqttPort;
    std::string m_mqttUsername;
    std::string m_mqttPassword;

    IPAddress m_ip;
    IPAddress m_mask;
    IPAddress m_gateway;

    std::string m_deviceName;
    std::string m_deviceType;
    std::string m_deviceMac;
    std::string m_fw;
    std::string m_fwVersion;

    std::string m_deviceMacTopic;
    std::string m_deviceIpTopic;
    std::string m_deviceNameTopic;
    std::string m_deviceTypeTopic;
    std::string m_fwTopic;
    std::string m_fwVersionTopic;

    std::map<std::string, std::string> m_statusTopics;
    std::vector<std::string> m_subscribeTopics;
    std::vector<std::pair<std::string, std::string>> m_tempPublishTopics;

    WiFiClient m_wifiClient;
    PubSubClient* m_pubSubClient;

    bool m_connected;
    bool m_publishMQTT;

    SimpleTimer m_deviceStatusInfoTimer;
    SimpleTimer m_checkConnectivityTimer;

    void initWifi();
    void connectWifi();
    std::string getMacAddress();
    void publishDeviceStatusInfo();
    void checkConnectivity();
    void setDeviceMac();
    void checkMQTTConnectivity();
    void refreshStatusTopics();

public:
    WifiMQTTManager();

    void setup(std::string wifiSSID, std::string wifiPASS, std::string mqttServer, uint16_t mqttPort,
               std::string mqttUsername, std::string mqttPassword, std::string ip, std::string mask, std::string gateway,
               std::string deviceName, std::string deviceType, std::string fw, std::string fwVersion);

    ~WifiMQTTManager();

    void setCallback(void (*callback)(char*, uint8_t*, unsigned int));
    void setCheckConnectivityTime(unsigned long checkConnectivityTime);
    void setDeviceStatusInfoTime(unsigned long deviceStatusInfoTime);

    void addSubscribeTopic(std::string subscribeTopic);
    void eraseSubscribeTopic(std::string statusTopic);

    void addStatusTopic(std::string statusTopic);
    void eraseStatusTopic(std::string statusTopic);

    void startConnection();
    void stopConnection();

    void publishMQTT(std::string topic, std::string payload);

    bool connected();

    void loop();
};

#endif
