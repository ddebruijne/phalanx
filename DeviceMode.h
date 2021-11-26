#ifndef DEVICEMODE_H
#define DEVICEMODE_H

#include "DisplayBase.h"

// Arduino can't use dynamic-cast, so we define the device mode here.
enum EDeviceMode
{
    Unknown,
    Config,
    Normal,
    Spotify
};

class DeviceMode
{
protected:
    DisplayBase *display;

public:
    virtual bool Start() = 0;
    virtual bool Stop() = 0;
    virtual void OnTick() = 0;
    virtual EDeviceMode GetDeviceMode() = 0;
    inline void SetDisplay(DisplayBase* _display) { display = _display; }
};

#endif // DEVICEMODE_H