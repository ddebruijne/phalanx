#include "DeviceModeNormal.h"
#include <ESP8266WiFi.h>
#include "constants.h"

bool DeviceModeNormal::Start()
{
	EEPROM.get(0, saveData);

    timeClient = new NTPClient(ntpUDP, "pool.ntp.org");

	timeClient->begin();
	timeClient->setUpdateInterval(1800000); // 30 min
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
	int hours = timeClient->getHours();

	if (saveData.activeHours[0] != saveData.activeHours[1] && (hours < saveData.activeHours[0] || hours >= saveData.activeHours[1]))
	{
		display->ShiftBlank();
	}
	else
	{
		// modify hours for 12h mode if needed, don't do operations based around 24h clock after here.
		if (saveData.time_12hmode && hours > 12)
			hours -= 12;

		if (saveData.time_displaySeconds)
			display->ShiftCurrentTimeFull(hours, timeClient->getMinutes(), timeClient->getSeconds(), saveData.time_displayZeroFirstDigit);
		else
			display->ShiftCurrentTime(hours, timeClient->getMinutes(), timeClient->getSeconds(), saveData.time_displayZeroFirstDigit);
	}

	delay(delayBetweenTicks);
	timeSinceLastSettingsSync += delayBetweenTicks;

	if(timeSinceLastSettingsSync >= delayBetweenSettingsSync)
	{
		EEPROM.get(0, saveData);
		timeSinceLastSettingsSync = 0;
	}
}