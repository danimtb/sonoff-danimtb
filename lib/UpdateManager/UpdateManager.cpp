#include "UpdateManager.h"

#include <ArduinoJson.h>
#include <ESP8266HTTPClient>
#include <ESP8266httpUpdate.h>

UpdateManager::UpdateManager()
{
    m_checkUpdateTimer.setup(RT_ON, 600000); // 10 minutes
}

void UpdateManager::setup(std::string host, std::string fw, std::string fwVersion, std::string device)
{
    m_host = host;
    m_fw = fw;
    m_fwVersion = fwVersion;
    m_device = device;

    m_checkUpdateTimer.start();
}

void UpdateManager::checkUpdate()
{
    std::string payload = getServerResponse();

    if (!payload.empty())
    {
        //parse json response
    }
    else
    {
        //Could not reach update server
    }
}

std::string UpdateManager::getServerResponse()
{
    std::string request = "/" + m_fw + "/" + m_fwVersion + "/" + m_device;

    ESP8266HTTPClient http;
    http.begin(request.c_str());
    http.useHTTP10(true);
    http.setTimeout(8000);

    int httpCode = http.GET();

    std::string payload;
    if (httpCode > 0)
    {
        payload = http.getString();
    }
    else
    {
        payload = "";
    }
    http.end();

    return payload;
}

void UpdateManager::loop()
{
    if( m_checkUpdateTimer)
    {
        this->checkUpdate();
        m_checkUpdateTimer.start();
    }
}
