#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "SimpleTimer.h"

#undef min
#undef max

#include <map>
#include <string>
#include <vector>

class MqttManager
{
private:
    std::string m_mqttServer;
    uint16_t m_mqttPort;
    std::string m_mqttUsername;
    std::string m_mqttPassword;

    std::string m_deviceName;
    std::string m_deviceType;
    std::string m_deviceIP;
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

    void setDeviceMac();
    void publishDeviceStatusInfo();
    void checkConnectivity();
    void refreshStatusTopics();

public:
    MqttManager();
    ~MqttManager();

    void setup(std::string mqttServer, std::string mqttPort, std::string mqttUsername, std::string mqttPassword);

    void setDeviceData(std::string deviceName, std::string deviceType, std::string deviceIP, std::string fw, std::string fwVersion);
    void setCallback(void (*callback)(char*, uint8_t*, unsigned int));

    void setDeviceStatusInfoTime(unsigned long deviceStatusInfoTime);

    void addSubscribeTopic(std::string subscribeTopic);
    void clearSubscribeTopics();

    void addStatusTopic(std::string statusTopic);
    void clearStatusTopics();

    void startConnection();
    void stopConnection();

    void publishMQTT(std::string topic, std::string payload);

    bool connected();

    void loop();
};

#endif
