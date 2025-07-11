#include "DisplayIV4.h"
#include <SPI.h>
#include "constants.h"

bool DisplayIV4::Initialize()
{
    Serial.println("Initializing IV-4 Display");
    displayTextLength = 0;
    displayText = nullptr;

    pinMode(GPIO_Blank, OUTPUT);
	pinMode(GPIO_Data, OUTPUT);
	pinMode(GPIO_Clock, OUTPUT);
	pinMode(GPIO_Latch, OUTPUT);
    SetPinLow(GPIO_Blank);
    SetPinLow(GPIO_Data);
    SetPinLow(GPIO_Clock);
    SetPinHigh(GPIO_Latch);

    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV16);

    if (currentDimmingStep > MaxDimmingSteps)
        currentDimmingStep = MaxDimmingSteps;

    ShiftBlank();

    return true;
}

void DisplayIV4::OnTick(uint8_t displayModeDelay)
{
    if (!useDisplayText)
        return;

    if (maxOffset <= 0)
        return;

    msSinceScrollTick += displayModeDelay;

    // Scroll sequence: x time idle, scrolling with interval, same idle time, reset.
    if (isScrolling) {
        if(textOffset >= maxOffset) {
            if (msSinceScrollTick >= ScrollEndDelayMs) {
                InternalShiftDisplayTextFromBeginning();
            }
        } else {
            if(msSinceScrollTick >= ScrollIntervalMs) {
                msSinceScrollTick = 0;
                textOffset++;
                InternalShiftDigit(displayText[Digits - 1 + textOffset]);
                InternalCommit();
            }
        }
    } else if (msSinceScrollTick >= ScrollStartDelayMs) {
        isScrolling = true;
        msSinceScrollTick = 0;
    }
}

void DisplayIV4::ShiftCurrentTimeFull(int hour, int minute, int second, bool displayZeroFirstDigit)
{
    useDisplayText = false;

    InternalShiftTimeComponent(hour, displayZeroFirstDigit);
    InternalShiftDigit(CharMap[' ']);
    InternalShiftTimeComponent(minute, displayZeroFirstDigit);
    InternalShiftDigit(CharMap[' ']);
    InternalShiftTimeComponent(second, displayZeroFirstDigit);
    InternalCommit();
}

void DisplayIV4::ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit)
{
    useDisplayText = false;

    InternalShiftDigit(CharMap[' ']);
    InternalShiftTimeComponent(hour, displayZeroFirstDigit);
    InternalShiftDigit(CharMap[' ']);
    InternalShiftDigit(CharMap[' ']);
    InternalShiftTimeComponent(minute, displayZeroFirstDigit);
    InternalShiftDigit(CharMap[' ']);
    InternalCommit();
}

void DisplayIV4::ShiftRaw(byte data[])
{
    useDisplayText = false;

    for (size_t i = 0; i < DisplayBytes; i++)
        displayData[i] = data[i];

    InternalCommit();
}

void DisplayIV4::ShiftText(String text)
{
    ResetDisplayText(text);
}

void DisplayIV4::ShiftBlank()
{
    useDisplayText = false;

    for (size_t i = 0; i < DisplayBytes; i++)
        displayData[i] = 0b00000000;

    InternalCommit();
}

