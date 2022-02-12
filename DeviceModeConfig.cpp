#include "DeviceModeConfig.h"

#include <ESP8266WiFi.h>
#include "constants.h"
#include "defines.h"

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

	#ifdef DISPLAYTYPE_IV4
		display->ShiftText("Configure me");
	#else
		display->ShiftText("Conf...");
	#endif

    return true;
}

bool DeviceModeConfig::Stop()
{
    return true;
}