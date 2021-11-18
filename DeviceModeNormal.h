#include "DeviceMode.h"

#include <WiFiUdp.h>
#include <NTPClient.h>

class DeviceModeNormal : public DeviceMode
{
    WiFiUDP ntpUDP;
    NTPClient* timeClient;

    const uint8_t delayBetweenTicks = 100;
    const uint8_t delayBetweenNTPUpdates = 30000;
    uint8_t timeSinceLastNTPUpdate = 0;

public:
    bool Start();
    bool Stop();
    void OnTick();

    void ShiftCurrentTime();
};