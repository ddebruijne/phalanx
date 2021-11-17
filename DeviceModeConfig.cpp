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

	digitalWrite(GPIO_LATCH, LOW);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::dot);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::dot);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::F | TubeCharacter::dot);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::n);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::o);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::c);
	digitalWrite(GPIO_LATCH, HIGH);

    return true;
}

bool DeviceModeConfig::Stop()
{
    return false;
}

void DeviceModeConfig::OnTick()
{
}