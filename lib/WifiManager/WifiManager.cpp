#include "WifiManager.h"

WifiManager::WifiManager()
{
    m_staMode = false;
    m_apMode = false;
}

void WifiManager::setup(std::string staSSID, std::string staPASS, std::string ip, std::string mask, std::string gateway, std::string apSSID)
{
    m_staSSID = staSSID;
    m_staPASS = staPASS;

    if (apSSID.empty())
    {
        m_apSSID = "CONFIG DEVICE";
    }
    else
    {
        m_apSSID = apSSID;
    }

    m_ip.fromString(ip.c_str());
    m_mask.fromString(mask.c_str());
    m_gateway.fromString(gateway.c_str());

    m_checkConnectivityTimer.setup(RT_ON, 20000);
    m_checkConnectivityTimer.start();
}

void WifiManager::connectStaWifi()
{
    if (m_apMode)
    {
        this->destroyApWifi();
    }

    WiFi.config(m_ip, m_gateway, m_mask);
    WiFi.mode(WIFI_STA);
    WiFi.begin(m_staSSID.c_str(), m_staPASS.c_str());
    delay(1000);

    m_staMode = true;
}

void WifiManager::disconnectStaWifi()
{
    WiFi.disconnect();
    delay(100);

    m_staMode = false;
}

void WifiManager::createApWifi()
{
    if(m_staMode)
    {
        this->disconnectStaWifi();
    }

    WiFi.config(IPAddress(192, 168, 4, 1), m_gateway, m_mask);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(m_apSSID.c_str());
    delay(1000);

    m_apMode = true;
}

void WifiManager::destroyApWifi()
{
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    delay(100);

    m_apMode = false;
}


void WifiManager::checkConnectivity()
{
    if(WiFi.status() != WL_CONNECTED)
    {
        this->connectStaWifi();
    }
}

void WifiManager::loop()
{
    if (m_staMode)
    {
        if (m_checkConnectivityTimer.check())
        {
            this->checkConnectivity();
            m_checkConnectivityTimer.start();
        }
    }
}

bool WifiManager::connected()
{
    return WiFi.status() == WL_CONNECTED;
}

bool WifiManager::apModeEnabled()
{
    return m_apMode;
}
