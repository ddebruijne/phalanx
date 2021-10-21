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
	
	//max pins to have high at the same time now is 19 before voltage drops under 3v.
	digitalWrite(GPIO_LATCH, LOW);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00001101);
	digitalWrite(GPIO_LATCH, HIGH);

	delay(1000);
	Serial.println("Hola!");
	shiftVoltageTest();
	//doLoop = true;
}

void loop()
{
	if(!doLoop) return;

	loopDemo();
}

void loopDemo()
{
	digitalWrite(GPIO_LATCH, LOW);
	for (int i = 0; i < 5; i++)
		shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, letters::h);
	digitalWrite(GPIO_LATCH, HIGH);
	Serial.println("h");
	delay(400);

	digitalWrite(GPIO_LATCH, LOW);
	for (int i = 0; i < 5; i++)
		shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, letters::E);
	digitalWrite(GPIO_LATCH, HIGH);
	Serial.println("E");
	delay(400);

	digitalWrite(GPIO_LATCH, LOW);
	for (int i = 0; i < 5; i++)
		shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, letters::L);
	digitalWrite(GPIO_LATCH, HIGH);
	Serial.println("L");
	delay(400);

	digitalWrite(GPIO_LATCH, LOW);
	for (int i = 0; i < 5; i++)
		shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, letters::l);
	digitalWrite(GPIO_LATCH, HIGH);
	Serial.println("l");
	delay(400);

	digitalWrite(GPIO_LATCH, LOW);
	for (int i = 0; i < 5; i++)
		shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, letters::o);
	digitalWrite(GPIO_LATCH, HIGH);
	Serial.println("o");
	delay(400);

	digitalWrite(GPIO_LATCH, LOW);
	for (int i = 0; i < 5; i++)
		shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, letters::blank);
	digitalWrite(GPIO_LATCH, HIGH);
	Serial.println("blank");
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
	Serial.println(".");
	delay(400);
}

void loopFullAllTubes()
{
	digitalWrite(GPIO_LATCH, LOW);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B11111111);	// leftmost tube seems to bring a voltage drop?
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	digitalWrite(GPIO_LATCH, HIGH);
	Serial.println("6");
	delay(2000);

	digitalWrite(GPIO_LATCH, LOW);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B11111111);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	digitalWrite(GPIO_LATCH, HIGH);
	Serial.println("5");
	delay(2000);

	digitalWrite(GPIO_LATCH, LOW);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B11111111);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	digitalWrite(GPIO_LATCH, HIGH);
	Serial.println("4");
	delay(2000);

	digitalWrite(GPIO_LATCH, LOW);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B11111111);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	digitalWrite(GPIO_LATCH, HIGH);
	Serial.println("3");
	delay(2000);

	digitalWrite(GPIO_LATCH, LOW);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B11111111);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	digitalWrite(GPIO_LATCH, HIGH);
	Serial.println("2");
	delay(2000);

	digitalWrite(GPIO_LATCH, LOW);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B11111111);
	digitalWrite(GPIO_LATCH, HIGH);
	Serial.println("1");
	delay(2000);
}

void shiftVoltageTest()
{
	// This should give us a voltage right around 3,05-10v. Enough to test so we can debug the hardware and still flash the microcontroller without having to resort to hardware mods.
	digitalWrite(GPIO_LATCH, LOW);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000000);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B11111111);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000001);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B00000011);
	shiftOut(GPIO_DATA, GPIO_CLOCK, MSBFIRST, B11111111);
	digitalWrite(GPIO_LATCH, HIGH);
}