// Phalanx VFD Clock entrypoint
// Copyright (c) Danny de Bruijne 2021. All rights reserved.

/* Resources
	- https://arduino-esp8266.readthedocs.io/en/latest/index.html
	- https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html
	- https://github.com/witnessmenow/spotify-api-arduino
*/

#define DISPLAYTYPE_IV6

#ifdef DISPLAYTYPE_IV6
#include "DisplayIV6.h"
DisplayIV6 display;
#endif //DISPLAYTYPE_IV6

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
	Serial.begin(115200);	
	Serial.println("Phalanx is initializing...");

	display.Initialize();

	deviceMode = new DeviceModeNormal();
	deviceMode->SetDisplay(&display);
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
}