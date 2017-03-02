#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>

#include "SimpleTimer.h"

#undef min
#undef max

#include <string>

class WifiManager
{
private:
    std::string m_staSSID;
    std::string m_staPASS;
    std::string m_apSSID;

    IPAddress m_ip;
    IPAddress m_mask;
    IPAddress m_gateway;

    std::string m_macAddress;

    bool m_staMode;
    bool m_apMode;

    SimpleTimer m_checkConnectivityTimer;

    void checkConnectivity();

public:
    WifiManager();

    void setup(std::string staSSID, std::string staPASS, std::string ip, std::string mask, std::string gateway, std::string apSSID);

    void connectStaWifi();
    void createApWifi();
    void disconnectStaWifi();
    void destroyApWifi();

    bool apModeEnabled();

    bool connected();

    void loop();
};

#endif
