#include "DeviceModeSerialText.h"

bool DeviceModeSerialText::Start() 
{
    #ifdef DISPLAYTYPE_IV4
        display->ShiftText("........");
    #else
        display->ShiftText("...");
    #endif

    Serial.println("Enabled serial text mode");

    return true;
}

bool DeviceModeSerialText::Stop() 
{
    return true;
}

void DeviceModeSerialText::OnSerialDataReceived(String s) 
{
    s.replace("\n", "");
    s.replace("\r", "");
    Serial.println(String('\'') + s + String('\''));
    display->ShiftText(s);
}