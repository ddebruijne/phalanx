// Phalanx VFD Clock entrypoint
// Copyright 2021 Danny de Bruijne. Follows MIT license (see Readme)

#define DISPLAYTYPE_IV6
#define PORT 80
#define _TIMERINTERRUPT_LOGLEVEL_     0
#define DEBUGMODE 0

#ifdef DISPLAYTYPE_IV6
#include "DisplayIV6.h"
DisplayIV6 display;
#endif //DISPLAYTYPE_IV6

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> 
#include <ESP_EEPROM.h>
#include <ESP8266TimerInterrupt.h>
#include <SpotifyArduino.h>

#include "EEPROMData.h"
#include "constants.h"
#include "DeviceMode.h"
#include "DeviceModeConfig.h"
#include "DeviceModeNormal.h"
#include "DeviceModeSpotify.h"

DeviceMode *deviceMode;
ESP8266WebServer webServer(PORT);
EEPROMData saveData;
ESP8266Timer displayTimer;
WiFiClientSecure spotifyWebClient;
SpotifyArduino *spotify;
bool doTick = false;

void handleRoot()
{
	EEPROM.get(0, saveData);
	String hourDropdown = "<option value=\"\">-</option><option value = \"0\">0</option><option value = \"1\">1</option><option value = \"2\">2</option><option value = \"3\">3</option><option value = \"4\">4</option><option value = \"5\">5</option><option value = \"6\">6</option><option value = \"7\">7</option><option value = \"8\">8</option><option value = \"9\">9</option><option value = \"10\">10</option><option value = \"11\">11</option><option value = \"12\">13</option><option value = \"14\">14</option><option value = \"15\">15</option><option value = \"16\">16</option><option value = \"17\">17</option><option value = \"18\">18</option><option value = \"19\">19</option><option value = \"20\">20</option><option value = \"21\">21</option><option value = \"22\">22</option><option value = \"23\">23</option><option value = \"24\">24</option>";

	String str = "<!DOCTYPE html><html><head><title>Phalanx</title><style>body {font: normal 12px Verdana, Arial, sans-serif;}</style></head>";
	str.reserve(3500);	//3.5kb
	str += "<body><h1>Phalanx Config</h1>";
	str += "Current Connection Status: ";
	str += WifiStatusCode[WiFi.status()];

#if DEBUGMODE != 0
	str += "<br/>Spotify Refresh Token: ";
	str += saveData.spotifyRefreshToken;
#endif

	str += "<br/><h2>WiFi</h2>";
	str += "<form action=\"/save\" method=\"POST\">WiFi SSID: <input type=\"text\" name=\"ssid\" maxLength=32 placeholder=\"WiFi SSID\" value=\"";
	str += String(saveData.wifi_ssid);

	str += "\"><br/>WiFi Passphrase: <input type=\"text\" name=\"password\" maxLength=32 placeholder=\"WiFi Passphrase\" value=\"";
	str += String(saveData.wifi_pass);

	str += "\"><br/><h2>Tube Settings</h2>";
	str += "Dimming: <select name=\"dimming\">";
	for (int i = 0; i < display.MaxDimmingSteps; i++)
	{
		str += "<option value=\"" + String(i) + "\"";
		if (saveData.dimmingStep == i)
			str += " selected";
		str += ">" + String(i) + "</option>";
	}
	str += "</select><br/>";

	str += "<h2>Time Settings</h2>";
	str += "Timezone: <select name=\"timezone\"><option value=\"\">-</option><option value=\"0\">UTC</option><option value=\"1\">Britain, Portugal</option><option value=\"2\">Central Europe</option><option value=\"3\">Moscow</option><option value=\"4\">Australia Eastern</option><option value=\"5\">America Eastern</option></select>";

	str += "</br>12h mode: <input type=\"checkbox\" id=\"time_12hmode\" name=\"time_12hmode\" ";
	if (saveData.time_12hmode)
		str += "checked";
	
	str += "></br>Display 0 when it's the first digit: <input type=\"checkbox\" id=\"time_zerofirstdigit\" name=\"time_zerofirstdigit\" ";
	if (saveData.time_displayZeroFirstDigit)
		str += "checked";
	
	str += "></br>Display seconds: <input type=\"checkbox\" id=\"time_displaySeconds\" name=\"time_displaySeconds\" ";
	if (saveData.time_displaySeconds)
		str += "checked";

	str += "><br>Active Hours: <select name=\"activehrs_begin\">" + hourDropdown + "</select><select name=\"activehrs_end\">" + hourDropdown + "</select>";
	str += " Set to: " + String(saveData.activeHours[0]) + "-" + String(saveData.activeHours[1]) + ". Equal numbers mean always active.";

	str += "<br/><br/><input type=\"submit\" value=\"Save\"></form><br/><br/><br/>";

	if (deviceMode->GetDeviceMode() != EDeviceMode::Config)
	{
		String authUri;
		SpotifyApiConstants::GetAuthUri(authUri);
		str += "<p><a href=\"" + authUri + "\">Attempt Spotify Auth</a></p>";

		// We only have 2 modes, normal is the default. Don't bother switching if we don't have a refresh token.
		if (strlen(saveData.spotifyRefreshToken) > 5)
			str += "<p><a href=\"/toggleMode\">Toggle Device Mode</a> - Current Device Mode: " + DeviceModeString[deviceMode->GetDeviceMode()] + "</p>";
	}

	str += "</body></html>";

	webServer.send(200, "text/html", str.c_str());
}

