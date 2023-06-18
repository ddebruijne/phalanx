#include "DeviceMode.h"
#include <ESP8266HTTPClient.h>

class DeviceModeStockTicker : public DeviceMode
{
private:
    HTTPClient httpClient;

public:
    bool Start();
    bool Stop();
    void OnTick();
    void OnSerialDataReceived(String s) {};
    EDeviceMode GetDeviceMode() { return EDeviceMode::StockTicker; };
};