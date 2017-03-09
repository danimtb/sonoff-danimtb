#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <Arduino.h>

#include "SimpleTimer.h"

#undef min
#undef max

#include <string>

class UpdateManager
{
private:
    SimpleTimer m_checkUpdateTimer;

    std::string m_host;
    std::string m_fw;
    std::string m_fwVersion;
    std::string m_device;

    std::string m_firmwareDownload;
    std::string m_spiffsDownload;

    std::string getServerResponse();
    bool updateSpiffs();
    bool updateFirmware();
    void update();

public:
    UpdateManager();
    void checkUpdate();
    void setup(std::string host, std::string fw, std::string fwVersion, std::string device);
    void loop();
};

#endif
