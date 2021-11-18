#include "DeviceModeNormal.h"

#include <SPI.h>
#include <ESP8266WiFi.h>
#include "constants.h"

bool DeviceModeNormal::Start()
{
    timeClient = new NTPClient(ntpUDP, "pool.ntp.org");

	// Connect to WLAN
	WiFi.begin("Schotsman_IOT", "pannenkoekmetkaas");

	digitalWrite(GPIO_LATCH, LOW);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::dot);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::dot);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::n | TubeCharacter::dot);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::n);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::o);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::c);
	digitalWrite(GPIO_LATCH, HIGH);
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
    int hour = timeClient->getHours();
	int minute = timeClient->getMinutes();
	int second = timeClient->getSeconds();

	if (minute == 30 && second == 0) 
	{
		Serial.println("Attempting to sync with NTP...");
		timeClient->update();
	}

    ShiftCurrentTime(hour, minute, second);

    delay(delayBetweenTicks);
}

void DeviceModeNormal::ShiftCurrentTime(int hour, int minute, int second)
{
	digitalWrite(GPIO_LATCH, LOW);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeDigit[second % 10]);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeDigit[second / 10]);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeDigit[minute % 10] | TubeCharacter::dot);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeDigit[minute / 10]);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeDigit[hour % 10] | TubeCharacter::dot);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeDigit[hour / 10]);
	digitalWrite(GPIO_LATCH, HIGH);
}