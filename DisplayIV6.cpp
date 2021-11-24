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

    //TODO don't shift blanking after 1st tick.
    //TODO if dimming step = 0 only write if new data;
    //TODO if dimming step = max dimming step then treat as off.
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
//  REMOVEME
    ShiftBlank();

	// digitalWrite(GPIO_Latch, LOW);

    // InternalShiftTimeComponent(second, displayZeroFirstDigit, false);
    // InternalShiftTimeComponent(minute, displayZeroFirstDigit, second % 2 != 0);
    // InternalShiftTimeComponent(hour, displayZeroFirstDigit, second % 2 == 0);

	// digitalWrite(GPIO_Latch, HIGH);
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

//  REMOVEME
    ShiftBlank();

    // // Shift Minute and hour.
	// digitalWrite(GPIO_Latch, LOW);
    
    // switch(currentTimePosition)
    // {
    //     default:
    //     case TimePosition::Middle:
    //         shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeCharacter::blank);
    //         InternalShiftTimeComponent(minute, displayZeroFirstDigit, false);
    //         InternalShiftTimeComponent(hour, displayZeroFirstDigit, second % 2 == 0);
    //         shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeCharacter::blank);
    //         break;
    //     case TimePosition::Left:
    //         shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeCharacter::blank);
    //         shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeCharacter::blank);
    //         InternalShiftTimeComponent(minute, displayZeroFirstDigit, false);
    //         InternalShiftTimeComponent(hour, displayZeroFirstDigit, second % 2 == 0);
    //         break;
    //     case TimePosition::Right:
    //         InternalShiftTimeComponent(minute, displayZeroFirstDigit, false);
    //         InternalShiftTimeComponent(hour, displayZeroFirstDigit, second % 2 == 0);
    //         shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeCharacter::blank);
    //         shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeCharacter::blank);
    //         break;
    //     case TimePosition::Split:
    //         InternalShiftTimeComponent(minute, displayZeroFirstDigit, false);
    //         shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeCharacter::blank);
    //         shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeCharacter::blank);
    //         InternalShiftTimeComponent(hour, displayZeroFirstDigit, second % 2 == 0);
    //         break;
    // }

	// digitalWrite(GPIO_Latch, HIGH);
}

void DisplayIV6::ShiftRaw(char data[])
{
    for (uint8_t i = 0; i < Digits; i++)
    {
        displayData[i] = data[i];
    }

	// digitalWrite(GPIO_Latch, LOW);
    // for (uint8_t i = 0; i < Digits; i++)
    // {
    //     shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, data[i]);
    // }
    // digitalWrite(GPIO_Latch, HIGH);
}

void DisplayIV6::ShiftBlank()
{
    for(int tubeIndex = 0; tubeIndex < Digits; tubeIndex++)
        displayData[tubeIndex] = B11111110;
    //TODO make B0000000

   	// digitalWrite(GPIO_Latch, LOW);
    // for (uint8_t i = 0; i < Digits; i++)
    // {
    //     shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeCharacter::blank);
    // }
    // digitalWrite(GPIO_Latch, HIGH); 
}

void DisplayIV6::InternalShiftTimeComponent(int number, bool displayZeroFirstDigit, bool dotOnSecondDigit)
{
    // if (number > 99 || number < 0)
    //     number = 0;

    // char secondDigit = TubeDigit[number % 10];
    // if (dotOnSecondDigit)
    //     secondDigit = secondDigit | TubeCharacter::dot;
	// shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, secondDigit);

    // int firstDigit = number / 10;
    // if (firstDigit != 0 or displayZeroFirstDigit)
	//     shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeDigit[firstDigit]);
    // else
	//     shiftOut(GPIO_Data, GPIO_Clock, MSBFIRST, TubeCharacter::blank);
}