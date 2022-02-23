#include "DeviceMode.h"

class DeviceModeSerialText : public DeviceMode
{
    bool Start();
    bool Stop();
    void OnTick() {};
    void OnSerialDataReceived(String s);
    EDeviceMode GetDeviceMode() { return EDeviceMode::SerialText; };
    };