#include "DeviceModeConfig.h"

#include <SPI.h>
#include <ESP8266WiFi.h>
#include "constants.h"

bool DeviceModeConfig::Start()
{
    IPAddress ip = IPAddress(192, 168, 0, 1);
	IPAddress subnetMask = IPAddress(255, 255, 255, 0);
	uint8_t macAddr[6];

	WiFi.softAPmacAddress(macAddr);
	WiFi.softAPConfig(ip, ip, subnetMask);
	WiFi.softAP("Phalanx-IV6");

	char data[6] = {
		TubeCharacter::dot,
		TubeCharacter::dot,
		TubeCharacter::F | TubeCharacter::dot,
		TubeCharacter::n,
		TubeCharacter::o,
		TubeCharacter::c,
	};
	display->ShiftRaw(data);

    return true;
}

bool DeviceModeConfig::Stop()
{
    return false;
}

void DeviceModeConfig::OnTick()
{
}