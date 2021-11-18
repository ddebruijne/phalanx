// Phalanx VFD Clock entrypoint
// Copyright (c) Danny de Bruijne 2021. All rights reserved.

/* Resources
	- https://arduino-esp8266.readthedocs.io/en/latest/index.html
	- https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html
	- https://github.com/witnessmenow/spotify-api-arduino
*/

#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> 

#include "constants.h"
#include "DeviceMode.h"
#include "DeviceModeConfig.h"
#include "DeviceModeNormal.h"

DeviceMode *deviceMode;
ESP8266WebServer webServer(80);
bool doLoop = false;

void handleRoot()
{
	webServer.send(200, "text/plain", "Hello world!");
}

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

	MDNS.begin("Phalanx");

	webServer.on("/", handleRoot);
	webServer.onNotFound([](){
		webServer.send(404, "text/plain", "404: Not found");
	});
	webServer.begin();

	Serial.println("Phalanx Initialized!");
}

void loop()
{
	if(!doLoop) {
		delay(1);
		return;
	}

	webServer.handleClient();

	if(deviceMode != nullptr)
		deviceMode->OnTick();

	delay(1);
}