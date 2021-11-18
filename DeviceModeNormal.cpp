#include "DeviceModeNormal.h"

#include <SPI.h>
#include <ESP8266WiFi.h>
#include "constants.h"

bool DeviceModeNormal::Start()
{
    timeClient = new NTPClient(ntpUDP, "pool.ntp.org");

	// Connect to WLAN
	WiFi.begin("Schotsman_IOT", "pannenkoekmetkaas");

	char data[6] = {
		TubeCharacter::dot,
		TubeCharacter::dot,
		TubeCharacter::n | TubeCharacter::dot,
		TubeCharacter::n,
		TubeCharacter::o,
		TubeCharacter::c,
	};
	display->ShiftRaw(data);

	while (WiFi.status() != WL_CONNECTED) 
		delay(100);

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
	int second = timeClient->getSeconds();
	if (second == 30) 
	{
		Serial.println("Attempting to sync with NTP...");
		timeClient->update();
	}

	display->ShiftCurrentTime(timeClient->getHours(), timeClient->getMinutes(), second);
    delay(delayBetweenTicks);
}