#include "DeviceMode.h"

#include <WiFiUdp.h>
#include <NTPClient.h>

class DeviceModeNormal : public DeviceMode
{
    WiFiUDP ntpUDP;
    NTPClient* timeClient;

    const uint8_t delayBetweenTicks = 100;  // 1/10th of a second

public:
    bool Start();
    bool Stop();
    void OnTick();

    void ShiftCurrentTime(int hour, int minute, int second);
};