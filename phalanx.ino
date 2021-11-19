// Phalanx VFD Clock entrypoint
// Copyright (c) Danny de Bruijne 2021. All rights reserved.

/* Resources
	- https://arduino-esp8266.readthedocs.io/en/latest/index.html
	- https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html
	- https://github.com/jwrw/ESP_EEPROM/blob/master/examples/ESP_EEPROM_Simple/ESP_EEPROM_Simple.ino
	- https://github.com/witnessmenow/spotify-api-arduino
*/

/* Todo:
	- If using HR:MIN mode for time, alternate tubes we display on
	- brightness controls
	- timezone storage, DST handling
	- date display (at 30s)
	- spotify device mode & mode switching
	- defining & writing string to display
	- OTA updates
*/

#define DISPLAYTYPE_IV6
#define PORT 80

#ifdef DISPLAYTYPE_IV6
#include "DisplayIV6.h"
DisplayIV6 display;
#endif //DISPLAYTYPE_IV6

#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> 
#include <ESP_EEPROM.h>

#include "EEPROMData.h"
#include "constants.h"
#include "DeviceMode.h"
#include "DeviceModeConfig.h"
#include "DeviceModeNormal.h"

DeviceMode *deviceMode;
ESP8266WebServer webServer(PORT);
EEPROMData saveData;
bool doLoop = false;

void handleRoot()
{
	String str = "<!DOCTYPE html><html><head><title>Phalanx</title></head>";
	str += "<body><h1>Phalanx Config</h1>";
	str += "Current Connection Status: ";
	str += WifiStatusCode[WiFi.status()];

	str += "<br/><form action=\"/save\" method=\"POST\">WiFi SSID: <input type=\"text\" name=\"ssid\" maxLength=32 placeholder=\"WiFi SSID\" value=\"";
	str += String(saveData.wifi_ssid);

	str += "\"></br>WiFi Passphrase: <input type=\"text\" name=\"password\" maxLength=32 placeholder=\"WiFi Passphrase\" value=\"";
	str += String(saveData.wifi_pass);

	str += "\"></br>12h mode: <input type=\"checkbox\" id=\"time_12hmode\" name=\"time_12hmode\" ";
	if (saveData.time_12hmode)
		str += "checked";
	
	str += "></br>Display 0 when it's the first digit: <input type=\"checkbox\" id=\"time_zerofirstdigit\" name=\"time_zerofirstdigit\" ";
	if (saveData.time_displayZeroFirstDigit)
		str += "checked";
	
	str += "></br>Display seconds: <input type=\"checkbox\" id=\"time_displaySeconds\" name=\"time_displaySeconds\" ";
	if (saveData.time_displaySeconds)
		str += "checked";
	
	str += "></br></p><input type=\"submit\" value=\"Save\"></form><br/><br/>";
	str += "</body></html>";

	webServer.send(200, "text/html", str.c_str());
}

void handleSave()
{
	if (webServer.hasArg("ssid") && !webServer.arg("ssid").isEmpty())
		strncpy(saveData.wifi_ssid, webServer.arg("ssid").c_str(), 32);

	if (webServer.hasArg("password"))
		strncpy(saveData.wifi_pass, webServer.arg("password").c_str(), 32);

	saveData.time_12hmode = webServer.hasArg("time_12hmode");
	saveData.time_displayZeroFirstDigit = webServer.hasArg("time_zerofirstdigit");
	saveData.time_displaySeconds = webServer.hasArg("time_displaySeconds");

	Serial.printf("Saved data: ssid:%s / pw:%s\n", saveData.wifi_ssid, saveData.wifi_pass);
	saveData.initialized = true;
	EEPROM.put(0, saveData);
	EEPROM.commit();

	webServer.sendHeader("Location","/"); 
	webServer.send(303);
}

void setup()
{
	Serial.begin(115200);	
	Serial.println("Phalanx is initializing...");

	display.Initialize();
	EEPROM.begin(sizeof(EEPROMData));
	EEPROM.get(0, saveData);

	// Determine device mode and start it.
	// Go to config mode if not initialized or wlan connection fails.
	// If a successful internet connection is established, enable the normal device mode
	if (saveData.initialized == false)
		deviceMode = new DeviceModeConfig();
	else
	{
		char data[6] = {
			TubeCharacter::dot,
			TubeCharacter::dot,
			TubeCharacter::n | TubeCharacter::dot,
			TubeCharacter::n,
			TubeCharacter::o,
			TubeCharacter::c,
		};
		display.ShiftRaw(data);

		bool success = attemptConnectWLAN();
		if (!success)
			deviceMode = new DeviceModeConfig();
		else
			deviceMode = new DeviceModeNormal();
	}

	deviceMode->SetDisplay(&display);
	doLoop = deviceMode->Start();

	// Setup network presentation and web server.
	MDNS.begin("Phalanx");

	webServer.on("/", HTTP_GET, handleRoot);
	webServer.on("/save", HTTP_POST, handleSave);
	webServer.onNotFound([](){
		webServer.send(404, "text/plain", "404: Not found");
	});
	webServer.begin();

	MDNS.addService("http", "tcp", PORT);

	Serial.println("Phalanx Initialized!");
}

bool attemptConnectWLAN()
{
	WiFi.begin(String(saveData.wifi_ssid), String(saveData.wifi_pass));
	uint8_t timeOutSeconds = 10;
	uint8_t currentTimeOutSeconds = 0;

	// Waits for 10 seconds or until the device is connected.
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1000);
		currentTimeOutSeconds++;

		if (currentTimeOutSeconds >= timeOutSeconds)
			break;
	}

	return WiFi.status() == WL_CONNECTED;
}

void loop()
{
	if(!doLoop) {
		delay(1);
		return;
	}

	MDNS.update();
	webServer.handleClient();

	if(deviceMode != nullptr)
		deviceMode->OnTick();
}