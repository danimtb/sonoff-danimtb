#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>
#include <EEPROM.h>

#include <string>

#define DM_BEGIN_ADDR 0
#define DM_TOTAL_MEM 512
#define DM_END_ADDR 511

#define DM_NETWORKDATA_ADDR_START 0
#define DM_WIFISSID_MEM 32
#define DM_WIFIPASS_MEM 32
#define DM_IP_MEM 4 //1+1+1+1
#define DM_MASK_MEM 4 //1+1+1+1
#define DM_GATEWAY_MEM 4 //1+1+1+1
#define DM_OTA_MEM 32
#define DM_NETWORKDATA_ADDR_END 107

#define DM_MQTT_ADDR_START 108
#define DM_MQTTSERVER_MEM 32
#define DM_MQTTPORT_MEM 2
#define DM_MQTTUSER_MEM 32
#define DM_MQTTPASS_MEM 32
#define DM_MQTT_ADDR_END 205

#define DM_DEVICEDATA_ADDR_START 206
#define DM_DEVICENAME_MEM 32
#define DM_MQTTSTATUS_MEM 40
#define DM_MQTTCOMMAND_MEM 40
#define DM_MQTTSECOND_MEM 40
#define DM_DEVICEDATA_ADDR_END 369

class DataManager
{

public:
    DataManager();

    void setWifiSSID(std::string wifissid);
    void setWifiPass(std::string wifipass);
    void setIP(byte ip1, byte ip2, byte ip3, byte ip4);
    void setMask(byte mask1, byte mask2, byte mask3, byte mask4);
    void setGateway(byte gateway1, byte gateway2, byte gateway3, byte gateway4);

    void setup();

    void loop();
};

#endif
