#include "WifiMQTTManager.h"

WifiMQTTManager::WifiMQTTManager()
{
    m_connected = false;
    m_publishMQTT = false;

    m_deviceStatusInfoTimer.setup(RT_ON, 300000);   // 5 min
    m_checkConnectivityTimer.setup(RT_ON, 20000); // 20 secs
}

void WifiMQTTManager::setup(std::string wifiSSID, std::string wifiPASS, std::string mqttServer, uint16_t mqttPort, std::string mqttUsername, std::string mqttPassword, std::string ip, std::string mask, std::string gateway, std::string deviceName, std::string deviceType, std::string fw, std::string fwVersion)
{
    m_wifiSSID = wifiSSID;
    m_wifiPASS = wifiPASS;
    m_mqttServer = mqttServer;
    m_mqttPort = mqttPort;
    m_mqttUsername = mqttUsername;
    m_mqttPassword = mqttPassword;
    m_deviceName = deviceName;
    m_deviceType = deviceType;
    m_fw = fw;
    m_fwVersion = fwVersion;

    m_deviceNameTopic = "/" + m_deviceName;
    m_deviceMacTopic = m_deviceNameTopic + "/mac";
    m_deviceIpTopic = m_deviceNameTopic + "/ip";
    m_deviceTypeTopic = m_deviceNameTopic + "/type";
    m_fwTopic = m_deviceNameTopic + "/fw";
    m_fwVersionTopic = m_fwTopic + "/version";

    m_ip.fromString(ip.c_str());
    m_mask.fromString(mask.c_str());
    m_gateway.fromString(gateway.c_str());

    m_pubSubClient = new PubSubClient(m_wifiClient);
    m_pubSubClient->setServer(mqttServer.c_str(), mqttPort);

    m_deviceStatusInfoTimer.start();
    m_checkConnectivityTimer.start();

    this->initWifi();
}

std::string WifiMQTTManager::getMacAddress()
{
    return std::string(WiFi.macAddress().c_str());
}

void WifiMQTTManager::initWifi()
{
    WiFi.config(m_ip, m_gateway, m_mask);
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
}

void WifiMQTTManager::connectWifi()
{
    WiFi.begin(m_wifiSSID.c_str(), m_wifiPASS.c_str());
    delay(1000);
}

void WifiMQTTManager::publishDeviceStatusInfo()
{
    m_pubSubClient->publish(m_deviceNameTopic.c_str(), m_deviceName.c_str());
    m_pubSubClient->publish(m_deviceMacTopic.c_str(), m_deviceMac.c_str());
    m_pubSubClient->publish(m_deviceIpTopic.c_str(), m_ip.toString().c_str());
    m_pubSubClient->publish(m_deviceTypeTopic.c_str(), m_deviceType.c_str());
    m_pubSubClient->publish(m_fwTopic.c_str(), m_fw.c_str());
    m_pubSubClient->publish(m_fwVersionTopic.c_str(), m_fwVersion.c_str());

    this->refreshStatusTopics();
}

void WifiMQTTManager::checkConnectivity()
{
    if(WiFi.status() != WL_CONNECTED)
    {
        m_connected = false;
        this->connectWifi();

        if (WiFi.status() == WL_CONNECTED)
        {
            this->checkMQTTConnectivity();
            this->setDeviceMac();
        }
    }
    else
    {
        this->checkMQTTConnectivity();
        this->setDeviceMac();
    }
}

void WifiMQTTManager::setDeviceMac()
{
    if (m_deviceMac.empty())
    {
        m_deviceMac = this->getMacAddress();
    }
}


void WifiMQTTManager::checkMQTTConnectivity()
{
    if (!m_pubSubClient->connected())
    {
        if (m_pubSubClient->connect(m_deviceName.c_str(), m_mqttUsername.c_str(), m_mqttPassword.c_str()))
        {
            for (int i = 0; i < m_subscribeTopics.size(); i++)
            {
                m_pubSubClient->subscribe(m_subscribeTopics[i].c_str());
            }

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

void WifiMQTTManager::addStatusTopic(std::string statusTopic)
{
    m_statusTopics[statusTopic] = "";
}

void WifiMQTTManager::eraseStatusTopic(std::string statusTopic)
{
    m_statusTopics.erase(statusTopic);
}

void WifiMQTTManager::addSubscribeTopic(std::string subscribeTopic)
{
    m_subscribeTopics.push_back(subscribeTopic);
}

void WifiMQTTManager::eraseSubscribeTopic(std::string statusTopic)
{
    // TODO: improve erase of topic

    for (int i = 0; i < m_subscribeTopics.size(); i++)
    {
        if (m_subscribeTopics[i] == statusTopic)
        {
            m_subscribeTopics.erase(m_subscribeTopics.begin() + i);
        }
    }
}

void WifiMQTTManager::startConnection()
{
    this->checkConnectivity();
}

void WifiMQTTManager::stopConnection()
{
    m_pubSubClient->disconnect();
    WiFi.disconnect();
}

void WifiMQTTManager::publishMQTT(std::string topic, std::string payload)
{
    if(m_statusTopics.find(topic) != m_statusTopics.end())
    {
        if(m_statusTopics[topic] != payload)
        {
            m_statusTopics[topic] = payload;
            m_publishMQTT = true;
        }
    }
    else
    {
        std::pair<std::string, std::string> tempPair;
        tempPair.first = topic;
        tempPair.second = payload;
        m_tempPublishTopics.push_back(tempPair);
        m_publishMQTT = true;
    }
}

void WifiMQTTManager::setCallback(void (*callback)(char*, uint8_t*, unsigned int))
{
    m_pubSubClient->setCallback(callback);
}

void WifiMQTTManager::setCheckConnectivityTime(unsigned long checkConnectivityTime)
{
    m_checkConnectivityTimer.load(checkConnectivityTime);
    m_checkConnectivityTimer.start();
}

void WifiMQTTManager::setDeviceStatusInfoTime(unsigned long deviceStatusInfoTime)
{
    m_deviceStatusInfoTimer.load(deviceStatusInfoTime);
    m_deviceStatusInfoTimer.start();
}

void WifiMQTTManager::loop()
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
            if (!m_tempPublishTopics.empty())
            {
                for (int i = 0; i < m_tempPublishTopics.size(); i++)
                {
                    m_pubSubClient->publish(m_tempPublishTopics[i].first.c_str(), m_tempPublishTopics[i].second.c_str());
                }

                m_tempPublishTopics.clear();
            }

            this->refreshStatusTopics();

            m_publishMQTT = false;
        }
    }
}

void WifiMQTTManager::refreshStatusTopics()
{
    for (std::map<std::string, std::string>::iterator it = m_statusTopics.begin(); it != m_statusTopics.end(); it++)
    {
        m_pubSubClient->publish(it->first.c_str(), it->second.c_str());
    }
}

bool WifiMQTTManager::connected()
{
    return m_connected;
}

WifiMQTTManager::~WifiMQTTManager()
{
    delete m_pubSubClient;
}
