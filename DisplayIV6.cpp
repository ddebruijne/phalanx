#include "DisplayIV6.h"
#include <SPI.h>
#include "constants.h"

bool DisplayIV6::Initialize()
{
    Serial.println("Initializing IV-6 Display");

	pinMode(GPIO_Data, OUTPUT);
	pinMode(GPIO_Clock, OUTPUT);
	pinMode(GPIO_Latch, OUTPUT);

	digitalWrite(GPIO_Data, LOW);
	digitalWrite(GPIO_Clock, LOW);
	digitalWrite(GPIO_Latch, HIGH);

    return true;
}

void DisplayIV6::ShiftCurrentTime(int hour, int minute, int second)
{
	digitalWrite(GPIO_Latch, LOW);
	shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeDigit[second % 10]);
	shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeDigit[second / 10]);
	shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeDigit[minute % 10] | TubeCharacter::dot);
	shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeDigit[minute / 10]);
	shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeDigit[hour % 10] | TubeCharacter::dot);
	shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeDigit[hour / 10]);
	digitalWrite(GPIO_Latch, HIGH);
}

void DisplayIV6::ShiftRaw(char data[])
{
	digitalWrite(GPIO_Latch, LOW);
    for (uint8_t i = 0; i < 6; i++)
    {
        shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, data[i]);
    }
    digitalWrite(GPIO_Latch, HIGH);
}