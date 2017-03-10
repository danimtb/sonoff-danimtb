#include "UpdateManager.h"

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

UpdateManager::UpdateManager()
{
    m_checkUpdateTimer.setup(RT_ON, 120000); // 20 minutes
}

void UpdateManager::setup(std::string host, std::string fw, std::string fwVersion, std::string device)
{
    m_host = host;
    m_fw = fw;
    m_fwVersion = fwVersion;
    m_device = device;

    m_checkUpdateTimer.start();
}

std::string UpdateManager::getServerResponse()
{
    std::string request = m_host + "/" + m_fw + "/" + m_fwVersion + "/" + m_device;

    HTTPClient http;
    http.begin((char *)request.c_str());
    http.useHTTP10(true); // ?
    http.setTimeout(8000);

    int httpCode = http.GET();

    std::string payload;
    if (httpCode > 0)
    {
        payload = http.getString().c_str();
    }
    else
    {
        payload = "";
    }
    http.end();

    return payload;
}

void UpdateManager::checkUpdate()
{
    std::string payload = getServerResponse();

    if (!payload.empty())
    {
        StaticJsonBuffer<500> jsonBuffer;
        JsonObject& response = jsonBuffer.parseObject(payload.c_str());

        if (!response.success())
        {
          return; // parseObject() failed
        }

        if (response.containsKey("version"))
        {
            if (response.containsKey("fw"))
            {
                std::string fw(response["fw"].as<char*>());
                m_firmwareDownload = fw;
            }
            else
            {
                return; // Error: no firmware
            }

            if (response.containsKey("spiffs"))
            {
                std::string spiffs(response["spiffs"].as<char*>());
                m_spiffsDownload = spiffs;
            }
            else
            {
                // No spiffs to flash
            }
        }
        else
        {
            return; // Device is up to date
        }
    }
    else
    {
        return; // Could not reach update server
    }

    this->update();
}

void UpdateManager::update()
{
    bool spiffsSuccess = false;
    bool firmwareSuccess = false;

    ESPhttpUpdate.rebootOnUpdate(false);

    spiffsSuccess = this->updateSpiffs();
    firmwareSuccess = this->updateFirmware();

    if (spiffsSuccess && firmwareSuccess)
    {
        ESP.restart();
    }
}

bool UpdateManager::updateSpiffs()
{
    bool spiffsSuccessful = false;

    if (!m_spiffsDownload.empty())
    {
        std::string spiffs = m_host + m_spiffsDownload;
        t_httpUpdate_return ret = ESPhttpUpdate.updateSpiffs(spiffs.c_str());

        if (ret == HTTP_UPDATE_FAILED)
        {
            spiffsSuccessful = false; //Error: SPIFFS update failed
        }
        else if (ret == HTTP_UPDATE_OK)
        {
            spiffsSuccessful = true; //SPIFFS update successful
        }
    }
    else
    {
        spiffsSuccessful = true; //No SPIFFS to flash, continue
    }

    m_spiffsDownload.clear();

    return spiffsSuccessful;
}

bool UpdateManager::updateFirmware()
{
    bool firmwareSuccessful = false;

    if (!m_firmwareDownload.empty())
    {
        std::string firmware = m_host + m_firmwareDownload;
        t_httpUpdate_return ret = ESPhttpUpdate.update(firmware.c_str());

        if (ret == HTTP_UPDATE_FAILED)
        {
            firmwareSuccessful = false; //Error: firmware update failed
        }
        else if (ret == HTTP_UPDATE_OK)
        {
            firmwareSuccessful = true; //Firmware update successful
        }
    }
    else
    {
        firmwareSuccessful = false; //Error: No firmware to flash
    }

    m_firmwareDownload.clear();

    return firmwareSuccessful;
}

void UpdateManager::loop()
{
    if (m_checkUpdateTimer.check())
    {
        this->checkUpdate();
        m_checkUpdateTimer.start();
    }
}
