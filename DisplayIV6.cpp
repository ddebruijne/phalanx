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
    int sec_firstDigit = second / 10;
    if (sec_firstDigit != 0) 
	    shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeDigit[sec_firstDigit]);
    else
	    shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeCharacter::blank);

	shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeDigit[minute % 10] | TubeCharacter::dot);
    int min_firstDigit = minute / 10;
    if (min_firstDigit != 0)
	    shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeDigit[min_firstDigit]);
    else
        shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeCharacter::blank);

	shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeDigit[hour % 10] | TubeCharacter::dot);
    int hr_firstDigit = hour / 10;
    if (hr_firstDigit != 0)
	    shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeDigit[hr_firstDigit]);
    else
	    shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeCharacter::blank);

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