void DisplayIV4::InternalShiftDigit(const uint32_t &tubeDigit)
{
    /*  
        If we could have used a 160 bit / 20 byte data type we could've easily done
        displayData <<= 20;
        displayData = displayData |= (uint32)Number

        But, this is not possible, so we need to manually shift all data across the array by 20 bits
        Our limitation is that displayData needs to be 1-2 byte in size.

        -------------------------------------------------------------------------
        VISUALIZATION
        -------------------------------------------------------------------------
        maskLeft 11110000
        maskRight 00001111

        uint8_t displayData[20] needs << 20 so we have space for digit incoming on the right
        10010011 11000110 10010011 11000110 10010011 11000110
        ^^^^^^^^^^^^^^^^^^^^^^
                | will be lost in the process
            0       1       2         3        4        5
        00000000 00000000 10010011 11000110 10010011 11000110
                            ^^^^
                                [2] & maskRight = 00000011
                                << 4 = 00110000
                                |=  [2 - 2 = 0];
            0       1       2         3        4        5
        00110000 00000000 10010011 11000110 10010011 11000110
        ****                       ^^^^
                                    [3] & maskLeft = 11000000
                                    >> 4 = 00001100
                                    |= [3 - 3 = 0];
            0       1       2         3        4        5
        00111100 00000000 10010011 11000110 10010011 11000110
        ********                       ^^^^
                                        [3] & maskRight = 00000110
                                        << 4 = 01100000
                                        |=  [3 - 2 = 1];
            0       1       2         3        4        5
        00111100 01100000 10010011 11000110 10010011 11000110
        ******** ****
        etc...
    */

    constexpr uint8_t maskLeft = 0b11110000;
    constexpr uint8_t maskRight = 0b00001111;

    for(size_t i = 2; i < DisplayBytes; i++)
    {
        // Take the data from the RIGHT side, and move it to the LEFT
        uint8_t rightOnly = displayData[i] & maskRight;
        rightOnly <<= 4;
        displayData[i-2] = (displayData[i-2] & maskRight) | rightOnly; // wipes left 4 bits & combines with now shifted right

        // Take the data from the LEFT side, and move it to the RIGHT of the byte BEFORE.
        if (i == 2) continue;
        uint8_t leftOnly = displayData[i] & maskLeft;
        leftOnly >>= 4;
        displayData[i-3] = (displayData[i-3] & maskLeft) | leftOnly; // wipes right 4 bits & combines with now shifted left
    }

    // in the array shape of iv4Data, the bits are back-to-front.
    // num8.int8[3] can be ignored, we only need to copy 2,5bytes/20bits
    iv4Data conv;
    conv.int32 = tubeDigit;
    displayData[DisplayBytes-3] = displayData[DisplayBytes-3] & maskLeft;
    displayData[DisplayBytes-3] = displayData[DisplayBytes-3] | conv.int8[2];
    displayData[DisplayBytes-2] = conv.int8[1];
    displayData[DisplayBytes-1] = conv.int8[0];
}

void DisplayIV4::InternalShiftTimeComponent(int number, bool displayZeroFirstDigit)
{
    if (number > 99 || number < 0)
        number = 0;

    int firstDigit = number / 10;
    if (firstDigit != 0 or displayZeroFirstDigit)
	    InternalShiftDigit(TubeDigit[firstDigit]);
    else
	    InternalShiftDigit(CharMap[' ']);

    uint32_t secondDigit = TubeDigit[number % 10];
    InternalShiftDigit(secondDigit);
}

void DisplayIV4::InternalCommit()
{
    SetPinLow(GPIO_Latch);

    for(volatile int i = 0; i < DisplayBytes; i++)
        SPI.transfer(displayData[i]);

    SetPinHigh(GPIO_Latch);
}

void DisplayIV4::ResetDisplayText(String input)
{
    // // ex: Gefeliciteerd! (14) - 8 = 6
    // maxOffset = input.length() - Digits;
    maxOffset = 0;
    ShiftBlank();

    useDisplayText = false;
    if (displayText) {
        free(displayText);
    }

    displayTextLength = 0;
    displayText = new uint32_t[input.length()];

    size_t inputIdx = 0;
    size_t displayTextIdx = 0;
    for (; inputIdx < input.length(); inputIdx++)
    {
        if (input[displayTextIdx] < sizeof(CharMap))
        {
            if (CharMap[input[inputIdx]] == CharPeriod && displayTextIdx > 0 && CharMap[input[inputIdx - 1]] != CharPeriod)
            {
                displayTextIdx--;
                displayText[displayTextIdx] = displayText[displayTextIdx] | CharPeriodInline;
            }
            else
            {
                displayText[displayTextIdx] = CharMap[input[inputIdx]];
            }
        }
        else 
        {
            displayText[displayTextIdx] = CharMap[' '];
        }

        displayTextIdx++;
    }

    displayTextLength = displayTextIdx;
    maxOffset = displayTextLength - Digits;

    InternalShiftDisplayTextFromBeginning();
}

void DisplayIV4::InternalShiftDisplayTextFromBeginning() 
{
    textOffset = 0;
    msSinceScrollTick = 0;
    isScrolling = false;
    useDisplayText = false;
    for (size_t tubeIndex = 0; tubeIndex < Digits; tubeIndex++)
        if (tubeIndex < displayTextLength)
            InternalShiftDigit(displayText[tubeIndex]);
        else
            InternalShiftDigit(CharMap[' ']);

    InternalCommit();
    useDisplayText = true;
}
