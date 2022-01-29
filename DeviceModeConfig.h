#include "DeviceMode.h"

class DeviceModeConfig : public DeviceMode
{
    const String* deviceName;

public:
    DeviceModeConfig(const String* _deviceName) { deviceName = _deviceName; }

    bool Start();
    bool Stop();
    void OnTick();
    EDeviceMode GetDeviceMode() { return EDeviceMode::Config; }
};