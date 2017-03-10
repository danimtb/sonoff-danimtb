#include "UpdateManager.h"

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

UpdateManager::UpdateManager()
{
    m_checkUpdateTimer.setup(RT_ON, 300000); // 5 minutes
}

void UpdateManager::setup(std::string host, std::string fw, std::string fwVersion, std::string device)
{
    m_host = host;
    m_fw = fw;
    m_fwVersion = fwVersion;
    m_device = device;

    m_checkUpdateTimer.start();
}

int16_t UpdateManager::getServerResponse(std::string &payload)
{
    std::string request = m_host + "/" + m_fw + "/" + m_fwVersion + "/" + m_device;

    HTTPClient http;
    http.begin((char *)request.c_str());
    http.useHTTP10(true);
    http.setTimeout(8000);

    int16_t httpCode = http.GET();

    if (httpCode > 0)
    {
        payload = http.getString().c_str();
    }
    else
    {
        payload = "";
    }

    http.end();

    return httpCode;
}

void UpdateManager::checkUpdate()
{
    std::string payload;
    std::string firmwareDownload = "";
    std::string spiffsDownload = "";

    uint16_t httpCode = this->getServerResponse(payload);


    if (httpCode == 200)
    {
        if (!payload.empty())
        {
            StaticJsonBuffer<500> jsonBuffer;
            JsonObject& response = jsonBuffer.parseObject(payload.c_str());

            if (!response.success())
            {
                return; // Error parsing JSON
            }

            if (response.containsKey("version"))
            {
                if (response.containsKey("fw"))
                {
                    firmwareDownload = response["fw"].as<char*>();
                }
                else
                {
                    return; // Error: no firmware
                }

                if (response.containsKey("spiffs"))
                {
                    spiffsDownload = response["spiffs"].as<char*>();
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
    }
    else if (httpCode == -1)
    {
        return; // Could not reach update server
    }
    else //(httpCode == 500 || httpCode == 404 || httpCode == 400)
    {
        return; // Bad request
    }

    this->update(firmwareDownload, spiffsDownload);
}

void UpdateManager::update(std::string firmwarePath, std::string spiffsPath)
{
    firmwarePath = m_host + firmwarePath;
    spiffsPath = m_host + spiffsPath;

    ESPhttpUpdate.rebootOnUpdate(false);

    if (this->updateSpiffs(spiffsPath))
    {
        if (this->updateFirmware(firmwarePath))
        {
            ESP.restart(); // Update Successful: RESTART DEVICE
        }
    }
}

bool UpdateManager::updateSpiffs(std::string spiffsUrl)
{
    bool spiffsSuccessful = false;

    if (!spiffsUrl.empty())
    {
        t_httpUpdate_return ret = ESPhttpUpdate.updateSpiffs(spiffsUrl.c_str());

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

    return spiffsSuccessful;
}

bool UpdateManager::updateFirmware(std::string firmwareUrl)
{
    bool firmwareSuccessful = false;

    if (!firmwareUrl.empty())
    {
        t_httpUpdate_return ret = ESPhttpUpdate.update(firmwareUrl.c_str());

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
