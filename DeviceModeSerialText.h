#include "DeviceMode.h"

class DeviceModeSerialText : public DeviceMode
{
public:
    bool Start();
    bool Stop();
    void OnTick();
    EDeviceMode GetDeviceMode() { return EDeviceMode::SerialText; }
};