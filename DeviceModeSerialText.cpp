#include "DeviceModeSerialText.h"

bool DeviceModeSerialText::Start() 
{
    #ifdef DISPLAYTYPE_IV4
        display->ShiftText("Waiting for Serial Data");
    #else
        display->ShiftText("wait...");
    #endif

    return true;
}

bool DeviceModeSerialText::Stop() 
{
    return true;
}

void DeviceModeSerialText::OnTick() 
{
    if (Serial.available() <= 0)
        return;

    display->ShiftText(Serial.readStringUntil('\n'));
}