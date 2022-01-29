#include "DisplayIV12.h"
#include <SPI.h>
#include "constants.h"

bool DisplayIV12::Initialize()
{
    Serial.println("Initializing IV-12 Display");
    bool init = DisplayBase_SevenSegmentVFD::Initialize();

    for(int tubeIndex = 0; tubeIndex < Digits; tubeIndex++)
        displayData[tubeIndex] = B11111111;

    return init;
}

void DisplayIV12::OnTick()
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
            SPI.transfer(displayData[tubeIndex] | B00000001);   // Enables GRID
    }

    SetPinHigh(GPIO_Latch);

    ticksSinceLastWrite++;
    if (ticksSinceLastWrite > currentDimmingStep)
        ticksSinceLastWrite = 0;

}