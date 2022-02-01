#include "DisplayIV12.h"
#include <SPI.h>
#include "constants.h"

bool DisplayIV12::Initialize()
{
    Serial.println("Initializing IV-12 Display");

	pinMode(GPIO_Data, OUTPUT);
	pinMode(GPIO_Clock, OUTPUT);
	pinMode(GPIO_Latch, OUTPUT);
    SetPinLow(GPIO_Data);
    SetPinLow(GPIO_Clock);
    SetPinHigh(GPIO_Latch);

    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV16);

    if (currentDimmingStep > MaxDimmingSteps)
        currentDimmingStep = MaxDimmingSteps;

    for(int tubeIndex = 0; tubeIndex < Digits; tubeIndex++)
        displayData[tubeIndex] = 0b11111111;

    return true;
}

void DisplayIV12::OnTick()
{
    SetPinLow(GPIO_Latch);

    if (ticksSinceLastWrite < currentDimmingStep)
    {
        for(volatile int tubeIndex = 0; tubeIndex < Digits; tubeIndex++)
            SPI.transfer(0b00000000);
    }
    else
    {
        for(volatile int tubeIndex = 0; tubeIndex < Digits; tubeIndex++)
            SPI.transfer(displayData[tubeIndex]);
    }

    SetPinHigh(GPIO_Latch);

    ticksSinceLastWrite++;
    if (ticksSinceLastWrite > currentDimmingStep)
        ticksSinceLastWrite = 0;

}

void DisplayIV12::ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit)
{
	hour = constrain(hour, 0, 99);
	minute = constrain(minute, 0, 99);

	// Minute: 3rd and 4th tube
	displayData[3] = TubeDigit[minute % 10];
	int firstDigit = minute / 10;
	if (firstDigit != 0 or displayZeroFirstDigit)
		displayData[2] = TubeDigit[firstDigit];
	else
		displayData[2] = CharMap[' '];

	// Hour: 1st and 2nd tube
	displayData[1] = TubeDigit[hour % 10];
	firstDigit = hour / 10;
	if (firstDigit != 0 or displayZeroFirstDigit)
		displayData[0] = TubeDigit[firstDigit];
	else
		displayData[0] = CharMap[' '];
}

void DisplayIV12::ShiftRaw(byte data[])
{
    for (size_t i = 0; i < Digits; i++)
        displayData[i] = data[i];
}

void DisplayIV12::ShiftText(String text)
{
    ShiftBlank();

    for(size_t tubeIndex = 0; tubeIndex < Digits; tubeIndex++) 
    {
        // eg TubeIndex 1: 6 - 1 - 1 = 4;
        // eg TubeIndex 5 = 6 - 1 - 5 = 0;
        size_t reverse = Digits - 1 - tubeIndex;

        // leave blanks for extended ascii table.
        if (text[tubeIndex] >= sizeof(CharMap))
            continue;

        displayData[reverse] = CharMap[text[tubeIndex]];
    }
}

void DisplayIV12::ShiftBlank()
{
    for (size_t tubeIndex = 0; tubeIndex < Digits; tubeIndex++)
        displayData[tubeIndex] = 0b0000000;
}