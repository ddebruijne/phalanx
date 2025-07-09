#include "DeviceMode.h"
#include "EEPROMData.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "constants.h"

class DeviceModeStockTicker : public DeviceMode
{
public:
    const uint8_t delayBetweenTicks = 100;  // 1/10th of a second

private:
    EEPROMData saveData;
    WiFiClientSecure client;
    const uint16_t delayBetweenUpdates = 1000 * 60 * 60 * 15; // 1s * 60 * 60 * 15 = 15 min.
    long timeSinceLastUpdate = 0;

public:
    bool Start();
    bool Stop();
    void OnTick();
    void OnSerialDataReceived(String s) {};
    EDeviceMode GetDeviceMode() { return EDeviceMode::StockTicker; };

private:
    bool UpdateSymbol();
};
