// Phalanx VFD Clock entrypoint
// Copyright (c) Danny de Bruijne 2021. All rights reserved.

/* Resources
	- https://arduino-esp8266.readthedocs.io/en/latest/index.html
	- https://github.com/witnessmenow/spotify-api-arduino
*/

#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <SPI.h>

#include "constants.h"

DeviceMode currentDeviceMode = DeviceMode::Boot;
bool doLoop = false;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

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

	doLoop = startInNormalMode();

	Serial.println("Phalanx Initialized!");
}

bool startInConfigMode()
{
	IPAddress ip = IPAddress(192, 168, 0, 1);
	IPAddress subnetMask = IPAddress(255, 255, 255, 0);
	uint8_t macAddr[6];

	WiFi.softAPmacAddress(macAddr);
	WiFi.softAPConfig(ip, ip, subnetMask);
	WiFi.softAP("Phalanx-IV6");

	digitalWrite(GPIO_LATCH, LOW);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::dot);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::dot);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::F | TubeCharacter::dot);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::n);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::o);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeCharacter::c);
	digitalWrite(GPIO_LATCH, HIGH);

	currentDeviceMode = DeviceMode::Config;

	return false;
}

bool startInNormalMode()
{
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

	timeClient.begin();
	timeClient.setTimeOffset(0);
	timeClient.update();
	delay(1000);

	currentDeviceMode = DeviceMode::Normal;

	return true;
}

void loop()
{
	if(!doLoop) {
		delay(1);
		return;
	}

	shiftCurrentTime();
	delay(100);
}

void shiftCurrentTime()
{
	int hour = timeClient.getHours();
	int minute = timeClient.getMinutes();
	int second = timeClient.getSeconds();

	digitalWrite(GPIO_LATCH, LOW);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeDigit[second % 10]);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeDigit[second / 10]);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeDigit[minute % 10] | TubeCharacter::dot);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeDigit[minute / 10]);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeDigit[hour % 10] | TubeCharacter::dot);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, TubeDigit[hour / 10]);
	digitalWrite(GPIO_LATCH, HIGH);
}