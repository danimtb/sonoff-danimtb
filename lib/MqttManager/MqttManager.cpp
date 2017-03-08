#include "MqttManager.h"

MqttManager::MqttManager()
{
    m_connected = false;
    m_publishMQTT = false;

    m_deviceStatusInfoTimer.setup(RT_ON, 300000);
    m_checkConnectivityTimer.setup(RT_ON, 20000);
}

void MqttManager::setup(std::string mqttServer, std::string mqttPort, std::string mqttUsername, std::string mqttPassword)
{
    m_mqttServer = mqttServer;
    m_mqttPort = atoi(mqttPort.c_str());
    m_mqttUsername = mqttUsername;
    m_mqttPassword = mqttPassword;

    m_pubSubClient = new PubSubClient(m_wifiClient);
    m_pubSubClient->setServer(mqttServer.c_str(), m_mqttPort);

    m_deviceStatusInfoTimer.start();
    m_checkConnectivityTimer.start();
}

void MqttManager::setDeviceData(std::string deviceName, std::string deviceType, std::string deviceIP, std::string fw, std::string fwVersion)
{
    m_deviceName = deviceName;
    m_deviceType = deviceType;
    m_deviceIP = deviceIP;
    m_fw = fw;
    m_fwVersion = fwVersion;

    m_deviceNameTopic = "/" + m_deviceName;
    m_deviceMacTopic = m_deviceNameTopic + "/mac";
    m_deviceIpTopic = m_deviceNameTopic + "/ip";
    m_deviceTypeTopic = m_deviceNameTopic + "/type";
    m_fwTopic = m_deviceNameTopic + "/fw";
    m_fwVersionTopic = m_fwTopic + "/version";
}

void MqttManager::publishDeviceStatusInfo()
{
    m_pubSubClient->publish(m_deviceNameTopic.c_str(), m_deviceName.c_str());
    m_pubSubClient->publish(m_deviceMacTopic.c_str(), m_deviceMac.c_str());
    m_pubSubClient->publish(m_deviceIpTopic.c_str(), m_deviceIP.c_str());
    m_pubSubClient->publish(m_deviceTypeTopic.c_str(), m_deviceType.c_str());
    m_pubSubClient->publish(m_fwTopic.c_str(), m_fw.c_str());
    m_pubSubClient->publish(m_fwVersionTopic.c_str(), m_fwVersion.c_str());

    this->refreshStatusTopics();
}

void MqttManager::checkConnectivity()
{
    if (!m_pubSubClient->connected())
    {
        if (m_pubSubClient->connect(m_deviceName.c_str(), m_mqttUsername.c_str(), m_mqttPassword.c_str()))
        {
            for (int i = 0; i < m_subscribeTopics.size(); i++)
            {
                m_pubSubClient->subscribe(m_subscribeTopics[i].c_str());
            }

            this->setDeviceMac();
            this->publishDeviceStatusInfo();
            m_connected = true;
        }
        else
        {
            m_connected = false;
        }
    }
    else
    {
        m_connected = true;
    }
}

void MqttManager::setDeviceMac()
{
    if (m_deviceMac.empty())
    {
        m_deviceMac = std::string(WiFi.macAddress().c_str());
    }
}

void MqttManager::addStatusTopic(std::string statusTopic)
{
    m_statusTopics[statusTopic] = "";
}

void MqttManager::clearStatusTopics()
{
    m_statusTopics.clear();
}

void MqttManager::addSubscribeTopic(std::string subscribeTopic)
{
    m_subscribeTopics.push_back(subscribeTopic);
}

void MqttManager::clearSubscribeTopics()
{
    m_subscribeTopics.clear();
}

void MqttManager::startConnection()
{
    this->checkConnectivity();
}

void MqttManager::stopConnection()
{
    m_pubSubClient->disconnect();
    m_connected = false;
}

void MqttManager::publishMQTT(std::string topic, std::string payload)
{
    if(m_statusTopics.find(topic) != m_statusTopics.end())
    {
        if(m_statusTopics[topic] != payload)
        {
            m_statusTopics[topic] = payload;
            m_publishMQTT = true;
        }
    }
    else if(m_connected)
    {
        std::pair<std::string, std::string> tempPair;
        tempPair.first = topic;
        tempPair.second = payload;
        m_tempPublishTopics.push_back(tempPair);
        m_publishMQTT = true;
    }
}

void MqttManager::setCallback(void (*callback)(char*, uint8_t*, unsigned int))
{
    m_pubSubClient->setCallback(callback);
}

void MqttManager::setDeviceStatusInfoTime(unsigned long deviceStatusInfoTime)
{
    m_deviceStatusInfoTimer.load(deviceStatusInfoTime);
    m_deviceStatusInfoTimer.start();
}

void MqttManager::loop()
{

    m_pubSubClient->loop();


    if (m_checkConnectivityTimer.check())
    {
        this->checkConnectivity();

        m_checkConnectivityTimer.start(); //restart timer
    }


    if (m_connected)
    {

        if (m_deviceStatusInfoTimer.check())
        {
            this->publishDeviceStatusInfo();

            m_deviceStatusInfoTimer.start(); //restart timer
        }


        if (m_publishMQTT)
        {
            this->refreshStatusTopics();

            if (!m_tempPublishTopics.empty())
            {
                for (int i = 0; i < m_tempPublishTopics.size(); i++)
                {
                    m_pubSubClient->publish(m_tempPublishTopics[i].first.c_str(), m_tempPublishTopics[i].second.c_str());
                }

                m_tempPublishTopics.clear();
            }

            m_publishMQTT = false;
        }
    }
    else
    {
        m_tempPublishTopics.clear(); //Delete temp publish topics buffer if there is not connection
    }
}

void MqttManager::refreshStatusTopics()
{
    for (std::map<std::string, std::string>::iterator it = m_statusTopics.begin(); it != m_statusTopics.end(); it++)
    {
        m_pubSubClient->publish(it->first.c_str(), it->second.c_str());
    }
}

bool MqttManager::connected()
{
    return m_connected;
}

MqttManager::~MqttManager()
{
    delete m_pubSubClient;
}
