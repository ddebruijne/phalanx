#ifndef DEVICEMODE_H
#define DEVICEMODE_H

#include "DisplayBase.h"
#include "defines.h"

// Arduino can't use dynamic-cast, so we define the device mode here.
enum EDeviceMode
{
    Unknown,
    Config,
    Normal,
    Spotify,
    SerialText,
    StockTicker,
};

static String DeviceModeString[6]
{
    String("Unknown"),
    String("Config"),
    String("Normal"),
    String("Spotify"),
    String("SerialText"),
    String("StockTicker"),
};

class DeviceMode
{
public:
    const uint8_t delayBetweenTicks = 1;  // 1/1000th of a second

protected:
    DisplayBase *display;

public:
    virtual bool Start() = 0;
    virtual bool Stop() = 0;
    virtual void OnTick() = 0;
    virtual void OnSerialDataReceived(String s) = 0;
    virtual EDeviceMode GetDeviceMode() = 0;
    virtual void OnInternetConnectedStateChanged(bool newState) {};
    inline void SetDisplay(DisplayBase* _display) { display = _display; }
};

#endif // DEVICEMODE_H