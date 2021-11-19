#include "DeviceMode.h"
#include "EEPROMData.h"

#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP_EEPROM.h>

class DeviceModeNormal : public DeviceMode
{
    WiFiUDP ntpUDP;
    NTPClient* timeClient;
    EEPROMData saveData;

    const uint8_t delayBetweenTicks = 100;  // 1/10th of a second
    const uint16_t delayBetweenSettingsSync = 1000; // 1s
    uint16_t timeSinceLastSettingsSync = 0;

public:
    bool Start();
    bool Stop();
    void OnTick();
};