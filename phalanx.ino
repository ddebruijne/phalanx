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

#include <SPI.h>

#include "constants.h"

byte mac[6];
IPAddress apIp(8, 8, 8, 8);
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
	Serial.println("");
	WiFi.macAddress(mac);
	WiFi.mode(WIFI_AP);

	WiFi.softAPConfig(apIp, apIp, IPAddress(255, 255, 255, 0));
	WiFi.softAP("ESP M2");
	delay(100);

	Serial.println("Hola!");

	doLoop = true;
}

void loop()
{
	if(!doLoop) return;
//   for (int j = 0; j < 256; j++) {
//     //ground latchPin and hold low for as long as you are transmitting
//     digitalWrite(GPIO_LATCH, LOW);
//     shiftOut(GPIO_DATA, GPIO_CLOCK, LSBFIRST, j);
//     //return the latch pin high to signal chip that it
//     //no longer needs to listen for information
//     digitalWrite(GPIO_LATCH, HIGH);
//     delay(1000);
//   }

	digitalWrite(GPIO_LATCH, LOW);
	for (int i = 0; i < 5; i++)
		shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, letters::h);
	digitalWrite(GPIO_LATCH, HIGH);
	delay(400);

	digitalWrite(GPIO_LATCH, LOW);
	for (int i = 0; i < 5; i++)
		shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, letters::E);
	digitalWrite(GPIO_LATCH, HIGH);
	delay(400);

	digitalWrite(GPIO_LATCH, LOW);
	for (int i = 0; i < 5; i++)
		shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, letters::L);
	digitalWrite(GPIO_LATCH, HIGH);
	delay(400);

	digitalWrite(GPIO_LATCH, LOW);
	for (int i = 0; i < 5; i++)
		shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, letters::l);
	digitalWrite(GPIO_LATCH, HIGH);
	delay(400);

	digitalWrite(GPIO_LATCH, LOW);
	for (int i = 0; i < 5; i++)
		shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, letters::o);
	digitalWrite(GPIO_LATCH, HIGH);
	delay(400);

	digitalWrite(GPIO_LATCH, LOW);
	for (int i = 0; i < 5; i++)
		shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, letters::blank);
	digitalWrite(GPIO_LATCH, HIGH);
	delay(400);

	for (int number = 0; number < 10; number++)
	{
		digitalWrite(GPIO_LATCH, LOW);
		for (int tube = 0; tube < 6; tube++)
		{
			if(tube == 5)
				shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, bnum[number]);
			else
				shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, bnum[11]);
			
		}
		digitalWrite(GPIO_LATCH, HIGH);
		Serial.println(number);
		delay(400);
	}

	digitalWrite(GPIO_LATCH, LOW);
	for (int i = 0; i < 5; i++)
		shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, letters::dot);
	digitalWrite(GPIO_LATCH, HIGH);
	delay(400);
}

// void initVFD()
// {
// 	Serial.println("Initializing SPI interface for communicating with shift registers...");
// 	SPI.begin();
// 	SPI.setDataMode(SPI_MODE0);
// 	SPI.setBitOrder(MSBFIRST);
// 	SPI.setClockDivider(SPI_CLOCK_DIV16);
// }

void sendNumber(uint8_t number)
{
	for (int i = 0; i < 8; i++)
	{
		//SPI.transfer(Constants::numbers[number][i]);
	}
}