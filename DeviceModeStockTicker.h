#include "DeviceMode.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

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