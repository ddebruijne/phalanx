#include "DisplayIV6.h"
#include <SPI.h>
#include "constants.h"

bool DisplayIV6::Initialize()
{
    Serial.println("Initializing IV-6 Display");

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
        displayData[tubeIndex] = B11111110;

    return true;
}

void DisplayIV6::OnTick()
{
    SetPinLow(GPIO_Latch);

    if (ticksSinceLastWrite < currentDimmingStep)
    {
        for(volatile int tubeIndex = 0; tubeIndex < Digits; tubeIndex++)
            SPI.transfer(B00000000);
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

void DisplayIV6::ShiftCurrentTimeFull(int hour, int minute, int second, bool displayZeroFirstDigit)
{
    InternalShiftTimeComponent(second, displayZeroFirstDigit, false);
    InternalShiftTimeComponent(minute, displayZeroFirstDigit, second % 2 == 0);
    InternalShiftTimeComponent(hour, displayZeroFirstDigit, second % 2 == 0);
}

void DisplayIV6::ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit)
{
    // Update time position every hour.
    if (lastHour < 0)
        lastHour = hour;

    if (lastHour != hour)
    {
        lastHour = hour;
        currentTimePosition = (TimePosition)((int)currentTimePosition + 1);
    }

    if (currentTimePosition == TimePosition::MAX)
        currentTimePosition = TimePosition::Middle;

    // Shift Minute and hour.
    
    switch(currentTimePosition)
    {
        default:
        case TimePosition::Middle:
            InternalShiftOut(TubeCharacter::blank);
            InternalShiftTimeComponent(minute, displayZeroFirstDigit, false);
            InternalShiftTimeComponent(hour, displayZeroFirstDigit, second % 2 == 0);
            InternalShiftOut(TubeCharacter::blank);
            break;
        case TimePosition::Left:
            InternalShiftOut(TubeCharacter::blank);
            InternalShiftOut(TubeCharacter::blank);
            InternalShiftTimeComponent(minute, displayZeroFirstDigit, false);
            InternalShiftTimeComponent(hour, displayZeroFirstDigit, second % 2 == 0);
            break;
        case TimePosition::Right:
            InternalShiftTimeComponent(minute, displayZeroFirstDigit, false);
            InternalShiftTimeComponent(hour, displayZeroFirstDigit, second % 2 == 0);
            InternalShiftOut(TubeCharacter::blank);
            InternalShiftOut(TubeCharacter::blank);
            break;
        case TimePosition::Split:
            InternalShiftTimeComponent(minute, displayZeroFirstDigit, false);
            InternalShiftOut(TubeCharacter::blank);
            InternalShiftOut(TubeCharacter::blank);
            InternalShiftTimeComponent(hour, displayZeroFirstDigit, second % 2 == 0);
            break;
    }
}

void DisplayIV6::ShiftRaw(char data[])
{
    for (uint8_t i = 0; i < Digits; i++)
    {
        displayData[i] = data[i];
    }
}

void DisplayIV6::ShiftBlank()
{
    for(int tubeIndex = 0; tubeIndex < Digits; tubeIndex++)
        displayData[tubeIndex] = B0000000;
}

void DisplayIV6::InternalShiftTimeComponent(int number, bool displayZeroFirstDigit, bool dotOnSecondDigit)
{
    if (number > 99 || number < 0)
        number = 0;

    char secondDigit = TubeDigit[number % 10];
    if (dotOnSecondDigit)
        secondDigit = secondDigit | TubeCharacter::dot;
    InternalShiftOut(secondDigit);

    int firstDigit = number / 10;
    if (firstDigit != 0 or displayZeroFirstDigit)
	    InternalShiftOut(TubeDigit[firstDigit]);
    else
	    InternalShiftOut(TubeCharacter::blank);
}

void DisplayIV6::InternalShiftOut(char data)
{
    displayData[shiftOutIndex] = data;
    
    shiftOutIndex++;
    if (shiftOutIndex >= Digits)
        shiftOutIndex = 0;
}