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

uint16_t DataManager::EEPROMreadNumber(uint16_t startAddress, uint16_t maxMemory)
{
    uint16_t number;

    for (uint16_t i = startAddress; i < startAddress + maxMemory; i++)
    {
        number <<= EEPROM.read(i);
    }

    return number;
}

void DataManager::EEPROMwrite2bNumber(uint16_t startAddress, uint16_t maxMemory, uint16_t number)
{
    uint8_t numberCounter = 0;

    for (uint16_t i = startAddress; i < startAddress + maxMemory; i++)
    {
        int nbyte = (number >> (8*numberCounter)) & 0xff;
        EEPROM.write(i, nbyte);
        numberCounter++;
    }

    EEPROM.commit();
}

void DataManager::EEPROMwrite1bNumber(uint16_t startAddress, uint16_t maxMemory, uint8_t number)
{
    EEPROM.write(startAddress, number);
    EEPROM.commit();
}

void DataManager::addressToInt(std::string address, uint8_t& i1, uint8_t& i2, uint8_t& i3, uint8_t& i4)
{
    uint8_t dot1pos, dot2pos, dot3pos;

    for(uint8_t i = 0; i < address.size(); i++)
    {
        if(address[i] == '.')
        {
            dot1pos = i;
            break;
        }
    }

    for(uint8_t i = dot1pos + 1; i < address.size(); i++)
    {
        if(address[i] == '.')
        {
            dot2pos = i;
            break;
        }
    }

    for(uint8_t i = dot2pos + 1; i < address.size(); i++)
    {
        if(address[i] == '.')
        {
            dot3pos = i;
            break;
        }
    }

    i1 = atoi(address.substr(0, dot1pos+1).c_str());
    i2 = atoi(address.substr(dot1pos+1, dot2pos-(dot1pos+1)).c_str());
    i3 = atoi(address.substr(dot2pos+1, dot3pos-(dot2pos+1)).c_str());
    i4 = atoi(address.substr(dot3pos+1, address.length()-(dot3pos+1)).c_str());
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
    if (ip.size() > 6)
    {
        uint8_t ip1, ip2, ip3, ip4;

        this->addressToInt(ip, ip1, ip2, ip3, ip4);

        this->EEPROMwrite1bNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM, 1, ip1);
        this->EEPROMwrite1bNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + 1, 1, ip2);
        this->EEPROMwrite1bNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + 2, 1, ip3);
        this->EEPROMwrite1bNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + 3, 1, ip4);
    }
}

void DataManager::setMask(std::string mask)
{
    if (mask.size() > 6)
    {
        uint8_t mask1, mask2, mask3, mask4;

        this->addressToInt(mask, mask1, mask2, mask3, mask4);

        this->EEPROMwrite1bNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM, 1, mask1);
        this->EEPROMwrite1bNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + 1, 1, mask2);
        this->EEPROMwrite1bNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + 2, 1, mask3);
        this->EEPROMwrite1bNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + 3, 1, mask4);
    }
}

void DataManager::setGateway(std::string gateway)
{
    if (gateway.size() > 6)
    {
        uint8_t gateway1, gateway2, gateway3, gateway4;

        this->addressToInt(gateway, gateway1, gateway2, gateway3, gateway4);

        this->EEPROMwrite1bNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + DM_MASK_MEM, 1, gateway1);
        this->EEPROMwrite1bNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + DM_MASK_MEM + 1, 1, gateway2);
        this->EEPROMwrite1bNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + DM_MASK_MEM + 2, 1, gateway3);
        this->EEPROMwrite1bNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + DM_MASK_MEM + 3, 1, gateway4);
    }
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
    uint16_t port = atoi(mqttPort.c_str());
    this->EEPROMwrite2bNumber(DM_MQTT_ADDR_START + DM_MQTTSERVER_MEM, DM_MQTTPORT_MEM, port);
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
    char* ip;

    uint8_t ip1 = this->EEPROMreadNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM, 1);
    uint8_t ip2 = this->EEPROMreadNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + 1, 1);
    uint8_t ip3 = this->EEPROMreadNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + 2, 1);
    uint8_t ip4 = this->EEPROMreadNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + 3, 1);

    sprintf(ip, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);

    return std::string(ip);
}

std::string DataManager::getMask()
{
    char* mask;

    uint8_t mask1 = this->EEPROMreadNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM, 1);
    uint8_t mask2 = this->EEPROMreadNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + 1, 1);
    uint8_t mask3 = this->EEPROMreadNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + 2, 1);
    uint8_t mask4 = this->EEPROMreadNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + 3, 1);

    sprintf(mask, "%d.%d.%d.%d", mask1, mask2, mask3, mask4);

    return std::string(mask);
}

std::string DataManager::getGateway()
{
    char* gateway;

    uint8_t gateway1 = this->EEPROMreadNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + DM_MASK_MEM, 1);
    uint8_t gateway2 = this->EEPROMreadNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + DM_MASK_MEM + 1, 1);
    uint8_t gateway3 = this->EEPROMreadNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + DM_MASK_MEM + 2, 1);
    uint8_t gateway4 = this->EEPROMreadNumber(DM_NETWORKDATA_ADDR_START + DM_WIFISSID_MEM + DM_WIFIPASS_MEM + DM_IP_MEM + DM_MASK_MEM + 3, 1);

    sprintf(gateway, "%d.%d.%d.%d", gateway1, gateway2, gateway3, gateway4);

    return std::string(gateway);
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
    char * mqttport;
    uint16_t port = this->EEPROMreadNumber(DM_MQTT_ADDR_START + DM_MQTTSERVER_MEM, DM_MQTTPORT_MEM);
    sprintf(mqttport, "%d", port);
    return std::string(mqttport);
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

    return this->EEPROMreadString(DM_DEVICEDATA_ADDR_START + DM_DEVICEDATA_ADDR_END + DM_MQTTTOPIC_MEM*index, DM_MQTTTOPIC_MEM);
}
