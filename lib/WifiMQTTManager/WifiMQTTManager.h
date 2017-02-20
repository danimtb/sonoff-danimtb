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
    String m_wifiSSID;
    String m_wifiPASS;
    String m_mqttServer;
    uint16_t m_mqttPort;
    String m_mqttUsername;
    String m_mqttPassword;

    IPAddress m_ip;
    IPAddress m_mask;
    IPAddress m_gateway;

    String m_deviceName;
    String m_deviceType;
    String m_deviceMac;
    String m_fw;
    String m_fwVersion;

    String m_deviceMacTopic;
    String m_deviceIpTopic;
    String m_deviceNameTopic;
    String m_deviceTypeTopic;
    String m_fwTopic;
    String m_fwVersionTopic;

    std::map<String, String> m_statusTopics;
    std::vector<String> m_subscribeTopics;
    std::vector<std::pair<String, String>> m_tempPublishTopics;

    WiFiClient m_wifiClient;
    PubSubClient* m_pubSubClient;

    bool m_connected;

    bool m_publishMQTT;

    unsigned long m_deviceStatusInfoTime;
    SimpleTimer m_deviceStatusInfoTimer;

    unsigned long m_checkConnectivityTime;
    SimpleTimer m_checkConnectivityTimer;

    void initWifi();
    void connectWifi();
    String getMacAddress();
    void publishDeviceStatusInfo();
    void checkConnectivity();
    void refreshStatusTopics();

public:
    WifiMQTTManager();

    void setup(String wifiSSID, String wifiPASS, String mqttServer, uint16_t mqttPort,
               String mqttUsername, String mqttPassword, String ip, String mask, String gateway,
               String deviceName, String deviceType, String fw, String fwVersion);

    ~WifiMQTTManager();

    void setCallback(void (*callback)(char*, uint8_t*, unsigned int));
    void setCheckConnectivityTime(unsigned long checkConnectivityTime);
    void setDeviceStatusInfoTime(unsigned long deviceStatusInfoTime);

    void addSubscribeTopic(String subscribeTopic);
    void eraseSubscribeTopic(String statusTopic);

    void addStatusTopic(String statusTopic);
    void eraseStatusTopic(String statusTopic);

    void startConnection();
    void stopConnection();

    void publishMQTT(String topic, String payload);

    bool connected();

    void loop();
};

#endif
