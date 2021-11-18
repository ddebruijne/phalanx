#include "DeviceModeNormal.h"
#include <ESP8266WiFi.h>

bool DeviceModeNormal::Start()
{
    timeClient = new NTPClient(ntpUDP, "pool.ntp.org");

	timeClient->begin();
	timeClient->setTimeOffset(0);

    return true;
}

bool DeviceModeNormal::Stop()
{
	timeClient->end();
	delete timeClient;
    return true;
}

void DeviceModeNormal::OnTick()
{
	timeClient->update();
	display->ShiftCurrentTime(timeClient->getHours(), timeClient->getMinutes(), timeClient->getSeconds());
    delay(delayBetweenTicks);
}