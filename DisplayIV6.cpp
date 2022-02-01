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
        displayData[tubeIndex] = 0b11111110;

    return true;
}

void DisplayIV6::OnTick()
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
            InternalShiftOut(CharMap[' ']);
            InternalShiftTimeComponent(minute, displayZeroFirstDigit, false);
            InternalShiftTimeComponent(hour, displayZeroFirstDigit, second % 2 == 0);
            InternalShiftOut(CharMap[' ']);
            break;
        case TimePosition::Left:
            InternalShiftOut(CharMap[' ']);
            InternalShiftOut(CharMap[' ']);
            InternalShiftTimeComponent(minute, displayZeroFirstDigit, false);
            InternalShiftTimeComponent(hour, displayZeroFirstDigit, second % 2 == 0);
            break;
        case TimePosition::Right:
            InternalShiftTimeComponent(minute, displayZeroFirstDigit, false);
            InternalShiftTimeComponent(hour, displayZeroFirstDigit, second % 2 == 0);
            InternalShiftOut(CharMap[' ']);
            InternalShiftOut(CharMap[' ']);
            break;
        case TimePosition::Split:
            InternalShiftTimeComponent(minute, displayZeroFirstDigit, false);
            InternalShiftOut(CharMap[' ']);
            InternalShiftOut(CharMap[' ']);
            InternalShiftTimeComponent(hour, displayZeroFirstDigit, second % 2 == 0);
            break;
    }
}

void DisplayIV6::ShiftRaw(byte data[])
{
    for (size_t i = 0; i < Digits; i++)
        displayData[i] = data[i];
}

void DisplayIV6::ShiftText(String text)
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

        // TODO: don't be lazy, catch first character being a . or ,
        if ((text[tubeIndex] == '.' || text[tubeIndex] == ',') && tubeIndex > 0)  {  
            displayData[reverse+1] = displayData[reverse+1] | CharMap['.'];
            continue;
        }

        displayData[reverse] = CharMap[text[tubeIndex]];
    }

    // since we shift . and , one to the left, we need to manually check if index 6 (ie tube 7) would've been a dot.
    if (text.length() > Digits && (text[Digits] == '.' || text[Digits] == ',')) {
        displayData[0] = displayData[0] | CharMap['.'];
    }
}

void DisplayIV6::ShiftBlank()
{
    for (size_t tubeIndex = 0; tubeIndex < Digits; tubeIndex++)
        displayData[tubeIndex] = 0b0000000;
}

void DisplayIV6::InternalShiftTimeComponent(int number, bool displayZeroFirstDigit, bool dotOnSecondDigit)
{
    if (number > 99 || number < 0)
        number = 0;

    char secondDigit = TubeDigit[number % 10];
    if (dotOnSecondDigit)
        secondDigit = secondDigit | CharMap['.'];
    InternalShiftOut(secondDigit);

    int firstDigit = number / 10;
    if (firstDigit != 0 or displayZeroFirstDigit)
	    InternalShiftOut(TubeDigit[firstDigit]);
    else
	    InternalShiftOut(CharMap[' ']);
}

void DisplayIV6::InternalShiftOut(byte data)
{
    displayData[shiftOutIndex] = data;
    
    shiftOutIndex++;
    if (shiftOutIndex >= Digits)
        shiftOutIndex = 0;
}