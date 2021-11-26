#include "DeviceMode.h"

class DeviceModeConfig : public DeviceMode
{
public:
    bool Start();
    bool Stop();
    void OnTick();
    EDeviceMode GetDeviceMode() { return EDeviceMode::Config; }
};