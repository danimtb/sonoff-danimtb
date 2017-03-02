#include "DataManager.h"

DataManager::DataManager()
{
    EEPROM.begin(DM_TOTAL_MEM);
}


template <class T>
bool DataManager::EEPROMwriteAnything(uint16_t startAddress, uint16_t maxMemory, const T& value)
{
    if (sizeof(value) > maxMemory)
    {
        return false;
    }

    const byte* p = (const byte*)(const void*)&value;

    for (uint16_t i = 0; i < sizeof(value); i++)
    {
          EEPROM.write(startAddress++, *p++);
    }

    return true;
}

template <class T>
void DataManager::EEPROMreadAnything(uint16_t startAddress, uint16_t maxMemory, T& value)
{
    byte* p = (byte*)(void*)&value;

    for (uint16_t i = 0; i < maxMemory; i++)
    {
          *p++ = EEPROM.read(startAddress++);
    }

    return true;
}

void DataManager::setWifiSSID(std::string wifissid)
{
    this->EEPROMwriteAnything(DM_NETWORKDATA_ADDR_START, DM_WIFISSID_MEM, wifissid);
}


void DataManager::setup()
{
}


void DataManager::loop()
{
}
