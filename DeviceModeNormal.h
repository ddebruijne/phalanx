#include "DeviceMode.h"

#include <WiFiUdp.h>
#include <NTPClient.h>

class DeviceModeNormal : public DeviceMode
{
    WiFiUDP ntpUDP;
    NTPClient* timeClient;

public:
    bool Start();
    bool Stop();
    void OnTick();

    void ShiftCurrentTime();
};