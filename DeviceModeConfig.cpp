#include "DeviceModeConfig.h"

#include <ESP8266WiFi.h>
#include "constants.h"

bool DeviceModeConfig::Start()
{
    IPAddress ip = IPAddress(192, 168, 0, 1);
	IPAddress subnetMask = IPAddress(255, 255, 255, 0);
	uint8_t macAddr[6];

	if (WiFi.getMode() == WIFI_AP)
		WiFi.mode(WIFI_STA);

	WiFi.softAPmacAddress(macAddr);
	WiFi.softAPConfig(ip, ip, subnetMask);
	WiFi.softAP(deviceName->c_str());

	display->ShiftText("Conf...");

    return true;
}

bool DeviceModeConfig::Stop()
{
    return true;
}

void DeviceModeConfig::OnTick()
{
	delay(1);
}