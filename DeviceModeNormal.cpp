#include "DeviceModeNormal.h"

#include <SPI.h>
#include <ESP8266WiFi.h>
#include "constants.h"

bool DeviceModeNormal::Start()
{
    timeClient = new NTPClient(ntpUDP, "pool.ntp.org");

	Serial.println(WiFi.localIP());

	timeClient->begin();
	timeClient->setTimeOffset(0);
	timeClient->update();

    return true;
}

bool DeviceModeNormal::Stop()
{
    return false;
}

void DeviceModeNormal::OnTick()
{
	timeClient->update();
	display->ShiftCurrentTime(timeClient->getHours(), timeClient->getMinutes(), timeClient->getSeconds());
    delay(delayBetweenTicks);
}