#include "DeviceModeNormal.h"
#include "constants.h"

bool DeviceModeNormal::Start()
{
	EEPROM.get(0, saveData);

    timeClient = new NTPClient(ntpUDP, "pool.ntp.org", 0, 1800000); // 30 min
	timeClient->begin();

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

	Timezone* tz = Timezones[saveData.timeZone].timezone;
	time_t localizedTime = tz->toLocal(timeClient->getEpochTime());

	int hours = hour(localizedTime);
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
			display->ShiftCurrentTimeFull(hours, minute(localizedTime), second(localizedTime), saveData.time_displayZeroFirstDigit);
		else
			display->ShiftCurrentTime(hours, minute(localizedTime), second(localizedTime), saveData.time_displayZeroFirstDigit);
	}

	timeSinceLastSettingsSync += delayBetweenTicks;
	if(timeSinceLastSettingsSync >= delayBetweenSettingsSync)
	{
		EEPROM.get(0, saveData);
		timeSinceLastSettingsSync = 0;
	}
}