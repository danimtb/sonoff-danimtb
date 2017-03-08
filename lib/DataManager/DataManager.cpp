#include "DataManager.h"

DataManager::DataManager()
{
    EEPROM.begin(DM_TOTAL_MEM);
}

std::string DataManager::EEPROMreadString(uint16_t startAddress, uint16_t maxMemory)
{
    String data;

    for (uint16_t i = startAddress; i < startAddress + maxMemory; i++)
    {
        char newChar = char(EEPROM.read(i));

        if (newChar == '&')
        {
            break;
        }
        else
        {
            data += newChar;
        }
    }

    return std::string(data.c_str());
}

void DataManager::EEPROMwriteString(uint16_t startAddress, uint16_t maxMemory, std::string data)
{
    String stringData(data.c_str());

    uint16_t stringCounter = 0;

    for (uint16_t i = startAddress; i < startAddress + maxMemory; i++)
    {
        if (i < startAddress + stringData.length())
        {
            EEPROM.write(i, stringData[stringCounter]);
            stringCounter++;
        }
        else
        {
            EEPROM.write(i, '&');
        }
    }

    EEPROM.commit();
}


void DataManager::setWifiSSID(std::string wifissid)
{
    this->EEPROMwriteString(DM_NETWORKDATA_ADDR_START, DM_WIFISSID_MEM, wifissid);
}

void DataManager::setWifiPass(std::string wifipass)
{
    this->EEPROMwriteString(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM, DM_WIFIPASS_MEM, wifipass);
}

void DataManager::setIP(std::string ip)
{
    this->EEPROMwriteString(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM, DM_IP_MEM, ip);
}

void DataManager::setMask(std::string mask)
{
    this->EEPROMwriteString(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM, DM_MASK_MEM, mask);
}

void DataManager::setGateway(std::string gateway)
{
    this->EEPROMwriteString(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + DM_MASK_MEM, DM_GATEWAY_MEM, gateway);
}

void DataManager::setOta(std::string ota)
{
    this->EEPROMwriteString(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + DM_MASK_MEM + DM_GATEWAY_MEM, DM_OTA_MEM, ota);
}

void DataManager::setMqttServer(std::string mqttServer)
{
    this->EEPROMwriteString(DM_MQTT_ADDR_START, DM_MQTTSERVER_MEM, mqttServer);
}

void DataManager::setMqttPort(std::string mqttPort)
{
    this->EEPROMwriteString(DM_MQTT_ADDR_START + DM_MQTTSERVER_MEM, DM_MQTTPORT_MEM, mqttPort);
}

void DataManager::setMqttUser(std::string mqttUser)
{
    this->EEPROMwriteString(DM_MQTT_ADDR_START + DM_MQTTSERVER_MEM + DM_MQTTPORT_MEM, DM_MQTTUSER_MEM, mqttUser);
}

void DataManager::setMqttPass(std::string mqttPass)
{
    this->EEPROMwriteString(DM_MQTT_ADDR_START + DM_MQTTSERVER_MEM + DM_MQTTPORT_MEM + DM_MQTTUSER_MEM, DM_MQTTPASS_MEM, mqttPass);
}

void DataManager::setDeviceName(std::string deviceName)
{
    this->EEPROMwriteString(DM_DEVICEDATA_ADDR_START, DM_DEVICENAME_MEM, deviceName);
}

void DataManager::setMqttTopic(uint8_t index, std::string mqttTopic)
{
    if(index > 5)
    {
        index = 5;
    }

    this->EEPROMwriteString(DM_DEVICEDATA_ADDR_START + DM_DEVICENAME_MEM + DM_MQTTTOPIC_MEM*index, DM_MQTTTOPIC_MEM, mqttTopic);
}


std::string DataManager::getWifiSSID()
{
    return this->EEPROMreadString(DM_NETWORKDATA_ADDR_START, DM_WIFISSID_MEM);
}

std::string DataManager::getWifiPass()
{
    return this->EEPROMreadString(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM, DM_WIFIPASS_MEM);
}

std::string DataManager::getIP()
{
    return this->EEPROMreadString(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM, DM_IP_MEM);
}

std::string DataManager::getMask()
{
    return this->EEPROMreadString(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM, DM_MASK_MEM);
}

std::string DataManager::getGateway()
{
    return this->EEPROMreadString(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + DM_MASK_MEM, DM_GATEWAY_MEM);
}

std::string DataManager::getOta()
{
    return this->EEPROMreadString(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + DM_MASK_MEM + DM_GATEWAY_MEM, DM_OTA_MEM);
}

std::string DataManager::getMqttServer()
{
    return this->EEPROMreadString(DM_MQTT_ADDR_START, DM_MQTTSERVER_MEM);
}

std::string DataManager::getMqttPort()
{
    return this->EEPROMreadString(DM_MQTT_ADDR_START + DM_MQTTSERVER_MEM, DM_MQTTPORT_MEM);
}

std::string DataManager::getMqttUser()
{
    return this->EEPROMreadString(DM_MQTT_ADDR_START + DM_MQTTSERVER_MEM + DM_MQTTPORT_MEM, DM_MQTTUSER_MEM);
}

std::string DataManager::getMqttPass()
{
    return this->EEPROMreadString(DM_MQTT_ADDR_START + DM_MQTTSERVER_MEM + DM_MQTTPORT_MEM + DM_MQTTUSER_MEM, DM_MQTTPASS_MEM);
}

std::string DataManager::getDeviceName()
{
    return this->EEPROMreadString(DM_DEVICEDATA_ADDR_START, DM_DEVICENAME_MEM);
}

std::string DataManager::getMqttTopic(uint8_t index)
{
    if (index > 6)
    {
        index = 5;
    }

    return this->EEPROMreadString(DM_DEVICEDATA_ADDR_START + DM_DEVICENAME_MEM + DM_MQTTTOPIC_MEM*index, DM_MQTTTOPIC_MEM);
}