void handleSave()
{
	if (webServer.hasArg("ssid") && !webServer.arg("ssid").isEmpty())
		strncpy(saveData.wifi_ssid, webServer.arg("ssid").c_str(), 32);

	if (webServer.hasArg("password"))
		strncpy(saveData.wifi_pass, webServer.arg("password").c_str(), 32);

	if (webServer.hasArg("dimming") && !webServer.arg("dimming").isEmpty())
		saveData.dimmingStep = webServer.arg("dimming").toInt();

	if (webServer.hasArg("timezone") && !webServer.arg("timezone").isEmpty())
		saveData.timeZone = webServer.arg("timezone").toInt();

	saveData.time_12hmode = webServer.hasArg("time_12hmode");
	saveData.time_displayZeroFirstDigit = webServer.hasArg("time_zerofirstdigit");
	saveData.time_displaySeconds = webServer.hasArg("time_displaySeconds");

	// Active Hours
	if (webServer.hasArg("activehrs_begin") && !webServer.arg("activehrs_begin").isEmpty())
		saveData.activeHours[0] = webServer.arg("activehrs_begin").toInt();
		
	if (webServer.hasArg("activehrs_end") && !webServer.arg("activehrs_end").isEmpty())
		saveData.activeHours[1] = webServer.arg("activehrs_end").toInt();

	if (saveData.activeHours[0] > saveData.activeHours[1])
		saveData.activeHours[0] = saveData.activeHours[1];
	if (saveData.activeHours[1] < saveData.activeHours[0])
		saveData.activeHours[1] = saveData.activeHours[0];

	Serial.printf("Saved data: ssid:%s / pw:%s\n", saveData.wifi_ssid, saveData.wifi_pass);
	saveData.initialized = true;
	EEPROM.put(0, saveData);
	EEPROM.commit();

	display.SetDimmingStep(saveData.dimmingStep);

	webServer.sendHeader("Location","/"); 
	webServer.send(303);
}

void handleSpotifyAuth()
{
	const char *refreshToken = NULL;
	for (uint8_t i = 0; i < webServer.args(); i++)
		if (webServer.argName(i) == "code")
			refreshToken = spotify->requestAccessTokens(webServer.arg(i).c_str(), SpotifyApiConstants::CallbackUri);	// yeah, this actually returns the refresh token!

	if (refreshToken == NULL)
	{
		webServer.send(404, "text/plain", "Failed to load token, check serial monitor");
		return;
	}

	strcpy(saveData.spotifyRefreshToken, refreshToken);
	saveData.initialized = true;
	EEPROM.put(0, saveData);
	EEPROM.commit();

	webServer.sendHeader("Location","/"); 
	webServer.send(303);
}

void handleToggleMode()
{
	toggleDeviceMode();
	webServer.sendHeader("Location","/"); 
	webServer.send(303);
}

void IRAM_ATTR displayTimerHandler()
{
	if (!display.RequiresTimer)
		return;

	display.OnTick();
}

void setup()
{
	Serial.begin(115200);
	while (!Serial) 
		return;

	Serial.println("Phalanx is initializing...");

	// Load storage
	EEPROM.begin(sizeof(EEPROMData));
	EEPROM.get(0, saveData);

	// Initialize display & display timer
	display.SetDimmingStep(saveData.dimmingStep);
	display.Initialize();
	if (display.RequiresTimer)
	{
		Serial.println("Display requires timer - starting...");
		displayTimer.attachInterruptInterval(display.TimerIntervalUs, displayTimerHandler);
	}

	// Determine device mode and start it.
	// Go to config mode if not initialized or wlan connection fails.
	// If a successful internet connection is established, enable the normal device mode
	if (saveData.initialized == false)
	{
		saveData = EEPROMData();
		deviceMode = new DeviceModeConfig();
	}
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

		spotifyWebClient.setFingerprint(SPOTIFY_FINGERPRINT);
		if (strlen(saveData.spotifyRefreshToken) > 5)
			spotify = new SpotifyArduino(spotifyWebClient, SpotifyClientId, SpotifyClientSecret, saveData.spotifyRefreshToken);
		else
			spotify = new SpotifyArduino(spotifyWebClient, SpotifyClientId, SpotifyClientSecret);
	}

	deviceMode->SetDisplay(&display);
	doTick = deviceMode->Start();

	// Setup network presentation and web server.
	MDNS.begin("Phalanx");

	webServer.on("/", HTTP_GET, handleRoot);
	webServer.on("/save", HTTP_POST, handleSave);
	webServer.on("/SpotifyCallback", handleSpotifyAuth);
	webServer.on("/toggleMode", handleToggleMode);
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

void toggleDeviceMode()
{
	// Destroy old mode
	EDeviceMode oldDeviceMode = deviceMode->GetDeviceMode();
	doTick = false;
	deviceMode->Stop();
	delete deviceMode;

	// Create new mode
	switch(oldDeviceMode)
	{
		default:
		case EDeviceMode::Normal:
		{
			deviceMode = new DeviceModeSpotify();
			DeviceModeSpotify* dms = (DeviceModeSpotify*)deviceMode;
			dms->spotify = spotify;
			break;
		}
		case EDeviceMode::Spotify:
		{
			deviceMode = new DeviceModeNormal();
			break;
		}
	}

	deviceMode->SetDisplay(&display);
	doTick = deviceMode->Start();
}

void loop()
{
	// Always update web services
	MDNS.update();
	webServer.handleClient();

	// Device Mode Update Loop
	if(!doTick) {
		delay(1);
		return;
	}

	if(deviceMode != nullptr)
		deviceMode->OnTick();
}