// Phalanx VFD Clock entrypoint
// Copyright (c) Danny de Bruijne 2021. All rights reserved.

/* Resources
	- https://arduino-esp8266.readthedocs.io/en/latest/index.html
	- https://github.com/witnessmenow/spotify-api-arduino
*/

#include "constants.h"
#include "DeviceMode.h"
#include "DeviceModeConfig.h"
#include "DeviceModeNormal.h"

DeviceMode *deviceMode;
bool doLoop = false;

void setup()
{
	pinMode(GPIO_DATA, OUTPUT);
	pinMode(GPIO_CLOCK, OUTPUT);
	pinMode(GPIO_LATCH, OUTPUT);

	digitalWrite(GPIO_DATA, LOW);
	digitalWrite(GPIO_CLOCK, LOW);
	digitalWrite(GPIO_LATCH, HIGH);

	Serial.begin(115200);
	Serial.println("Phalanx is initializing...");

	deviceMode = new DeviceModeNormal();
	doLoop = deviceMode->Start();

	Serial.println("Phalanx Initialized!");
}

void loop()
{
	if(!doLoop) {
		delay(1);
		return;
	}

	if(deviceMode != nullptr)
		deviceMode->OnTick();

	delay(1);
}