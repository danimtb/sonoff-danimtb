#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <Arduino.h>
#include <EEPROM.h>

#include <cstdint>
#include <string>

#define DM_BEGIN_ADDR 0
#define DM_TOTAL_MEM 512
#define DM_END_ADDR 511

#define DM_NETWORKDATA_ADDR_START 0
#define DM_WIFISSID_MEM 32
#define DM_WIFIPASS_MEM 32
#define DM_IP_MEM 15
#define DM_MASK_MEM 15
#define DM_GATEWAY_MEM 15
#define DM_OTA_MEM 32
#define DM_NETWORKDATA_ADDR_END 140

#define DM_MQTT_ADDR_START 141
#define DM_MQTTSERVER_MEM 32
#define DM_MQTTPORT_MEM 4
#define DM_MQTTUSER_MEM 32
#define DM_MQTTPASS_MEM 32
#define DM_MQTT_ADDR_END 240

#define DM_DEVICEDATA_ADDR_START 241
#define DM_DEVICENAME_MEM 15
#define DM_MQTTTOPIC_MEM 40 // 40 x 6
#define DM_DEVICEDATA_ADDR_END 495

class DataManager
{
private:
    std::string EEPROMreadString(uint16_t startAddress, uint16_t maxMemory);
    void EEPROMwriteString(uint16_t startAddress, uint16_t maxMemory, std::string data);

public:
    DataManager();

    void setWifiSSID(std::string wifissid);
    void setWifiPass(std::string wifipass);
    void setIP(std::string ip);
    void setMask(std::string mask);
    void setGateway(std::string gateway);
    void setOta(std::string ota);

    void setMqttServer(std::string mqttServer);
    void setMqttPort(std::string mqttPort);
    void setMqttUser(std::string mqttUser);
    void setMqttPass(std::string mqttPass);

    void setDeviceName(std::string deviceName);
    void setMqttTopic(uint8_t index, std::string mqttTopic);


    std::string getWifiSSID();
    std::string getWifiPass();
    std::string getIP();
    std::string getMask();
    std::string getGateway();
    std::string getOta();

    std::string getMqttServer();
    std::string getMqttPort();
    std::string getMqttUser();
    std::string getMqttPass();

    std::string getDeviceName();
    std::string getMqttTopic(uint8_t index);

    void setup();

    void loop();
};

#endif
